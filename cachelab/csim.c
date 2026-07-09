#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h> // to use atoi()
#include <unistd.h> // to use getopt()
#include <stdint.h> // to use uint64_t
#include "cachelab.h"
/* for verbose usage */
#define CACHE_HIT 0
#define CACHE_MISS 1
#define CACHE_EVICTION 2

/* a cache is a 2D array of cache lines */
typedef struct {
    int valid;
    uint64_t tag;
    int lru_counter;
} cache_line;

typedef struct {
    uint64_t tag;
    uint64_t set;
    uint64_t block;
} address_parts;

typedef struct {
    int hits;
    int misses;
    int evictions;
} cache_stat;

// use malloc to allocate cache_line *cache，大小 (1 << s) * E
cache_line *init_cache(int s, int E){
    int S = (1 << s);
    // treat this as one dimensional array: access element with cache[set * E + way]
    // allocate S*E contiguous memorys of the size of cache_line
    cache_line *cache = malloc(sizeof(*cache) * S * E);
    return cache;
}

// call address parsing on each L/S/M
void address_parse_safely(uint64_t address, int s, int b, address_parts *cache_address_part){
    // this is where to cut the 64-bit address into
    // tag / set index / block
    // s is for number of bits on set
    uint64_t block_mask = (1ULL << b) - 1;
    cache_address_part->block = address & block_mask;
    uint64_t set_mask = (1ULL << s) - 1;
    cache_address_part->set = (address >> b) & set_mask;
    cache_address_part->tag = address >> (s + b);
}

/* hit/miss/eviction evaluate function */
int evaluate_cache_set(cache_line *cache, int s, int E, int b, 
    address_parts *cache_address_part, cache_stat *cache_stats){
    /* use tag to evaluate hit/miss/update LRU in the whole set */
    static int global_clock = 0;
    int empty_line_idx = -1; // empty spot not found yet

    /* for finding the line with smallest lru_counter */
    int evict_line_idx = cache_address_part->set * E;

    for (int way = 0; way < E; way++){
        // access element with cache[set * E + way]
        // as in 2D-array
        int line_idx = cache_address_part->set * E + way;
        if (cache[line_idx].valid == 1 && 
            cache[line_idx].tag == cache_address_part->tag){
            /* hit */
            cache[line_idx].lru_counter = global_clock++;
            cache_stats->hits++;
            return CACHE_HIT;
        }
        if (cache[line_idx].valid == 0 && empty_line_idx == -1){
            empty_line_idx = line_idx;
        }
        /* find the line with smallest lru_counter */
        if (cache[line_idx].valid == 1 && 
            cache[line_idx].lru_counter < cache[evict_line_idx].lru_counter){
            evict_line_idx = line_idx;
        }
    }
    /* cold miss */
    cache_stats->misses++;
    if (empty_line_idx != -1){
        cache[empty_line_idx].valid = 1;
        cache[empty_line_idx].tag = cache_address_part->tag; // update tag
        cache[empty_line_idx].lru_counter = global_clock++;
        return CACHE_MISS;
    }
    /* miss-eviction */
    cache_stats->evictions++;
    /* already found the smallest index on top */
            // cache[idx].valid are all 1, no need to update valid bit
    cache[evict_line_idx].tag = cache_address_part->tag; // update tag
    cache[evict_line_idx].lru_counter = global_clock++;
    return CACHE_EVICTION;
}

void process_tracefile(char *tracefile, cache_line *cache, 
    int s, int E, int b, cache_stat *cache_stats, int verbose){
    // fopen → while(fscanf(...)) loop → 每行判斷 L/S/M → call hit/miss 邏輯 → fclose
    FILE *pFile = fopen(tracefile, "r");
    if (pFile == NULL) {
        printf("Error opening tracefile\n");
        exit(1);
    }

    char identifier;
    uint64_t address;
    int size;

    /* for verbose usage */
    int evaluate_state;
    const char *state_msg[] = {"hit", "miss", "miss eviction"};

    while (fscanf(pFile, " %c %lx,%d", &identifier, &address, &size) == 3) {
        if (identifier == 'I') continue;  // instruction load, skip and do nothing
        if (verbose) printf("%c %lx,%d", identifier, address, size); // first phase

        address_parts cache_address_part;
        address_parse_safely(address, s, b, &cache_address_part);
        // identifier: L (Load) / S (Store) / M (Modify), here to process hit/miss/eviction evaluation
        if (identifier == 'L' || identifier == 'S') {
            evaluate_state = evaluate_cache_set(cache, s, E, b, &cache_address_part, cache_stats);
            if(verbose){ // second phase possibility
                printf(" %s", state_msg[evaluate_state]); 
            }
        }
        // if it's M (Modify)，access 2 times（Load and Store）
        else if (identifier == 'M'){ 
            /* Load */
            int evaluate_state_1 = evaluate_cache_set(cache, s, E, b, &cache_address_part, cache_stats);
            // second phase possibility
            if(verbose){
                printf(" %s", state_msg[evaluate_state_1]);
            }
            /* Store */
            int evaluate_state_2 = evaluate_cache_set(cache, s, E, b, &cache_address_part, cache_stats);
            if(verbose){
                printf(" %s", state_msg[evaluate_state_2]);
            }
        }
        // third phase
        if (verbose) {
            printf("\n");
        }
    }
    fclose(pFile);
}

void free_cache(cache_line *cache){
    free(cache); // no need to use sizeof()
}

/* usage - Print usage info */
void usage(char *argv[]){
    printf("Usage: %s -s <sets> -E <ways> -b <blocks> -t <tracefile> [-v] [-h]\n", argv[0]);
    printf("Options:\n");
    printf("  -s <sets>      Number of bits to represent set index\n");
    printf("  -E <ways>      Number of lines per set\n");
    printf("  -b <blocks>    Number of bits of block offset\n");
    printf("  -t <tracefile> Name of the valgrind trace to replay\n");
    printf("  -v <verbose>   Optional: Print trace info (hit/miss/eviction)\n");
    printf("  -h <help>      Optional: Print this help message.\n");
    printf("Example: %s -s 4 -E 1 -b 4 -t traces/yi.trace -v\n", argv[0]);
}

int main(int argc, char **argv) {
    // argument parsing
    // use getopt() to read -s -E -b -t -v -h these 6 flags
    int s = -1, E = -1, b = -1;
    int verbose = 0;
    char *tracefile = NULL; // "trace.f1.txt"
    int opt;

    while ((opt = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch(opt) {
            case 's': s = atoi(optarg); break;
            case 'E': E = atoi(optarg); break;
            case 'b': b = atoi(optarg); break;
            case 't': tracefile = optarg; break;
            case 'v': verbose = 1; break;
            case 'h': usage(argv); exit(0);
            default: printf("wrong argument\n"); exit(1);    
        }
    }

    if (s <= -1 || E <= -1 || b <= -1 || tracefile == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        usage(argv);
        exit(1);
    }

    cache_stat cache_stats = {
        .hits      = 0, 
        .misses    = 0, 
        .evictions = 0
    };

    cache_line *cache = init_cache(s, E);
    process_tracefile(tracefile, cache, s, E, b, &cache_stats, verbose);
    free_cache(cache);
    printSummary(cache_stats.hits, cache_stats.misses, cache_stats.evictions);
    return 0;
}