#! /usr/bin/env python3

"""Print column encodings of connect-4."""


def to_ternary(z: int, num_digits: int, digit_characters: str) -> str:
    """Convert an integer to a ternary representation."""
    return to_ternary(z // 3, num_digits - 1, digit_characters) + digit_characters[z % 3] if num_digits else ""


def enumerate_possible_columns(n: int, q: int) -> None:
    """Print a table of all possible columns in connect-q of height n."""

    # Generate all possible columns as ternary integer values.

    to_be_processed = [""]
    valid_columns = []
    while to_be_processed:
        column = to_be_processed.pop()
        column_as_integer = int("0" + column, 3)
        valid_columns.append(column_as_integer)

        if len(column) == n:
            continue

        # Check if the last q entries are the same.
        if len(column) >= q and len(set(column[:q])) == 1:
            continue

        # We can still add a chip for either player.
        to_be_processed.append("1" + column)
        to_be_processed.append("2" + column)

    # Sort possible column values just enumerated.

    valid_columns.sort()

    # Print the possible columns as a table.

    print("+------------------+----------------------+----------------------+----------------------+")
    print("|  encoded_decimal | column_value_decimal | column_value_ternary | column_top_to_bottom |")
    print("+------------------+----------------------+----------------------+----------------------+")
    for (encoded, ternary) in enumerate(valid_columns):
        print("|{:11d}       |{:13d}         | {:>13s}        | {:>13s}        |".format(
            encoded, ternary, to_ternary(ternary, n, "012"), to_ternary(ternary, n, ".AB")))
    print("+------------------+----------------------+----------------------+----------------------+")


def main():
    # Enumerate the encoded -> ternary mapping of connect-4 with column height 6.
    enumerate_possible_columns(6, 4)


if __name__ == "__main__":
    main()
