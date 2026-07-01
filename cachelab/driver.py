#!/usr/bin/env python3
#
# driver.py - Modern Python 3 version for Cache Lab autograding.
#
import subprocess
import re
import sys
import optparse

def computeMissScore(miss, lower, upper, full_score):
    if miss <= lower:
        return full_score
    if miss >= upper: 
        return 0
    score = (miss - lower) * 1.0 
    range_val = (upper - lower) * 1.0
    return round((1 - score / range_val) * full_score, 1)

def main():
    maxscore = {
        'csim': 27,
        'transc': 1,
        'trans32': 8,
        'trans64': 8,
        'trans61': 10
    }

    p = optparse.OptionParser()
    p.add_option("-A", action="store_true", dest="autograde", help="emit autoresult string")
    opts, args = p.parse_args()
    autograde = opts.autograde

    # Part A: Test Cache Simulator
    print("Part A: Testing cache simulator")
    print("Running ./test-csim")
    # 這裡我們確保調用的是你在本地用 make 編譯出來的 64 位元版本，不使用預編譯古董
    p = subprocess.Popen("./test-csim", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout_data, _ = p.communicate()
    stdout_data = stdout_data.decode('utf-8', errors='ignore')

    resultsim = []
    lines = stdout_data.split('\n')
    for line in lines:
        if re.match("TEST_CSIM_RESULTS", line):
            resultsim = re.findall(r'(\d+)', line)
        else:
            print(f"{line}")

    if not resultsim:
        resultsim = [0, 0]

    # Part B: Test Transpose Function
    print("Part B: Testing transpose function")
    
    # 32x32
    print("Running ./test-trans -M 32 -N 32")
    p = subprocess.Popen("./test-trans -M 32 -N 32 | grep TEST_TRANS_RESULTS", shell=True, stdout=subprocess.PIPE)
    out32 = p.communicate()[0].decode('utf-8', errors='ignore')
    result32 = re.findall(r'(\d+)', out32)
    if not result32: result32 = [0, "2147483647"]

    # 64x64
    print("Running ./test-trans -M 64 -N 64")
    p = subprocess.Popen("./test-trans -M 64 -N 64 | grep TEST_TRANS_RESULTS", shell=True, stdout=subprocess.PIPE)
    out64 = p.communicate()[0].decode('utf-8', errors='ignore')
    result64 = re.findall(r'(\d+)', out64)
    if not result64: result64 = [0, "2147483647"]

    # 61x67
    print("Running ./test-trans -M 61 -N 67")
    p = subprocess.Popen("./test-trans -M 61 -N 67 | grep TEST_TRANS_RESULTS", shell=True, stdout=subprocess.PIPE)
    out61 = p.communicate()[0].decode('utf-8', errors='ignore')
    result61 = re.findall(r'(\d+)', out61)
    if not result61: result61 = [0, "2147483647"]

    # Compute Scores
    csim_cscore = int(resultsim[0])
    miss32 = int(result32[1])
    miss64 = int(result64[1])
    miss61 = int(result61[1])

    trans32_score = computeMissScore(miss32, 300, 600, maxscore['trans32']) * int(result32[0])
    trans64_score = computeMissScore(miss64, 1300, 2000, maxscore['trans64']) * int(result64[0])
    trans61_score = computeMissScore(miss61, 2000, 3000, maxscore['trans61']) * int(result61[0])
    total_score = csim_cscore + trans32_score + trans64_score + trans61_score

    # Summary Output
    print("\nCache Lab summary:")
    print("%-22s%8s%10s%12s" % ("", "Points", "Max pts", "Misses"))
    print("%-22s%8.1f%10d" % ("Csim correctness", csim_cscore, maxscore['csim']))

    for name, score, max_p, m in [("Trans perf 32x32", trans32_score, maxscore['trans32'], miss32),
                                  ("Trans perf 64x64", trans64_score, maxscore['trans64'], miss64),
                                  ("Trans perf 61x67", trans61_score, maxscore['trans61'], miss61)]:
        miss_str = "invalid" if m == 2147483647 else str(m)
        print("%-22s%8.1f%10d%12s" % (name, score, max_p, miss_str))

    print("%-22s%8.1f%10d" % ("Total points", total_score, sum(maxscore.values()) - maxscore['transc']))

    if autograde:
        print(f"\nAUTORESULT_STRING={total_score:.1f}:{miss32}:{miss64}:{miss61}")

if __name__ == "__main__":
    main()
    