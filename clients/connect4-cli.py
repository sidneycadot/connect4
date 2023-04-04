#! /usr/bin/env -S python3 -B

"""A command-line interface for playing connect-4."""

import argparse
import re
import random

from utils.find_optimal_moves import find_optimal_moves
from utils.game_info import GameInfo
from utils.game_manager import GameManager
from utils.simple_types import Outcome, Player


def show_help(manager: GameManager) -> None:
    info = manager.info
    print("Commands:")
    print()
    print("  help, h     Print this help text.")
    print("  info, i     Show info on current board state.")
    print("  move, m     Make optimal computer move.")
    print("  1 .. {}      Make move in the selected column.".format(info.h_size))
    print("  undo, u     Undo last move.")
    print("  reset, r    Reset game to empty board.")
    print("  quit, q     Quit program.")
    print()


def unknown_command(command: str) -> None:
    print("Unknown command: {!r}. Type 'help' for help.".format(command))
    print()


def show_info(manager: GameManager) -> None:
    info = manager.info
    board = manager.current_board()
    score = info.lookup(board)
    if len(manager.moves) == 0:
        moves_so_far = "none"
    else:
        moves_so_far = ", ".join("{}: {}{}".format(i + 1, "AB"[i % 2], col + 1) for (i, col) in enumerate(manager.moves))
    print("Moves so far ....... :", moves_so_far)
    print("Current score ...... :", score.outcome.name.replace("_", " ").capitalize(), "in", score.ply, "moves.")
    print()

    if score.ply != 0:
        print("Consequences of the moves that player {} can make:".format(board.mover().name))
        print()
        for col in range(info.h_size):
            changed_board = board.execute_move(col)
            if changed_board is None:
                print(" ", col + 1, ":", "Impossible (column full).")
            else:
                score = info.lookup(changed_board)
                print(" ", col + 1, ":", score.outcome.name.replace("_", " ").capitalize(), "in", score.ply, "moves.")
        print()


def computer_move(manager: GameManager) -> None:
    board = manager.current_board()
    optimal_moves = find_optimal_moves(board)
    if len(optimal_moves) == 0:
        print("The game is finished.")
        print()
    else:
        selected_optimal_move = random.choice(optimal_moves)
        print("Optimal moves: {} ; playing in column {}.".format([col+1 for col in optimal_moves], selected_optimal_move + 1))
        print()
        manager.execute_move(selected_optimal_move)


def handle_move(manager: GameManager, col: int) -> None:
    info = manager.info
    board = manager.current_board()
    if board.outcome() != Outcome.INDETERMINATE:
        print("The game is finished.")
        print()
    elif not (0 <= col < info.h_size):
        print("The selected column is invalid.")
        print()
    elif board.get(col, info.v_size - 1) != Player.NONE:
        print("The selected column is full.")
        print()
    else:
        manager.execute_move(col)


def undo_move(manager: GameManager) -> None:
    if len(manager.moves) == 0:
        print("No move to undo.")
        print()
    else:
        manager.undo_move()


def run_cli_loop(filename: str) -> None:

    int_regexp = re.compile("0|[1-9][0-9]*")

    with GameInfo(filename) as info:

        manager = GameManager(info)
        quit_flag = False

        print("*** Connect-4 ***")
        print()
        print("Type 'help' for help, 'quit' to quit.")
        print()

        while not quit_flag:

            board = manager.current_board()
            board.print()
            print()
            outcome = board.outcome()
            if outcome in (Outcome.A_WINS, Outcome.B_WINS):
                print("Player {} has won after {} moves.".format(outcome.name[0], len(manager.moves)))
                print()
            elif outcome == Outcome.DRAW:
                print("The game is drawn after {} moves.".format(len(manager.moves)))
                print()
            else:
                mover = board.mover()
                print("Move {}; player {} to move.".format(len(manager.moves) + 1, mover.name))
                print()

            prompt = "> "
            try:
                command = input(prompt).strip()
            except:
                command = ''
            print()
            command_lowercase = command.lower()

            if command_lowercase in ('', ):
                # null command
                pass
            elif command_lowercase in ("h", "help"):
                show_help(manager)
            elif command_lowercase in ("i", "info"):
                show_info(manager)
            elif command_lowercase in ("m", "move"):
                computer_move(manager)
            elif command_lowercase in ("u", "undo"):
                undo_move(manager)
            elif command_lowercase in ("r", "reset"):
                manager.reset_game()
            elif int_regexp.fullmatch(command_lowercase):
                col = int(command_lowercase) - 1
                handle_move(manager, col)
            elif command_lowercase in ("q", "quit"):
                quit_flag = True
            else:
                unknown_command(command)


def main() -> None:

    default_database_filename = "connect4_7x6.dat"
    parser = argparse.ArgumentParser(description="Command-line interface for Connect-4.")
    parser.add_argument("-f", "--filename", default=default_database_filename, help="database filename (default: {!r})".format(default_database_filename))

    args = parser.parse_args()

    try:
        run_cli_loop(args.filename)
    except:
        # Swallow all exceptions.
        pass

    print("Bye!")
    print()


if __name__ == "__main__":
    main()
