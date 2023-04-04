"""Find optimal moves."""

from typing import List

from .board import Board
from .simple_types import Player, Outcome


def find_optimal_moves(board: Board) -> List[int]:
    """Find optimal moves for the given board."""

    info = board.info

    mover = board.mover()

    optimal_score = None
    optimal_moves = []

    for col in range(info.h_size):

        changed_board = board.execute_move(col)
        if changed_board is None:
            continue

        changed_board_score = info.lookup(changed_board)

        if len(optimal_moves) == 0:
            """This is the first valid move; it's always optimal."""
            optimal_score = changed_board_score
            optimal_moves.append(col)
            continue

        # We need to compare the score to the current optimal_score.

        if (mover == Player.A and changed_board_score.outcome == Outcome.A_WINS) or (mover == Player.B and changed_board_score.outcome == Outcome.B_WINS):
            # This move leads to a win with optimal play by both sides.
            if optimal_score.outcome != changed_board_score.outcome:
                # The current-best score is not winning, so we replace it.
                optimal_score = changed_board_score
                optimal_moves = [col]
            elif changed_board_score.ply < optimal_score.ply:
                # The current-best score is winning, but it takes more moves; so we replace it.
                optimal_score = changed_board_score
                optimal_moves = [col]
            elif changed_board_score.ply == optimal_score.ply:
                # The current-best score is winning, in the same number of moves; so we append the current move.
                optimal_moves.append(col)
            else:
                # The current-best score is winning in less moves.
                pass
        elif changed_board_score.outcome == Outcome.DRAW:
            # This move leads to a draw with optimal play by both sides.
            if (mover == Player.A and optimal_score.outcome == Outcome.A_WINS) or (mover == Player.B and optimal_score.outcome == Outcome.B_WINS):
                # There is already a winning move; so this move is not an improvement.
                pass
            elif optimal_score.outcome == Outcome.DRAW:
                # The current best move is a draw, as is the current move.
                if changed_board_score.ply > optimal_score.ply:
                    # Slower draw is better (give opponent more opportunity to make mistakes).
                    optimal_score = changed_board_score
                    optimal_moves = [col]
                elif changed_board_score.ply == optimal_score.ply:
                    # Equal-length draw. Add this move.
                    optimal_moves.append(col)
                else:
                    # No improvement.
                    pass
            else:
                # The current best move is losing; replace it with the current draw move.
                optimal_score = changed_board_score
                optimal_moves = [col]
        else:
            # This move leads to a loss with optimal play by both sides.
            if (mover == Player.A and optimal_score.outcome == Outcome.B_WINS) or (mover == Player.B and optimal_score.outcome == Outcome.A_WINS):
                # The best move found so far also loses.
                if changed_board_score.ply > optimal_score.ply:
                    # Slower loss is better (give opponent more opportunity to make mistakes).
                    optimal_score = changed_board_score
                    optimal_moves = [col]
                elif changed_board_score.ply == optimal_score.ply:
                    # Equal-length loss. Add this move.
                    optimal_moves.append(col)
                else:
                    # No improvement.
                    pass

    return optimal_moves
