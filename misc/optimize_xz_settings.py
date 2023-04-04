#! /usr/bin/env -S python3 -B

import argparse
import subprocess
import itertools
import random
import multiprocessing

from json_with_comments import parse_json_with_comments_file


def report_xz_performance(filename, bytecount, xz_delta_dist, xz_lzma2_dict, xz_lzma2_lc, xz_lzma2_lp, xz_lzma2_pb, xz_lzma2_mf, xz_lzma2_mode, xz_lzma2_nice, xz_lzma2_depth):

    if bytecount is not None:
        proc_head_args = ["head", "-c", str(bytecount), filename]
        proc_head = subprocess.Popen(proc_head_args, stdout=subprocess.PIPE)
    else:
        proc_head = None

    proc_xz_filter_args = []

    if xz_delta_dist is not None:
        delta_filter_arg="--delta=dist={}".format(xz_delta_dist)
        proc_xz_filter_args.append(delta_filter_arg)

    lzma2_filter_arg="--lzma2=dict={},lc={},lp={},pb={},mf={},mode={},nice={},depth={}".format(xz_lzma2_dict, xz_lzma2_lc, xz_lzma2_lp, xz_lzma2_pb, xz_lzma2_mf, xz_lzma2_mode, xz_lzma2_nice, xz_lzma2_depth)
    proc_xz_filter_args.append(lzma2_filter_arg)

    proc_xz_args = ["xz", "-c"]
    proc_xz_args.extend(proc_xz_filter_args)

    if proc_head is None:
        proc_xz_args.append(filename)
        proc_xz = subprocess.Popen(proc_xz_args, stdout = subprocess.PIPE)
    else:
        proc_xz = subprocess.Popen(proc_xz_args, stdin = proc_head.stdout, stdout = subprocess.PIPE)

    proc_wc_args = ["wc", "-c"]

    proc_wc = subprocess.Popen(proc_wc_args, stdin=proc_xz.stdout, stdout = subprocess.PIPE)

    if proc_head is not None:
        proc_head.wait()
    proc_xz.wait()
    proc_wc.wait()

    compressed_size = int(proc_wc.stdout.read())

    xz_filter_args = " ".join(proc_xz_filter_args)

    return (compressed_size, xz_filter_args)


def inclusive_range(a, b):
    return list(range(a, b + 1))


def main():

    parser = argparse.ArgumentParser()

    parser.add_argument("--samples", type=int, default=100)
    parser.add_argument("--limit", type=int, default=10)
    parser.add_argument("--bytecount", type=int, default=None)
    parser.add_argument("filename", type=str)

    args = parser.parse_args()

    parameter_space_data = parse_json_with_comments_file("optimize_xz_settings.json")

    if args.filename in parameter_space_data:
        parameter_space_definition_key = args.filename
    else:
        parameter_space_definition_key = "default"

    parameter_space_definition = parameter_space_data[parameter_space_definition_key]

    parameter_space_generator = itertools.product(
        parameter_space_definition["delta.dist"],
        parameter_space_definition["lzma2.dict"],
        parameter_space_definition["lzma2.lc"],
        parameter_space_definition["lzma2.lp"],
        parameter_space_definition["lzma2.pb"],
        parameter_space_definition["lzma2.mf"],
        parameter_space_definition["lzma2.mode"],
        inclusive_range(*parameter_space_definition["lzma2.nice.minmax"]),
        parameter_space_definition["lzma2.depth"]
    )

    minimum_nice_value = {
        "hc3": 3,
        "hc4": 4,
        "bt2": 2,
        "bt3": 3,
        "bt4": 4
    }

    parameter_space = [(args.filename, args.bytecount, xz_delta_dist, xz_lzma2_dict, xz_lzma2_lc, xz_lzma2_lp, xz_lzma2_pb, xz_lzma2_mf, xz_lzma2_mode, xz_lzma2_nice, xz_lzma2_depth) for (xz_delta_dist, xz_lzma2_dict, xz_lzma2_lc, xz_lzma2_lp, xz_lzma2_pb, xz_lzma2_mf, xz_lzma2_mode, xz_lzma2_nice, xz_lzma2_depth) in parameter_space_generator if xz_lzma2_lc + xz_lzma2_lp <= 4 and xz_lzma2_nice >= minimum_nice_value[xz_lzma2_mf]]

    parameter_space_samples = random.sample(parameter_space, min(args.samples, len(parameter_space)))

    print()
    print("Using parameter space definition {!r}.".format(parameter_space_definition_key))
    print()
    print("Taking {} samples from a parameter space of size {} ...".format(len(parameter_space_samples), len(parameter_space)))
    print()

    pool = multiprocessing.Pool()
    results = pool.starmap(report_xz_performance, parameter_space_samples)

    sorted_results = sorted(results)

    with open("results.txt", "w") as fo:
        for (compressed_size, command) in sorted_results:
            print("{:10}  {}".format(compressed_size, command), file=fo)

    best_results = sorted_results[:args.limit]

    print("Best {} results:".format(len(best_results)))
    print()
    for (compressed_size, command) in best_results:
        print("{:10}  {}".format(compressed_size, command))
    print()

    worst_results = sorted_results[-args.limit:]

    print("Worst {} results:".format(len(worst_results)))
    print()
    for (compressed_size, command) in worst_results:
        print("{:10}  {}".format(compressed_size, command))
    print()


if __name__ == "__main__":
    main()
