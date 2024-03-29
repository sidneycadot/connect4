
=================================================
A complete lookup table for the game of Connect-4
=================================================

Sidney Cadot, March 2023

1. Introduction
---------------

This document describes "connect4_7x6.dat", a single-file lookup table containing all reachable game positions for the
game of connect-4, together with their game-theoretical outcomes. The lookup table allows for optimal play from any
reachable game position by performing a small number of queries into the table. Since the table is sorted, queries can
be implemented using a fast binary search.

1.1. Credits and copyright
--------------------------

The connect-4 lookup table was generated by Sidney Cadot. When using this data for research or otherwise, you are
kindly requested to include a reference to:

    A complete lookup table for the game of Connect-4
    Sidney Cadot
    https://github.com/sidneycadot/connect4/blob/main/7x6/README.txt

The lookup table data itself is in the public domain.

1.2 How the lookup table was made
---------------------------------

The lookup table was generated using the code that can be found on Github:

  http://www.github.com/sidneycadot/connect4

The code uses a combination of a shell script, a C++ program, and the standard Unix "sort" tool to make two passes
over the game's state graph, and finally to collect the data.

In the first (forward) pass, the game tree is expanded, starting from the initial (empty) board, by making valid moves.
This is done in "generations", where generation 0 corresponds to the empty board, and generation 42 corresponds to
all boards that are completely filled up. In general, the generation number corresponds to the number of moves that
have been played on the board so far.

After each generation, a "sort and uniquify" pass is made over all board positions just reached, to ensure that each
achievable board position is only generated once.

The forward pass ends when all possible boards up to and including generation 42 have been generated.

In the second (backward) pass, the board positions are annotated by selecting, for each board position, the optimal
move for the player having the move, by inspecting all possible outcomes of each possible move and selecting a move
that has the best possible result for the player having the move. Starting at generation 41 (for generation 42, the
outcome is known by inspection) and working back to generation 0, this yields one file per generation with all board
positions in that generation annotated by their game-theoretical outcome.

As a last step, the data for all generations is combined, sorted, and converted to a compact binary format, resulting
in the file "connect4_7x6.dat" that contains the complete lookup table for all reachable board positions. The format of
this file is documented in Section 3.

The entire process of the first, second, and final passes took about 5 months on a reasonably fast (2022) computer. The
performance bottlenecks are (1) the sluggish performance of stream-based I/O in the C++ program; (2) the conversion
from board positions from and to ASCII to ensure that the Unix 'sort' tool can handle them; and (3) the Unix 'sort'
tool itself. A focused effort to optimize performance could probably bring down the required time to generate the full
lookup table by about an order of magnitude.

1.3. Possible uses of the dataset for research
----------------------------------------------

Finding the optimal move in a deterministic, full-information, two-player game is a challenge that has only been fully
accomplished for a small number of games.

There is an interesting tradeoff between performing a full search, perhaps optimized for performance using techniques
such as pruning and caching, which is CPU-intensive but compact, versus an approach using a lookup-table which is cheap
in terms of CPU cycles, but requires a large amount of (external) memory.

Other approaches that fall between these extremes are possible, including hybrid approaches with a partial lookup table
combined with a local search, and approaches based on machine learning techniques.

It is hoped that the availability of this complete connect-4 dataset will serve as an interesting benchmark for
research into different techniques for perfect knowledge representation and machine learning. Since it enables
high-performance determination of optimal moves for all reachable positions in the game of connect-4, it can serve,
for example, as both a training-set and a validation-set for building neural networks and assessing their performance.

1.4. Getting the lookup table over the internet
-----------------------------------------------

The lookup table file is distributed in compressed form (Section 2.2) using BitTorrent. Any modern BitTorrent client
should be able to download it using the following "Magnet" link:

    magnet:?xt=urn:btih:c0452fbf14327ed6f14b9c976e8c3223bf001807&dn=connect4%5F7x6.dat.xz

In case you are not familiar with BitTorrent, we recomment the 'qBittorent' client that can be downloaded from:

    https://www.qbittorrent.org/

Programs that demonstrate how to use the lookup table are not included; those can be found on Github:

    https://github.com/sidneycadot/connect4/

2. Data file description
------------------------

The data file is available in both uncompressed and compressed forms. The uncompressed form allows fast lookup of the
game-theoretical outcome of any game position, as described in Section 3.

Because of the considerable size of the uncompressed file (14.43 TiB), the file is distributed in compressed form.
The compressed file is about 45x smaller than the uncompressed file, but still rather big (326.20 GiB).

2.1. The uncompressed connect-4 data file
-----------------------------------------

The uncompressed file 'connect4_7x6.dat' consists of 2,265,994,664,313 entries of 7 bytes each, for a total of
15,861,962,650,191 bytes (or 14.43 TiB).

This file had the following hashes:

md5sum ......... 6fb030b5926a9a3d93cfcbfaa6646e63
sha1sum ........ 221170bd81c1f5cd2d760a7032a6284b0e3cda96
sha224sum ...... 706342ec241da40c696fea3618639b157b90544be52fd689a2406509
sha256sum ...... 13c5edc061e7d1b1dcd7501c0b7b8018def9147895a6bd28ef1ea32930596d4f
sha384sum ...... 036c5cc4e2e1e6371f1df74f5b09145109a31e7d38e5dba575f81176f4769ae0af787b219b50e49fbd161988d13c1805
sha512sum ...... c6252409f256c63586e5345ed7d00e1ab2f54265c8425d51d6ee735a7165b5ab\
                 14b78836f0f345d28ec3425dde451906ab8c2e6458bc9fc317a87fafe9992dd2

The format of the individual entries is described in Section 3.

2.2. The compressed connect-4 data file
---------------------------------------

The compressed file 'connect4_7x6.dat.xz' has a size of 350,251,723,872 bytes (or 326.20 GiB).

This file had the following hashes:

md5sum ......... c46d914e7cf4368c1471771d6faa3896
sha1sum ........ 8e1033a4c3f282e20c1806d3eab0461ea4df3154
sha224sum ...... f00b2bfe508b3ccb4a5c602e3848ad42900c80b6f8b53d462fecabbf
sha256sum ...... f50738970ac5063d8d3834877dbbfe733b2aac4bf68e7acba2056efc947df3ba
sha384sum ...... ea290898beeb3641869a0f21d45b48f36a7e32a687d34efab9406c7e2ec6da9fadb19a5942d84f69a426dc5859ee56ae
sha512sum ...... 7494e1580a33173c72de55de1dae679f1dc463a3f9aa874af0707315b2d01d08\
                 b1a59fdca8ab6c3dce6e48cbaeea860cebd5c62fb92c6bb43d77965e44ff10fa

The compressed file was generated with the 'xz" compression tool, using the following command line:

    $ xz -k --delta=dist=7 --lzma2=dict=64MiB,lc=4,lp=0,pb=0,mf=bt3,mode=normal,nice=273,depth=0 connect4_7x6.dat

The command line options were chosen to optimize the compression ratio, which is a rather impressive 45.287 : 1.

Compressing the file took about 2 months.

The compressed file can be decompressed by executing:

    $ unxz -k connect4_7x6.dat.xz

Decompressing the file takes on the order of 24 hours, depending, of course, on your hardware.

It is recommended to verify a hash of the uncompressed file after decompressing, e.g.

    $ sha256sum connect4_7x6.dat
    13c5edc061e7d1b1dcd7501c0b7b8018def9147895a6bd28ef1ea32930596d4f  connect4_7x6.dat

Note that, like decompressing, verifying the hash can take on the order of 24 hours.

3. Lookup table entry format
----------------------------

The "connect4_7x6.dat" file consists of 2,265,994,664,313 entries of 7 bytes each.

Each seven-byte entry contains a unique 48-bit unsigned integer representing an encoded game position in its first
six bytes, followed by a single byte representing the game-theoretical outcome of the position. The position and
outcome encodings are discussed in Section 3.1 and Section 3.2, respectively.

The file only contains positions that are actually achievable in games that start from the empty board, with players A
and B alternately moving, and the game ending immediately if (a) the last move leads to a win; or (b) the board is
completely filled without any player reaching a connect-4, resulting in a draw.

A board of connect-4 has horizontal mirror symmetry. This is used to cut the size of the lookup table (almost) in half,
as described in Section 3.1.

The entries are sorted by their 48-bit position encoding, enabling a fast (binary) search to quickly find any given
position present in the file.

3.1. Position encoding
----------------------

A given connect-4 position is encoded as follows:

For each of the 7 columns, determine c[0] to c[6], where each of the c[i] is a number in the range 0..110 that
uniquely describes the column state (see Section 3.1.1 below).

Then, calculate the following two values:

    v1 = sum(111^k * C[  k] for k in 0..6)
    v2 = sum(111^k * C[6-k] for k in 0..6)

The values v1 and v2 correspond to the value of a seven-digit base-111 number where the base-111 digits correspond
to the column state encodings. The value v1 is little-endian, whereas v2 is big endian.

Note that, for board positions that are horizontally symmetrical, i.e., C[k]==C[6-k], v1 == v2.

The encoded value of the game position is then determined as:

    v = min(v1, v2).

Effectively, we make two potential representations of a given board, one where we read the board left-to-right, and one
where we read the board right-to-left; and only the smallest of these is included in the database. This exploits the
horizontal symmetry of the game board, effectively cutting the number of game position entries that need to be stored
(almost) in half.

This value v is stored as a big-endian integer value in the first six bytes of the entry (meaning that the first byte
is the most significant, and the sixth byte is the least significant). The choice for big-endian was made since this
results in a file that can be compressed to a (slightly) smaller size.

3.1.1. Column encoding
----------------------

The standard game of connect-4 is played on a 7x6 board, so each column has 6 vertical positions. Each of these
positions is either empty, occupied by a chip of player A, or occupied by a chip of player B.

This yields 3^6 == 729 potentially possible columns. We can encode these column numbers as 6-digit ternary numbers,
with an empty position corresponding to the digit 0, an A-chip corresponding to the digit 1, and a B-chip
corresponding to the digit 2; the bottom position will correspond to the least significant ternary digit. For example,
a column with a chip played by player A, with a chip played by player B on top, would be encoded as the ternary number
000021, or the decimal value 7.

However, not all of those columns are in fact possible in legal connect-4 games, for the following two reasons:

* It is impossible for a chip to be positioned directly above an empty slot;
* Columns that have a vertical connect-4 even when the topmost chip is removed are impossible to reach, as the game
  ends immediately when a connect-4 is reached.

If we apply these two constraints to the 729 potential columns, 111 reachable columns remain. If we order them by
their ternary encoding as described above, we get the following list of 111 possible columns:

+------------------+----------------------+----------------------+----------------------+
|  encoded_decimal | column_value_decimal | column_value_ternary | column_top_to_bottom |
+------------------+----------------------+----------------------+----------------------+
|          0       |            0         |        000000        |        ......        |
|          1       |            1         |        000001        |        .....A        |
|          2       |            2         |        000002        |        .....B        |
|          3       |            4         |        000011        |        ....AA        |
|          4       |            5         |        000012        |        ....AB        |
|          5       |            7         |        000021        |        ....BA        |
|          6       |            8         |        000022        |        ....BB        |
|          7       |           13         |        000111        |        ...AAA        |
|          8       |           14         |        000112        |        ...AAB        |
|          9       |           16         |        000121        |        ...ABA        |
|         10       |           17         |        000122        |        ...ABB        |
|         11       |           22         |        000211        |        ...BAA        |
|         12       |           23         |        000212        |        ...BAB        |
|         13       |           25         |        000221        |        ...BBA        |
|         14       |           26         |        000222        |        ...BBB        |
|         15       |           40         |        001111        |        ..AAAA        |
|         16       |           41         |        001112        |        ..AAAB        |
|         17       |           43         |        001121        |        ..AABA        |
|         18       |           44         |        001122        |        ..AABB        |
|         19       |           49         |        001211        |        ..ABAA        |
|         20       |           50         |        001212        |        ..ABAB        |
|         21       |           52         |        001221        |        ..ABBA        |
|         22       |           53         |        001222        |        ..ABBB        |
|         23       |           67         |        002111        |        ..BAAA        |
|         24       |           68         |        002112        |        ..BAAB        |
|         25       |           70         |        002121        |        ..BABA        |
|         26       |           71         |        002122        |        ..BABB        |
|         27       |           76         |        002211        |        ..BBAA        |
|         28       |           77         |        002212        |        ..BBAB        |
|         29       |           79         |        002221        |        ..BBBA        |
|         30       |           80         |        002222        |        ..BBBB        |
|         31       |          122         |        011112        |        .AAAAB        |
|         32       |          124         |        011121        |        .AAABA        |
|         33       |          125         |        011122        |        .AAABB        |
|         34       |          130         |        011211        |        .AABAA        |
|         35       |          131         |        011212        |        .AABAB        |
|         36       |          133         |        011221        |        .AABBA        |
|         37       |          134         |        011222        |        .AABBB        |
|         38       |          148         |        012111        |        .ABAAA        |
|         39       |          149         |        012112        |        .ABAAB        |
|         40       |          151         |        012121        |        .ABABA        |
|         41       |          152         |        012122        |        .ABABB        |
|         42       |          157         |        012211        |        .ABBAA        |
|         43       |          158         |        012212        |        .ABBAB        |
|         44       |          160         |        012221        |        .ABBBA        |
|         45       |          203         |        021112        |        .BAAAB        |
|         46       |          205         |        021121        |        .BAABA        |
|         47       |          206         |        021122        |        .BAABB        |
|         48       |          211         |        021211        |        .BABAA        |
|         49       |          212         |        021212        |        .BABAB        |
|         50       |          214         |        021221        |        .BABBA        |
|         51       |          215         |        021222        |        .BABBB        |
|         52       |          229         |        022111        |        .BBAAA        |
|         53       |          230         |        022112        |        .BBAAB        |
|         54       |          232         |        022121        |        .BBABA        |
|         55       |          233         |        022122        |        .BBABB        |
|         56       |          238         |        022211        |        .BBBAA        |
|         57       |          239         |        022212        |        .BBBAB        |
|         58       |          241         |        022221        |        .BBBBA        |
|         59       |          367         |        111121        |        AAAABA        |
|         60       |          368         |        111122        |        AAAABB        |
|         61       |          373         |        111211        |        AAABAA        |
|         62       |          374         |        111212        |        AAABAB        |
|         63       |          376         |        111221        |        AAABBA        |
|         64       |          377         |        111222        |        AAABBB        |
|         65       |          391         |        112111        |        AABAAA        |
|         66       |          392         |        112112        |        AABAAB        |
|         67       |          394         |        112121        |        AABABA        |
|         68       |          395         |        112122        |        AABABB        |
|         69       |          400         |        112211        |        AABBAA        |
|         70       |          401         |        112212        |        AABBAB        |
|         71       |          403         |        112221        |        AABBBA        |
|         72       |          446         |        121112        |        ABAAAB        |
|         73       |          448         |        121121        |        ABAABA        |
|         74       |          449         |        121122        |        ABAABB        |
|         75       |          454         |        121211        |        ABABAA        |
|         76       |          455         |        121212        |        ABABAB        |
|         77       |          457         |        121221        |        ABABBA        |
|         78       |          458         |        121222        |        ABABBB        |
|         79       |          472         |        122111        |        ABBAAA        |
|         80       |          473         |        122112        |        ABBAAB        |
|         81       |          475         |        122121        |        ABBABA        |
|         82       |          476         |        122122        |        ABBABB        |
|         83       |          481         |        122211        |        ABBBAA        |
|         84       |          482         |        122212        |        ABBBAB        |
|         85       |          610         |        211121        |        BAAABA        |
|         86       |          611         |        211122        |        BAAABB        |
|         87       |          616         |        211211        |        BAABAA        |
|         88       |          617         |        211212        |        BAABAB        |
|         89       |          619         |        211221        |        BAABBA        |
|         90       |          620         |        211222        |        BAABBB        |
|         91       |          634         |        212111        |        BABAAA        |
|         92       |          635         |        212112        |        BABAAB        |
|         93       |          637         |        212121        |        BABABA        |
|         94       |          638         |        212122        |        BABABB        |
|         95       |          643         |        212211        |        BABBAA        |
|         96       |          644         |        212212        |        BABBAB        |
|         97       |          646         |        212221        |        BABBBA        |
|         98       |          689         |        221112        |        BBAAAB        |
|         99       |          691         |        221121        |        BBAABA        |
|        100       |          692         |        221122        |        BBAABB        |
|        101       |          697         |        221211        |        BBABAA        |
|        102       |          698         |        221212        |        BBABAB        |
|        103       |          700         |        221221        |        BBABBA        |
|        104       |          701         |        221222        |        BBABBB        |
|        105       |          715         |        222111        |        BBBAAA        |
|        106       |          716         |        222112        |        BBBAAB        |
|        107       |          718         |        222121        |        BBBABA        |
|        108       |          719         |        222122        |        BBBABB        |
|        109       |          724         |        222211        |        BBBBAA        |
|        110       |          725         |        222212        |        BBBBAB        |
+------------------+----------------------+----------------------+----------------------+

3.2. Game outcome encoding
--------------------------

The game-theoretical outcome of a game position is encoded in a single "outcome" byte (the last of seven bytes that
together form the entry for a single game position).

The game-theoretical outcome assumes optimal play by both players. The outcome, then, can be any of three cases,
that are encoded in the two most significant bits of the 'outcome" byte. The lower six bits encode the number of
half-moves until the game result is reached, assuming optimal play by both players.

* (outcome & 0xc0) == 0x00: The game is a draw. The draw will be reached in (outcome & 0x3f) half-moves.
* (outcome & 0xc0) == 0x40: The game is a win for player A. The win will be reached in (outcome & 0x3f) half-moves.
* (outcome & 0xc0) == 0x80: The game is a win for player B. The win will be reached in (outcome & 0x3f) half-moves.

Here, a "half-move" corresponds to a chip being put in the game board by either player. Since the game board has
size 7x6, the maximum number of half-moves that a game can have is 42.
