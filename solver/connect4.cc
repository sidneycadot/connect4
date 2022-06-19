
/////////////////
// connect4.cc //
/////////////////

#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "base62.h"
#include "player.h"
#include "score.h"
#include "board_size.h"
#include "board.h"
#include "files.h"

using namespace std;

static void write_node_with_trivial_score(ostream & out_stream, const Board & board)
{
    // During the inital and forward steps, we mark boards that we can determine by immediate
    // inspection as won-in-0 (i.e., one of the players has a four-in-a-row): "A0" or "B0",
    // or draw in zero: "-0".
    // If we cannot determine the node evaluation, we write "unknown" which is marked as "?0".

    out_stream << board << board.score() << '\n';
}

static void make_initial_node(const string & out_filename)
{
    // Write a file with the single initial empty board state.

    OutputFile out_file(out_filename);

    ostream & out = out_file.get_ostream_reference();

    const Board initial = Board::empty();
    write_node_with_trivial_score(out, initial);
}

static void make_nodes(const string & in_nodes_filename,
                       const string & out_nodes_filename)
{
    // Given an input file of nodes, write a file with the possible nodes that
    // can be reached by starting at a node found in the input file, and making
    // a single move.
    //
    // The output is unsorted and may contain duplicates;
    // it should therefore be piped through 'sort -u'.

    const InputFile  in_nodes_file(in_nodes_filename);
    const OutputFile out_nodes_file(out_nodes_filename);

    istream & in_nodes  = in_nodes_file.get_istream_reference();
    ostream & out_nodes = out_nodes_file.get_ostream_reference();

    Board board;
    Score score;

    while (in_nodes >> board >> score)
    {
        const set<Board> unique_boards = board.generate_unique_normalized_boards();

        for (const Board & unique_board: unique_boards)
        {
            write_node_with_trivial_score(out_nodes, unique_board);
        }
    }
}

static void make_edges(const string & in_nodes_filename,
                       const string & out_edges_filename)
{
    // Given an input file of nodes, write a file with the possible edges that
    // can be traversed by starting at a node found in the input file, and
    // making 1 move.
    //
    // The edges are writted starting with the destination node followed
    // by the source node. This is done so that a sorted version of the
    // output can be used to determine the value of each edge, by combining
    // the file with a file containing node evaluations of the destination node.
    //
    // The output is unsorted but will not contain duplicates;
    // it should therefore be piped through 'sort'.

    const InputFile  in_nodes_file(in_nodes_filename);
    const OutputFile out_edges_file(out_edges_filename);

    istream & in_nodes  = in_nodes_file.get_istream_reference();
    ostream & out_edges = out_edges_file.get_ostream_reference();

    Board board;
    Score score;

    while (in_nodes >> board >> score)
    {
        const set<Board> unique_boards = board.generate_unique_normalized_boards();

        for (const Board & unique_board: unique_boards)
        {
            // Write destination, followed by source.
            out_edges << unique_board << board << '\n';
        }
    }
}

static void make_edges_with_score(const string & in_edges_filename,
                                  const string & in_nodes_with_score_filename,
                                  const string & out_edges_with_score_filename)
{
    // We iterate over sorted (destination, source) edges. For each edge, we find
    // the matching destination node and its evaluation in the provided
    // 'nodes_with_score' file.
    //
    // We output the edge's source node and the evaluation of its outgoing edge.
    //
    // The output is unsorted and may contain duplicates;
    // it should therefore be piped through 'sort -u'.

    const InputFile  in_edges_file(in_edges_filename);
    const InputFile  in_nodes_with_score_file(in_nodes_with_score_filename);
    const OutputFile out_edges_with_score_file(out_edges_with_score_filename);

    istream & in_edges             = in_edges_file.get_istream_reference();
    istream & in_nodes_with_score  = in_nodes_with_score_file.get_istream_reference();
    ostream & out_edges_with_score = out_edges_with_score_file.get_ostream_reference();

    string edge_src, edge_dst;

    string board_string;
    Score score;

    while (in_edges >> setw(NUM_BASE62_BOARD_DIGITS) >> edge_dst >> setw(NUM_BASE62_BOARD_DIGITS) >> edge_src)
    {
        if (edge_dst != board_string)
        {
            in_nodes_with_score >> setw(NUM_BASE62_BOARD_DIGITS) >> board_string >> score;
            if (!in_nodes_with_score)
            {
                throw runtime_error("make_edges_with_score: bad read.");
            }

            if (edge_dst != board_string)
            {
                throw runtime_error("make_edges_with_score: didn't find score node we expected.");
            }
        }
        out_edges_with_score << edge_src << score << '\n';
    }
}

static void make_nodes_with_score(const string & in_nodes_filename,
                                  const string & in_edges_with_score_filename,
                                  const string & out_nodes_with_score_filename)
{
    // We read 'unscored' nodes (i.e., nodes for which an evaluation is not yet available),
    // and will annotate them with evaluations, based on the 'edges_with_score' input.

    const InputFile  in_nodes_file(in_nodes_filename);
    const InputFile  in_edges_with_score_file(in_edges_with_score_filename);
    const OutputFile out_nodes_with_score_file(out_nodes_with_score_filename);

    istream & in_nodes             = in_nodes_file.get_istream_reference();
    istream & in_edges_with_score  = in_edges_with_score_file.get_istream_reference();
    ostream & out_nodes_with_score = out_nodes_with_score_file.get_ostream_reference();

    string node_board_string;
    Score  node_score;

    bool   edge_score_valid = false; // Are the values below valid (i.e., read from the input, but yet unused)?
    string edge_score_board_string;
    Score  edge_score;

    while (in_nodes >> setw(NUM_BASE62_BOARD_DIGITS) >> node_board_string >> node_score)
    {
        if (node_score.outcome != Outcome::INDETERMINATE)
        {
            // The node has a determined result. No outgoing edges need to be checked, we can just write the result.
            out_nodes_with_score << node_board_string << node_score << '\n';
        }
        else
        {
            // The current node does not have a result yet; we need to determine its result evaluation by examining
            // its outgoing edges and their results as available in the 'in_edges_with_score' input stream.
            // For each indeterminate-result node, at least one such entry will be available.

            const Board board = Board::from_base62_string(node_board_string);
            const Player node_mover = board.mover();

            bool node_mover_has_draw = false;
            bool node_mover_has_win  = false;
            bool node_mover_has_loss = false;

            unsigned node_mover_min_win  = 0;
            unsigned node_mover_max_loss = 0;
            unsigned node_mover_any_draw = 0;

            while (true)
            {
                if (!edge_score_valid)
                {
                    if (in_edges_with_score >> setw(NUM_BASE62_BOARD_DIGITS) >> edge_score_board_string >> edge_score)
                    {
                        edge_score_valid = true;
                    }
                }

                // If 'edge_score_valid' is false here, we have reached the end of the 'in_edges_with_score' input stream.

                if (edge_score_valid && edge_score_board_string == node_board_string)
                {
                    // We have a valid edge score, and it does contain information relevant to the current board (node).
                    // Update the 'node_mover_*' variables with the new information provided by this edge.

                    if (edge_score.outcome == Outcome::DRAW)
                    {
                        if (node_mover_has_draw)
                        {
                            if (edge_score.ply + 1 != node_mover_any_draw)
                            {
                                throw runtime_error("multiple draw-in-n values encountered");
                            }
                        }
                        else
                        {
                            node_mover_has_draw = true;
                            node_mover_any_draw = edge_score.ply + 1;
                        }
                    }
                    else if ((node_mover == Player::A && edge_score.outcome == Outcome::A_WINS) || (node_mover == Player::B && edge_score.outcome == Outcome::B_WINS))
                    {
                        const unsigned win_ply = edge_score.ply + 1;

                        if (node_mover_has_win)
                        {
                            if (win_ply < node_mover_min_win)
                            {
                                node_mover_min_win = win_ply;
                            }
                        }
                        else
                        {
                            node_mover_has_win = true;
                            node_mover_min_win = win_ply;
                        }
                    }
                    else // The mover loses
                    {
                        const unsigned lose_ply = edge_score.ply + 1;

                        if (node_mover_has_loss)
                        {
                            if (lose_ply > node_mover_max_loss)
                            {
                                node_mover_max_loss = lose_ply;
                            }
                        }
                        else
                        {
                            node_mover_has_loss = true;
                            node_mover_max_loss = lose_ply;
                        }
                    }
                    edge_score_valid = false; // Invalidate current score; we used it.
                }
                else
                {
                    // Either we reached the end of the 'in_edges_with_score' stream, or the edge just read for is is not
                    // relevant for the currently processed node (board). In either case, we can now calculate the evaluation
                    // for the current node.

                    if (node_mover_has_win)
                    {
                        // The mover's best move is a win.
                        const Score score = Score(node_mover == Player::A ? Outcome::A_WINS : Outcome::B_WINS, node_mover_min_win);
                        out_nodes_with_score << node_board_string << score << '\n';
                    }
                    else if (node_mover_has_draw)
                    {
                        // The mover's best move is a draw.
                        const Score score = Score(Outcome::DRAW, node_mover_any_draw);
                        out_nodes_with_score << node_board_string << score << '\n';
                    }
                    else if (node_mover_has_loss)
                    {
                        // The mover's best move is a loss.
                        const Score score = Score(node_mover == Player::A ? Outcome::B_WINS : Outcome::A_WINS, node_mover_max_loss);
                        out_nodes_with_score << node_board_string << score << '\n';
                    }
                    else
                    {
                        throw runtime_error("we should have a win, draw, or loss.");
                    }

                    break; // Proceed to the next board.
                }
            } // while loop for reading edges for the current node (board).
        } // Visiting a node that has no trivial winner. Determine win/lose/draw state for the node.
    } // Walk the nodes.
}

static void make_binary_file(const string & in_nodes_filename,
                             const string & out_nodes_filename)
{
    const InputFile  in_nodes_file(in_nodes_filename);
    const OutputFile out_nodes_file(out_nodes_filename);

    istream & in_nodes  = in_nodes_file.get_istream_reference();
    ostream & out_nodes = out_nodes_file.get_ostream_reference();

    Board board;
    Score score;

    while (in_nodes >> board >> score)
    {
        uint8_t octets[NUM_BASE256_BOARD_DIGITS + 1];
        uint64_t n = board.to_uint64();

        // Insert the Board's unsigned int value in big-endian order.
        // We write using big-endian rather than little-endian order because it results in a
        // file that can be compressed to a significantly smaller size.

        for (unsigned i = 0; i < NUM_BASE256_BOARD_DIGITS; ++i)
        {
             octets[NUM_BASE256_BOARD_DIGITS - 1 - i] = n & 255;
             n >>= 8;
        }

        if (n != 0)
        {
            throw runtime_error("make_binary_file: unable to write board in NUM_BASE256_BOARD_DIGITS bytes.");
        }

        if (score.outcome == Outcome::INDETERMINATE)
        {
            throw runtime_error("unexpected indeterminate score.");
        }

        octets[NUM_BASE256_BOARD_DIGITS] = score.to_uint8();

        out_nodes.write(reinterpret_cast<char *>(octets), NUM_BASE256_BOARD_DIGITS + 1);
    }
}


static void upgrade_binary_file(const string & in_nodes_filename,
                                const string & out_nodes_filename)
{
    // This function converts an old-style binary file to a new-style binary file.

    const InputFile  in_nodes_file(in_nodes_filename);
    const OutputFile out_nodes_file(out_nodes_filename);

    istream & in_nodes  = in_nodes_file.get_istream_reference();
    ostream & out_nodes = out_nodes_file.get_ostream_reference();

    uint8_t octets[NUM_BASE256_BOARD_DIGITS + 1];

    while (in_nodes.read(reinterpret_cast<char *>(octets), NUM_BASE256_BOARD_DIGITS + 1))
    {
        uint64_t n = 0;
        for (unsigned i = 0; i < NUM_BASE256_BOARD_DIGITS; ++i)
        {
            n *= 256;
            n += octets[i];
        }

        Board board = Board::from_uint64(n);

        uint8_t old_style_score = octets[NUM_BASE256_BOARD_DIGITS];
        Score new_style_score;

        if (old_style_score > 0 && old_style_score <= 127)
        {
            // Player A wins in (old_style_score - 1) moves.
            new_style_score = Score(Outcome::A_WINS, old_style_score - 1);
        }
        else if (old_style_score >= 128)
        {
            // Player B wins in (255 - old_style_score) moves.
            new_style_score = Score(Outcome::B_WINS, 255 - old_style_score);
        }
        else
        {
            new_style_score = Score(Outcome::DRAW, V_SIZE * H_SIZE - board.count());
        }

        octets[NUM_BASE256_BOARD_DIGITS] = new_style_score.to_uint8();

        out_nodes.write(reinterpret_cast<char *>(octets), NUM_BASE256_BOARD_DIGITS + 1);
    }
}

static void print_constants()
{
    cout << "H_SIZE=" << H_SIZE << endl;
    cout << "V_SIZE=" << V_SIZE << endl;
    cout << "Q=" << Q << endl;
    cout << "NUMBER_OF_POSSIBLE_COLUMNS=" << NUMBER_OF_POSSIBLE_COLUMNS << endl;
    cout << "NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION=" << NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION << endl;
    cout << "NUM_BASE62_BOARD_DIGITS=" << NUM_BASE62_BOARD_DIGITS << endl;
    cout << "NUM_BASE256_BOARD_DIGITS=" << NUM_BASE256_BOARD_DIGITS << endl;
}

static void print_usage()
{
    cerr                                                                                                                                     << endl;
    cerr << "Usage: connect4 --MODE <filename> [<filename>...]"                                                                              << endl;
    cerr                                                                                                                                     << endl;
    cerr << "The following file-processing modes are available:"                                                                             << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    connect4 --make-initial-node                                                            <out:nodes-without-score(0)>"       << endl;
    cerr << "    connect4 --make-nodes            <in:nodes-without-score(n)>                            <out:nodes-without-score(n+1)>"     << endl;
    cerr << "    connect4 --make-edges            <in:nodes-without-score(n)>                            <out:edges-without-score(n)>"       << endl;
    cerr << "    connect4 --make-edges-with-score <in:edges-without-score(n)> <in:nodes-with-score(n+1)> <out:edges-with-score(n)>"          << endl;
    cerr << "    connect4 --make-nodes-with-score <in:nodes-without-score(n)> <in:edges-with-score(n)>   <out:nodes-with-score(n)>"          << endl;
    cerr << "    connect4 --make-binary-file      <in:nodes-file>                                        <out:nodes-file-binary>"            << endl;
    cerr << "    connect4 --upgrade-binary-file   <in:nodes-file-binary-old>                             <out:nodes-file-binary-new>"        << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    Note: "                                                                                                                     << endl;
    cerr                                                                                                                                     << endl;
    cerr << "       If an input filename is given as '"  << InputFile::stdin_name   << "', the program reads from stdin instead of a file."  << endl;
    cerr << "       If an output filename is given as '" << OutputFile::stdout_name << "', the program writes to stdout instead of a file."  << endl;
    cerr                                                                                                                                     << endl;
    cerr << "Compile-time constant can be printed as follows:"                                                                               << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    connect4 --print-constants"                                                                                                 << endl;
    cerr                                                                                                                                     << endl;
}

int main(int argc, char **argv)
{
    // Copy command-line arguments into a string vector.

    const vector<string> args(argv + 1, argv + argc);

    // The first command line argument should be the desired operation, and will be followed
    // by one or more arguments indicating filenames to use for input and/or output.

    if (args.size() == 2 && args[0] == "--make-initial-node")
    {
        make_initial_node(args[1]);
    }
    else if (args.size() == 3 && args[0] == "--make-nodes")
    {
        make_nodes(args[1], args[2]);
    }
    else if (args.size() == 3 && args[0] == "--make-edges")
    {
        make_edges(args[1], args[2]);
    }
    else if (args.size() == 4 && args[0] == "--make-edges-with-score")
    {
        make_edges_with_score(args[1], args[2], args[3]);
    }
    else if (args.size() == 4 && args[0] == "--make-nodes-with-score")
    {
        make_nodes_with_score(args[1], args[2], args[3]);
    }
    else if (args.size() == 3 && args[0] == "--make-binary-file")
    {
        make_binary_file(args[1], args[2]);
    }
    else if (args.size() == 3 && args[0] == "--upgrade-binary-file")
    {
        // Change from old-style score to new-style score.
        upgrade_binary_file(args[1], args[2]);
    }
    else if (args.size() == 1 && args[0] == "--print-constants")
    {
        print_constants();
    }
    else
    {
        // None of the command specifications tried above matches.
        // Print usage information instead.
        print_usage();
    }

    return 0;
}
