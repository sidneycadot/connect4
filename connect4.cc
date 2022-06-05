
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
#include "constants.h"
#include "board.h"
#include "unique_boards.h"
#include "files.h"

using namespace std;

static void write_node_with_trivial_evaluation(ostream & out_stream, const Board & board)
{
    // During the inital and forward steps, we mark boards that we can determine by immediate
    // inspection as won-in-0 (i.e., one of the players has a four-in-a-row): "A0" or "B0".
    // If we cannot determine the node evaluation, we write "unknown" which is marked as ".0".
    out_stream << board << board.winner() << "0\n";
}

static void make_initial_node(const string & out_filename)
{
    // Write a file with the single initial empty board state.

    OutputFile out_file(out_filename);

    ostream & out = out_file.get_ostream_reference();

    const Board initial = Board::empty();
    write_node_with_trivial_evaluation(out, initial);
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

    Board  board;
    Player winner;
    char   winply_encoded;

    while (in_nodes >> board >> winner >> winply_encoded)
    {
        const set<Board> unique_boards = generate_unique_boards(board);

        for (const Board & unique_board: unique_boards)
        {
            write_node_with_trivial_evaluation(out_nodes, unique_board);
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

    Board  board;
    Player winner;
    char   winply_encoded;

    while (in_nodes >> board >> winner >> winply_encoded)
    {
        const set<Board> unique_boards = generate_unique_boards(board);

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

    string score_board;
    Player score_winner;
    char   score_winply_encoded;

    while (in_edges >> setw(NUM_BASE62_DIGITS) >> edge_dst >> setw(NUM_BASE62_DIGITS) >> edge_src)
    {
        if (edge_dst != score_board)
        {
            in_nodes_with_score >> setw(NUM_BASE62_DIGITS) >> score_board >> score_winner >> score_winply_encoded;
            if (!in_nodes_with_score)
            {
                throw runtime_error("make_edges_with_score: bad read.");
            }

            if (edge_dst != score_board)
            {
                throw runtime_error("make_edges_with_score: didn't find score node we expected.");
            }
        }
        out_edges_with_score << edge_src << score_winner << score_winply_encoded << '\n';
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

    string node_board_encoded;
    Player node_winner;
    char   node_winply_encoded;

    bool   edge_score_valid = false; // Are the values below valid (i.e., read from the inpout, but yet unused)?
    string edge_score_board_encoded;
    Player edge_score_winner;
    char   edge_score_winply_encoded;

    while (in_nodes >> setw(NUM_BASE62_DIGITS) >> node_board_encoded >> node_winner >> node_winply_encoded)
    {
        if (node_winner != Player::NONE)
        {
            // The node has a trivial winner. No outgoing edges need to be checked, we can just write the result.
            out_nodes_with_score << node_board_encoded << node_winner << node_winply_encoded << '\n';
        }
        else
        {
            // The current node does not have a trivial winner; we need to determine its win/lose/draw evaluation by examining
            // its outgoing edges and their scores as available in the 'in_edges_with_score' input stream.
            // For each non-trivial node, at least one such entry will be available.

            const Board board = Board::from_string(node_board_encoded);
            const Player node_mover = board.mover();

            bool node_mover_has_draw = false;
            bool node_mover_has_win  = false;
            bool node_mover_has_loss = false;

            unsigned node_mover_min_win  = 0;
            unsigned node_mover_max_loss = 0;

            while (true)
            {
                if (!edge_score_valid)
                {
                    if (in_edges_with_score >> setw(NUM_BASE62_DIGITS) >> edge_score_board_encoded >> edge_score_winner >> edge_score_winply_encoded)
                    {
                        edge_score_valid = true;
                    }
                }

                // If 'edge_score_valid' is false here, we have reached the end of the 'in_edges_with_score' input stream.

                if (edge_score_valid && edge_score_board_encoded == node_board_encoded)
                {
                    // We have a valid edge score, and it does contain information relevant to the current board (node).
                    // Update the 'node_mover_*' variables with the new information provided by this edge.

                    if (edge_score_winner == Player::NONE)
                    {
                        node_mover_has_draw = true;
                    }
                    else if (edge_score_winner == node_mover)
                    {
                        const unsigned score_winply = from_base62_digit(edge_score_winply_encoded) + 1;

                        if (node_mover_has_win)
                        {
                            if (score_winply < node_mover_min_win)
                            {
                                node_mover_min_win = score_winply;
                            }
                        }
                        else
                        {
                            node_mover_has_win = true;
                            node_mover_min_win = score_winply;
                        }
                    }
                    else // we lose
                    {
                        const unsigned score_winply = from_base62_digit(edge_score_winply_encoded) + 1;

                        if (node_mover_has_loss)
                        {
                            if (score_winply > node_mover_max_loss)
                            {
                                node_mover_max_loss = score_winply;
                            }
                        }
                        else
                        {
                            node_mover_has_loss = true;
                            node_mover_max_loss = score_winply;
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
                        // We have a win!
                        out_nodes_with_score << node_board_encoded << node_mover << to_base62_digit(node_mover_min_win) << '\n';
                    }
                    else if (node_mover_has_draw || !node_mover_has_loss)
                    {
                        // We have a draw, or nothing.
                        out_nodes_with_score << node_board_encoded << Player::NONE << "0\n";
                    }
                    else
                    {
                        // We have only losing moves (at least 1).
                        const Player node_other_player = (node_mover == Player::A) ? Player::B : Player::A;
                        out_nodes_with_score << node_board_encoded << node_other_player << to_base62_digit(node_mover_max_loss) << '\n';
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

    Board  board;
    Player winner;
    char   winply_encoded;

    while (in_nodes >> board >> winner >> winply_encoded)
    {
        uint8_t octets[NUM_OCTETS + 1];
        uint64_t n = board.to_uint64();

        // Insert the Board's unsigned int value in big-endian order.
        // We write using big-endian order because it leads to a file that
        // can be compressed to a significantly smaller size.

        for (unsigned i = 0; i < NUM_OCTETS; ++i)
        {
             octets[NUM_OCTETS - 1 - i] = n & 255;
             n >>= 8;
        }

        if (n != 0)
        {
            throw runtime_error("make_binary_file: unable to write board in NUM_OCTETS bytes.");
        }

        // Insert the board evaluation as a single octet.
        const unsigned winply = from_base62_digit(winply_encoded);

        octets[NUM_OCTETS] = (winner == Player::A) ? 1 + winply : (winner == Player::B) ? 255 - winply : 0;

        out_nodes.write(reinterpret_cast<char *>(octets), NUM_OCTETS + 1);
    }
}

static void print_usage()
{
    cerr                                                                                                                                     << endl;
    cerr << "Usage: connect4 --MODE <filename> [<filename>...]"                                                                              << endl;
    cerr                                                                                                                                     << endl;
    cerr << "The program can be run in the following file-processing modes:"                                                                 << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    connect4 --make-initial-node                                                            <out:nodes-without-score(0)>"       << endl;
    cerr << "    connect4 --make-nodes            <in:nodes-without-score(n)>                            <out:nodes-without-score(n+1)>"     << endl;
    cerr << "    connect4 --make-edges            <in:nodes-without-score(n)>                            <out:edges-without-score(n)>"       << endl;
    cerr << "    connect4 --make-edges-with-score <in:edges-without-score(n)> <in:nodes-with-score(n+1)> <out:edges-with-score(n)>"          << endl;
    cerr << "    connect4 --make-nodes-with-score <in:nodes-without-score(n)> <in:edges-with-score(n)>   <out:nodes-with-score(n)>"          << endl;
    cerr << "    connect4 --make-binary-file      <in:nodes-file>                                        <out:nodes-file-binary>"            << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    Note: "                                                                                                                     << endl;
    cerr                                                                                                                                     << endl;
    cerr << "       If an input filename is given as '"  << InputFile::stdin_name   << "', the program reads from stdin instead of a file."  << endl;
    cerr << "       If an output filename is given as '" << OutputFile::stdout_name << "', the program writes to stdout instead of a file."  << endl;
    cerr                                                                                                                                     << endl;
    cerr << "Two miscellaneous modes show the board dimensions; this is useful for shell scripts:"                                           << endl;
    cerr                                                                                                                                     << endl;
    cerr << "    connect4 --horizontal-board-size"                                                                                           << endl;
    cerr << "    connect4 --vertical-board-size"                                                                                             << endl;
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
    else if (args.size() == 1 && args[0] == "--horizontal-board-size")
    {
        cout << H_SIZE << endl;
    }
    else if (args.size() == 1 && args[0] == "--vertical-board-size")
    {
        cout << V_SIZE << endl;
    }
    else
    {
        // None of the command specifications tried above matches.
        // Print usage information instead.
        print_usage();
    }

    return 0;
}
