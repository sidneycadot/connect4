#! /usr/bin/env python3

import time
import os
import subprocess
import datetime

def run():
    args = ["lsof", "-o", "connect4_7x6.new"]
    result = subprocess.run(args, capture_output=True)

    offsets = []
    lines = result.stdout.decode().split("\n")
    for line in lines:
        if line.startswith("xz"):
            parts = line.split()
            pid = int((parts[1]))
            assert parts[6].startswith("0x")
            offset = int(parts[6][2:], 16)
            offsets.append((pid, offset))

    offsets.sort()
    offsets = [offset for (pid, offset) in offsets]

    sizes = []
    for filename in ["bt2.xz", "bt3.xz", "bt4.xz"]:
        stat_result = os.stat(filename)
        sizes.append((filename, stat_result.st_size))

    print(datetime.datetime.now().isoformat())
    print()
    for ((filename, size), offset) in zip(sizes, offsets):
        perc_complete = offset / 158619626501.91
        compression_ratio = offset / size
        print("{} offset {:14} xz_size {:14} ; perc_complete {:10.6f} compression_ratio 1:{:.6f}".format(filename, offset, size, perc_complete, compression_ratio))
    print()

def main():
    run()
    while True:
        time.sleep(-time.time() % 3600.0)
        run()

if __name__ == "__main__":
    main()
