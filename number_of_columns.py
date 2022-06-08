#! /usr/bin/env python3

# Calculate the number of possible distinct connect-q columns of height 0..n.

def number_of_columns(q: int, n: int):

    rec = [0, 0, 0, 0]

    #for i in range(q - 1):
    #    rec.append(count)

    for i in range(n + 1):
        if i <= q:
            count = 2 * rec[-1] + 1
        else:
            count = sum(rec) + q + 2
        rec.append(count)
        rec = rec[-(q-1):]

    return rec[-1]
