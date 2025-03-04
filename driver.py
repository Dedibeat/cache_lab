#!/usr/bin/env python3
#
# driver.py - The driver tests the correctness of the student's cache
#     simulator and the correctness and performance of their transpose
#     function.
#

import subprocess
import re
import os
import sys
import argparse

#
# computeMissScore - compute the score depending on the number of
# cache misses
#
def computeMissScore(miss, lower, upper, full_score):
    if miss <= lower:
        return full_score
    if miss >= upper: 
        return 0

    score = (miss - lower) * 1.0 
    score_range = (upper - lower) * 1.0
    return round((1 - score / score_range) * full_score, 1)

#
# main - Main function
#
def main():
    # Configure max scores
    maxscore = {
        'csim': 27,
        'transc': 1,
        'trans32': 8,
        'trans64': 8,
        'trans61': 10
    }

    # Parse command-line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-A", action="store_true", dest="autograde", 
                        help="emit autoresult string for Autolab")
    opts = parser.parse_args()
    autograde = opts.autograde

    # Check the correctness of the cache simulator
    print("Part A: Testing cache simulator")
    print("Running ./test-csim")
    p = subprocess.Popen("./test-csim", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout_data, _ = p.communicate()
    stdout_data = stdout_data.decode("utf-8").split("\n")

    # Extract results
    resultsim = []
    for line in stdout_data:
        if re.match("TEST_CSIM_RESULTS", line):
            resultsim = [int(x) for x in re.findall(r'(\d+)', line)]
        else:
            print(line)

    # Check correctness and performance of the transpose function
    def run_test_trans(m, n):
        print(f"Running ./test-trans -M {m} -N {n}")
        cmd = f"./test-trans -M {m} -N {n}"
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout_data, _ = p.communicate()
        
        # Extract relevant lines
        output_lines = stdout_data.decode("utf-8").split("\n")
        result = []
        for line in output_lines:
            if "TEST_TRANS_RESULTS" in line:
                result = re.findall(r'(\d+)', line)
                break
        
        return [int(x) for x in result] if result else [0, 2**31 - 1]  # Default if no output


    print("Part B: Testing transpose function")
    result32 = run_test_trans(32, 32)
    result64 = run_test_trans(64, 64)
    result61 = run_test_trans(61, 67)

    # Compute scores
    csim_cscore = resultsim[0] if resultsim else 0
    trans_cscore = result32[0] * result64[0] * result61[0]

    miss32 = result32[1] if len(result32) > 1 else 2**31-1
    miss64 = result64[1] if len(result64) > 1 else 2**31-1
    miss61 = result61[1] if len(result61) > 1 else 2**31-1

    trans32_score = computeMissScore(miss32, 300, 600, maxscore['trans32']) * result32[0]
    trans64_score = computeMissScore(miss64, 1300, 2000, maxscore['trans64']) * result64[0]
    trans61_score = computeMissScore(miss61, 2000, 3000, maxscore['trans61']) * result61[0]

    total_score = csim_cscore + trans32_score + trans64_score + trans61_score

    # Print results
    print("\nCache Lab summary:")
    print("{:<22}{:>8}{:>10}{:>12}".format("", "Points", "Max pts", "Misses"))
    print("{:<22}{:>8.1f}{:>10d}".format("Csim correctness", csim_cscore, maxscore['csim']))

    def print_trans_perf(label, score, max_pts, misses):
        miss_str = "invalid" if misses == 2**31-1 else str(misses)
        print("{:<22}{:>8.1f}{:>10d}{:>12}".format(label, score, max_pts, miss_str))

    print_trans_perf("Trans perf 32x32", trans32_score, maxscore['trans32'], miss32)
    print_trans_perf("Trans perf 64x64", trans64_score, maxscore['trans64'], miss64)
    print_trans_perf("Trans perf 61x67", trans61_score, maxscore['trans61'], miss61)

    print("{:>22}{:>8.1f}{:>10d}".format("Total points", total_score,
                                         maxscore['csim'] + maxscore['trans32'] +
                                         maxscore['trans64'] + maxscore['trans61']))
    
    # Emit autoresult string for Autolab if called with -A option
    if autograde:
        autoresult = f"{total_score:.1f}:{miss32}:{miss64}:{miss61}"
        print("\nAUTORESULT_STRING={}".format(autoresult))

# Execute main only if called as a script
if __name__ == "__main__":
    main()
