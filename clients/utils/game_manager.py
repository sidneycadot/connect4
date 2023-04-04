from utils.board import Board
from utils.game_info import GameInfo
from utils.simple_types import Player


class GameManager:

    def __init__(self, info: GameInfo):

        self.info = info

        none_entries = [Player.NONE for i in range(info.h_size * info.v_size)]
        empty_board = Board(info, none_entries)

        self.boards = [empty_board]
        self.moves = []

    def reset_game(self):
        self.boards = self.boards[:1]
        self.moves.clear()

    def execute_move(self, col: int):
        changed_board = self.boards[-1].execute_move(col)
        if changed_board is None:
            raise RuntimeError("Bad move.")
        self.boards.append(changed_board)
        self.moves.append(col)

    def undo_move(self):
        if len(self.moves) == 0:
            raise RuntimeError("Cannot undo move.")
        self.boards.pop()
        self.moves.pop()

    def current_board(self):
        return self.boards[-1]
