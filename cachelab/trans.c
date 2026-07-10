/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"
// #define min(x, y) ((x) < (y) ? (x) : (y))
#define BLOCK_NUM 8

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

static inline int min(int x, int y){
    return x < y ? x : y;
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

 /* transpose the matrix with blocking technique to improve temporal locality
     * cannot use loop rearranging to improve spatial locality
     * with 2 inductive variables only 
     */

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    /* 8 by 8 blocking + loop unrolling */ 
    int bi, bj, i, j; // need bi, bj for blocking
    int tmp;
    int t0, t1, t2, t3;
    int t4, t5, t6, t7;
    int jump;
    
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < min(bi+8, N); i++){
                for (j = bj; j < min(bj+8, M); j += jump){
                    /* conditional loop unrolling */
                    if (j + 8 <= min(bj+8, M)){
                        jump = 8;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        t2 = A[i][j+2];
                        t3 = A[i][j+3];
                        t4 = A[i][j+4];
                        t5 = A[i][j+5];
                        t6 = A[i][j+6];
                        t7 = A[i][j+7];

                        B[j+1][i] = t1;
                        B[j+2][i] = t2;
                        B[j+3][i] = t3;
                        B[j+4][i] = t4;
                        B[j+5][i] = t5;
                        B[j+6][i] = t6;
                        B[j+7][i] = t7;
                        B[j][i] = t0;
                    }
                    else if (j + 4 <= min(bj+4, M)) {
                        jump = 4;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        t2 = A[i][j+2];
                        t3 = A[i][j+3];

                        B[j+1][i] = t1;
                        B[j+2][i] = t2;
                        B[j+3][i] = t3;
                        B[j][i] = t0;
                    }
                    else if (j + 2 <= min(bj+2, M)) {
                        jump = 2;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        
                        B[j+1][i] = t1;
                        B[j][i] = t0;
                    }
                    else{
                        jump = 1;
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

char trans_test_8_by_8_cond_loop_unroll_desc[] = "Transpose 8 by 8 block + conditional loop unroll";
void trans_test_8_by_8_cond_loop_unroll(int M, int N, int A[N][M], int B[M][N])
{
    /* 8 by 8 blocking + conditional loop unrolling */ 
    int bi, bj, i, j; // need bi, bj for blocking
    int tmp;
    int t0, t1, t2, t3;
    int t4, t5, t6, t7;
    int jump;
    
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < min(bi+8, N); i++){
                for (j = bj; j < min(bj+8, M); j += jump){
                    /* conditional loop unrolling */
                    if (j + 8 <= min(bj+8, M)){
                        jump = 8;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        t2 = A[i][j+2];
                        t3 = A[i][j+3];
                        t4 = A[i][j+4];
                        t5 = A[i][j+5];
                        t6 = A[i][j+6];
                        t7 = A[i][j+7];

                        B[j+1][i] = t1;
                        B[j+2][i] = t2;
                        B[j+3][i] = t3;
                        B[j+4][i] = t4;
                        B[j+5][i] = t5;
                        B[j+6][i] = t6;
                        B[j+7][i] = t7;
                        B[j][i] = t0;
                    }
                    else if (j + 4 <= min(bj+4, M)) {
                        jump = 4;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        t2 = A[i][j+2];
                        t3 = A[i][j+3];

                        B[j+1][i] = t1;
                        B[j+2][i] = t2;
                        B[j+3][i] = t3;
                        B[j][i] = t0;
                    }
                    else if (j + 2 <= min(bj+2, M)) {
                        jump = 2;
                        t0 = A[i][j];
                        t1 = A[i][j+1];
                        
                        B[j+1][i] = t1;
                        B[j][i] = t0;
                    }
                    else{
                        jump = 1;
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
        }
    }
}

char trans_perfect_8_by_8_desc[] = "8x8 Blocking + Full Unrolling Submission";
void trans_perfect_8_by_8(int M, int N, int A[N][M], int B[M][N])
{
    int bi, bj, i;
    int t0, t1, t2, t3, t4, t5, t6, t7;

    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < min(bi+8, N); i++) {
                
                // 步驟 1：徹底拔掉內層 j 迴圈，一口氣把 A 的一整行（8個整數）讀進暫存器
                // 這會觸發 1 次快取行載入，隨後 7 次存取都是暫存器命中，此時完全不碰 B
                t0 = A[i][bj];   t1 = A[i][bj+1]; t2 = A[i][bj+2]; t3 = A[i][bj+3];
                t4 = A[i][bj+4]; t5 = A[i][bj+5]; t6 = A[i][bj+6]; t7 = A[i][bj+7];

                // 步驟 2：一口氣寫入 B 的對應欄位
                // 這時候快取隨便怎麼踢 A 都沒關係了，因為 A 的資料已經安全躺在 CPU 暫存器裡
                B[bj][i]   = t0; B[bj+1][i] = t1; B[bj+2][i] = t2; B[bj+3][i] = t3;
                B[bj+4][i] = t4; B[bj+5][i] = t5; B[bj+6][i] = t6; B[bj+7][i] = t7;
            }
        }
    }
}

char trans_test_16_by_16_desc[] = "Transpose 16 by 16 blocking only";
void trans_test_16_by_16(int M, int N, int A[N][M], int B[M][N])
{
    // 16 by 16 blocking only
    int bi, bj, i, j; // bi, bj for blocking
    int tmp;
    for (bi = 0; bi < N; bi += 16) {
        for (bj = 0; bj < M; bj += 16) {
            for (i = bi; i < bi + 16; i++){
                for (j = bj; j < bj + 16; j++){
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}

char trans_test_8_by_8_loop_unroll_desc[] = "Transpose 8 by 8 block + 8-way loop unroll";
void trans_test_8_by_8_loop_unroll(int M, int N, int A[N][M], int B[M][N])
{
    /* 8 by 8 blocking + 8-way loop unrolling */ 
    int bi, bj, i, j; // need bi, bj for blocking
    int t0, t1, t2, t3;
    int t4, t5, t6, t7;
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < bi + 8; i++){
                for (j = bj; j < bj + 8; j += 8){
                    t0 = A[i][j];
                    t1 = A[i][j+1];
                    t2 = A[i][j+2];
                    t3 = A[i][j+3];
                    t4 = A[i][j+4];
                    t5 = A[i][j+5];
                    t6 = A[i][j+6];
                    t7 = A[i][j+7];

                    B[j][i] = t0;
                    B[j+1][i] = t1;
                    B[j+2][i] = t2;
                    B[j+3][i] = t3;
                    B[j+4][i] = t4;
                    B[j+5][i] = t5;
                    B[j+6][i] = t6;
                    B[j+7][i] = t7;
                }
            }
        }
    }
}

char trans_test_8_by_8_small_loop_unroll_desc[] = "Transpose 8 by 8 block + 4-way loop unroll";
void trans_test_8_by_8_small_loop_unroll(int M, int N, int A[N][M], int B[M][N])
{
    // 8 by 8 blocking + 4-way loop unrolling
    int bi, bj, i, j; // we need bi, bj for blocking
    int t0, t1, t2, t3;
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < bi + 8; i++){
                for (j = bj; j < bj + 8; j += 4){
                    t0 = A[i][j];
                    t1 = A[i][j+1];
                    t2 = A[i][j+2];
                    t3 = A[i][j+3];

                    B[j][i] = t0;
                    B[j+1][i] = t1;
                    B[j+2][i] = t2;
                    B[j+3][i] = t3;
                }
            }
        }
    }
}

char trans_test_8_by_8_diag_desc[] = "Transpose 8 by 8 blocking + diagonal value delay write test";
void trans_test_8_by_8_diag(int M, int N, int A[N][M], int B[M][N])
{
    // 8 by 8 blocking but no loop unrolling
    int bi, bj, i, j; // bi, bj for blocking
    // int t0, t1, t2, t3;
    // int t4, t5, t6, t7;
    int tmp, diag_val[8];
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < bi + 8; i++){
                for (j = bj; j < bj + 8; j++){
                    if (i == j && bi == bj){
                        diag_val[i - bi] = A[i][i];
                    }
                    else{
                        tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }
            }
            /* write in diagonal value */
            if (bi == bj) {
                for (int k = 0; k < 8 && bi + k < N; k++){
                    B[bi+k][bi+k] = diag_val[k];
                }
            }
        }
    }
}

char trans_test_8_by_8_desc[] = "Transpose 8 by 8 blocking only test";
void trans_test_8_by_8(int M, int N, int A[N][M], int B[M][N])
{
    // 8 by 8 blocking but no loop unrolling
    // we need bi, bj for blocking
    int bi, bj, i, j;
    // int t0, t1, t2, t3;
    // int t4, t5, t6, t7;
    int tmp;
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi; i < bi + 8; i++){
                for (j = bj; j < bj + 8; j++){
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}

char trans_test_4_by_4_loop_unrolling_desc[] = "Transpose 4 by 4 + loop unrolling test";
void trans_test_4_by_4_loop_unrolling(int M, int N, int A[N][M], int B[M][N])
{
    /* 4 by 4 blocking + loop unrolling */
    int bi, bj, i, j; // need bi, bj for blocking
    int t0, t1, t2, t3;
    for (bi = 0; bi < N; bi += 4) {
        for (bj = 0; bj < M; bj += 4) {
            for (i = bi; i < min(bi+4, N); i++){ 
                for (j = bj; j < min(bj+4, M); j += 4){
                    t0 = A[i][j];
                    t1 = A[i][j+1];
                    t2 = A[i][j+2];
                    t3 = A[i][j+3];

                    B[j][i] = t0;
                    B[j+1][i] = t1;
                    B[j+2][i] = t2;
                    B[j+3][i] = t3;
                }
            }
        }
    }
}

char trans_test_4_by_4_desc[] = "Transpose 4 by 4 blocking only test";
void trans_test_4_by_4(int M, int N, int A[N][M], int B[M][N])
{
    /* 4 by 4 blocking without loop unrolling */
    int bi, bj, i, j; // need bi, bj for blocking
    int tmp;
    for (bi = 0; bi < N; bi += 4) {
        for (bj = 0; bj < M; bj += 4) {
            for (i = bi; i < bi + 4; i++){
                for (j = bj; j < bj + 4; j++){
                    tmp = A[i][j];
                    B[j][i] = tmp;
                }
            }
        }
    }
}

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose, baseline performance.";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans_test_8_by_8_cond_loop_unroll, trans_test_8_by_8_cond_loop_unroll_desc);
    registerTransFunction(trans_perfect_8_by_8, trans_perfect_8_by_8_desc);
    // registerTransFunction(trans_test_16_by_16, trans_test_16_by_16_desc);
    // registerTransFunction(trans_test_8_by_8_loop_unroll, trans_test_8_by_8_loop_unroll_desc);
    // registerTransFunction(trans_test_8_by_8_small_loop_unroll, trans_test_8_by_8_small_loop_unroll_desc);
    // registerTransFunction(trans_test_8_by_8_diag, trans_test_8_by_8_diag_desc);
    // registerTransFunction(trans_test_8_by_8, trans_test_8_by_8_desc);
    // registerTransFunction(trans_test_4_by_4_loop_unrolling, trans_test_4_by_4_loop_unrolling_desc);
    // registerTransFunction(trans_test_4_by_4, trans_test_4_by_4_desc);

    // registerTransFunction(transpose_submit, transpose_submit_desc); 16 by 16
    registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}