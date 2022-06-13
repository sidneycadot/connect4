#! /usr/bin/env -S python3 -B

import sys
import functools
from typing import Optional

from PyQt5.QtCore import Qt, QAbstractTableModel, QVariant
from PyQt5.QtGui import QPalette
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, QPushButton, QTableView

from player import Player
from board import Board
from lookup_table import LookupTable

class MoveRecordModel(QAbstractTableModel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.move_record = []

    def rowCount(self, parent):
        return (len(self.move_record) + 1) // 2

    def columnCount(self, parent):
        return 2

    def data(self, index, role):
        row = index.row()
        col = index.column()
        move_index = row * 2 + col
        if role == Qt.DisplayRole:
            if move_index < len(self.move_record):
                return QVariant(str(self.move_record[move_index] + 1))
        elif role == Qt.TextAlignmentRole:
            return QVariant(Qt.AlignCenter)
        return QVariant()

    def headerData(self, section: int, orientation, role):
        if orientation == Qt.Horizontal:
            if role == Qt.DisplayRole:
                if section == 0:
                    return QVariant("red")
                if section == 1:
                    return QVariant("yellow")
        elif orientation == Qt.Vertical:
            if role == Qt.DisplayRole:
                return QVariant("move {}:".format(section + 1))

        return QVariant()

    def append(self, col: int) -> None:
        self.beginResetModel()
        self.move_record.append(col)
        self.endResetModel()

    def reset(self) -> None:
        self.beginResetModel()
        self.move_record.clear()
        self.endResetModel()

    def takeback(self) -> Optional[int]:
        if len(self.move_record) == 0:
            return None
        self.beginResetModel()
        col = self.move_record.pop()
        self.endResetModel()
        return col


class Connect4Widget(QWidget):

    PLAYER_NONE_COLOR = Qt.gray
    PLAYER_A_COLOR    = Qt.red
    PLAYER_B_COLOR    = Qt.yellow

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.move_record_model = MoveRecordModel()

        grid_layout = QGridLayout()

        self.rows = []
        for y in range(Board.V_SIZE):
            row = []
            for x in range(Board.H_SIZE):
                label = QLabel("●")
                font = label.font()
                font.setPointSize(60)
                label.setFont(font)
                palette = label.palette()
                palette.setColor(QPalette.ColorRole.Foreground, self.PLAYER_NONE_COLOR)
                label.setPalette(palette)
                label.setAlignment(Qt.AlignCenter)
                grid_layout.addWidget(label, y, x)
                row.append(label)
            self.rows.append(row)

        self.drop_labels = []

        for x in range(Board.H_SIZE):
            label = QLabel()
            label.setAlignment(Qt.AlignCenter)
            font = label.font()
            font.setBold(True)
            label.setFont(font)
            grid_layout.addWidget(label, Board.V_SIZE, x)
            self.drop_labels.append(label)

        self.drop_buttons = []

        for x in range(Board.H_SIZE):
            button = QPushButton("drop")
            button.clicked.connect(functools.partial(self.drop, x))
            grid_layout.addWidget(button, Board.V_SIZE + 1, x)
            self.drop_buttons.append(button)

        left_layout = QVBoxLayout()

        self.status_widget = QLabel()
        self.status_widget.setAlignment(Qt.AlignCenter)
        font = self.status_widget.font()
        font.setBold(True)
        self.status_widget.setFont(font)

        left_layout.addWidget(self.status_widget)
        left_layout.addLayout(grid_layout)


        self.move_record_widget = QTableView()
        self.move_record_widget.setModel(self.move_record_model)

        reset_button = QPushButton("Reset")
        reset_button.clicked.connect(self.reset)

        takeback_button = QPushButton("Takeback")
        takeback_button.clicked.connect(self.takeback)

        button_layout = QHBoxLayout()
        button_layout.addStretch()
        button_layout.addWidget(reset_button)
        button_layout.addStretch()
        button_layout.addWidget(takeback_button)
        button_layout.addStretch()

        right_layout = QVBoxLayout()
        right_layout.addWidget(self.move_record_widget)
        right_layout.addLayout(button_layout)

        h_layout = QHBoxLayout()
        h_layout.addLayout(left_layout)
        h_layout.addLayout(right_layout)

        self.setLayout(h_layout)

        self.update_gui()

    def get_field(self, i, j) -> Player:
        label = self.rows[i][j]
        palette = label.palette()
        color = palette.color(QPalette.ColorRole.Foreground)
        if color == self.PLAYER_NONE_COLOR:
            return Player.NONE
        if color == self.PLAYER_A_COLOR:
            return Player.A
        if color == self.PLAYER_B_COLOR:
            return Player.B
        raise RuntimeError("Bad entry.")


    def set_field(self, i, j, player) -> None:
        label = self.rows[i][j]
        palette = label.palette()
        if player == Player.NONE:
            palette.setColor(QPalette.ColorRole.Foreground, self.PLAYER_NONE_COLOR)
        elif player == Player.A:
            palette.setColor(QPalette.ColorRole.Foreground, self.PLAYER_A_COLOR)
        elif player == Player.B:
            palette.setColor(QPalette.ColorRole.Foreground, self.PLAYER_B_COLOR)
        label.setPalette(palette)

    def reset(self) -> None:
        for i in range(Board.V_SIZE):
            for j in range(Board.H_SIZE):
                self.set_field(i, j, Player.NONE)
        self.move_record_model.reset()
        self.update_gui()

    def takeback(self) -> None:
        col = self.move_record_model.takeback()
        if col is not None:
            for row in range(Board.V_SIZE):
                if self.get_field(row, col) != Player.NONE:
                    self.set_field(row, col, Player.NONE)
                    self.update_gui()
                    break

    def drop(self, col: int) -> None:
        board = self.make_board()
        mover = board.mover()
        for row in reversed(range(Board.V_SIZE)):
            if self.get_field(row, col) == Player.NONE:
                self.set_field(row, col, mover)
                self.move_record_model.append(col)
                self.update_gui()
                break

    def make_board(self) -> Board:
        entries = []
        for i in range(Board.V_SIZE - 1, -1, -1):
            for j in range(Board.H_SIZE):
                entries.append(self.get_field(i, j))

        return Board(entries)

    def update_gui(self) -> None:

        board = self.make_board()
        #board.print()

        app = QApplication.instance()

        mover = board.mover()

        status_messages = []

        evaluation = app.lookup(board)

        moves_available = False
        for col in range(Board.H_SIZE):
            drop_board = board.drop(col)
            if drop_board is None:
                self.drop_buttons[col].setEnabled(False)
                self.drop_labels[col].setText("n/a")
            else:
                moves_available = True
                self.drop_buttons[col].setEnabled(True)
                drop_board_evaluation = app.lookup(drop_board)
                if drop_board_evaluation.player == Player.NONE:
                    self.drop_labels[col].setText("draw\n(-)")
                if drop_board_evaluation.player == Player.A:
                    self.drop_labels[col].setText("red\n({})".format(drop_board_evaluation.winply))
                if drop_board_evaluation.player == Player.B:
                    self.drop_labels[col].setText("yellow\n({})".format(drop_board_evaluation.winply))

        if moves_available:
            if mover == Player.A:
                status_messages.append("red has the move")
            else:
                status_messages.append("yellow has the move")
        else:
            status_messages.append("game over")

        if evaluation.player == Player.NONE:
            status_messages.append("the game is a draw")
        elif evaluation.player == Player.A:
            if evaluation.winply == 0:
                status_messages.append("red wins")
            else:
                status_messages.append("red has a {}-ply win".format(evaluation.winply))
        elif evaluation.player == Player.B:
            if evaluation.winply == 0:
                status_messages.append("yellow wins")
            else:
                status_messages.append("yellow has a {}-ply win".format(evaluation.winply))

        status_message = "; ".join(status_messages)

        self.status_widget.setText(status_message)

class MyMainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setWindowTitle("Connect-4")
        connect4_widget = Connect4Widget()
        self.setCentralWidget(connect4_widget)


class MyApplication(QApplication):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._lookup_table = LookupTable("connect4_6x5.bin", 6)
        self._lookup_table.open()

        self.main_window = MyMainWindow()
        self.main_window.show()

        self.aboutToQuit.connect(self.done)

    def done(self):
        self._lookup_table.close()

    def lookup(self, board: Board):
        return self._lookup_table.lookup(board)


def main():
    app = MyApplication(sys.argv)
    exitcode = app.exec_()
    if exitcode != 0:
        sys.exit(exitcode)


if __name__ == "__main__":
    main()
