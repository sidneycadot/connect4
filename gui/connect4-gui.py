#! /usr/bin/env -S python3 -B

import sys
import functools
from typing import Optional

from PyQt5.QtCore import Qt, QAbstractTableModel, QVariant
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, QPushButton, QTableView, QCheckBox

from simple_types import Player, Outcome
from board import Board
from lookup_table import LookupTable


class MoveRecordModel(QAbstractTableModel):
    """A table model for the move list."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.move_record = []

    def __len__(self):
        return len(self.move_record)

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
    """A widget for playing connect-4."""

    PLAYER_NONE_COLOR = Qt.white
    PLAYER_A_COLOR    = Qt.red
    PLAYER_B_COLOR    = Qt.yellow

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.move_record_model = MoveRecordModel()

        #palette = self.palette()
        #palette.setColor(QPalette.ColorRole.Window, QColor("#66acd1"))
        #self.setPalette(palette)
        #self.setAutoFillBackground(True) 

        self.status_widget = QLabel()
        self.status_widget.setAlignment(Qt.AlignCenter)
        font = self.status_widget.font()
        font.setBold(True)
        self.status_widget.setFont(font)

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

        game_state_checkbox = QCheckBox("Show game state")
        game_state_checkbox.setChecked(True)
        game_state_checkbox.stateChanged.connect(self.update_gui)

        move_info_checkbox = QCheckBox("Show move info")
        move_info_checkbox.setChecked(True)
        move_info_checkbox.stateChanged.connect(self.update_gui)

        left_checkbox_layout = QHBoxLayout()
        left_checkbox_layout.addStretch()
        left_checkbox_layout.addWidget(game_state_checkbox)
        left_checkbox_layout.addStretch()
        left_checkbox_layout.addWidget(move_info_checkbox)
        left_checkbox_layout.addStretch()

        left_layout = QVBoxLayout()
        left_layout.addWidget(self.status_widget)
        left_layout.addLayout(grid_layout)
        left_layout.addLayout(left_checkbox_layout)

        self.move_record_widget = QTableView()
        self.move_record_widget.setModel(self.move_record_model)

        reset_button = QPushButton("Reset")
        reset_button.clicked.connect(self.reset)

        takeback_button = QPushButton("Takeback")
        takeback_button.clicked.connect(self.takeback)

        right_button_layout = QHBoxLayout()
        right_button_layout.addStretch()
        right_button_layout.addWidget(reset_button)
        right_button_layout.addStretch()
        right_button_layout.addWidget(takeback_button)
        right_button_layout.addStretch()

        right_layout = QVBoxLayout()
        right_layout.addWidget(self.move_record_widget)
        right_layout.addLayout(right_button_layout)

        h_layout = QHBoxLayout()
        h_layout.addLayout(left_layout)
        h_layout.addLayout(right_layout)

        self.setLayout(h_layout)

        self.game_state_checkbox = game_state_checkbox
        self.move_info_checkbox = move_info_checkbox
        self.takeback_button = takeback_button
        self.reset_button = reset_button

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
        for i in reversed(range(Board.V_SIZE)):
            for j in range(Board.H_SIZE):
                entries.append(self.get_field(i, j))

        return Board(entries)

    def update_gui(self) -> None:

        board = self.make_board()
        #board.print()

        app = QApplication.instance()

        mover = board.mover()

        status_messages = []

        moves_available = False
        for col in range(Board.H_SIZE):
            drop_board = board.drop(col)
            if drop_board is None:
                self.drop_buttons[col].setEnabled(False)
                self.drop_labels[col].setText("n/a")
            else:
                moves_available = True
                self.drop_buttons[col].setEnabled(True)
                drop_board_score = app.lookup(drop_board)
                if drop_board_score.outcome == Outcome.DRAW:
                    self.drop_labels[col].setText("draw\n({})".format(drop_board_score.ply))
                elif drop_board_score.outcome == Outcome.A_WINS:
                    self.drop_labels[col].setText("<font color=\"red\">red win<br>({})</font>".format(drop_board_score.ply))
                elif drop_board_score.outcome == Outcome.B_WINS:
                    self.drop_labels[col].setText("<font color=\"yellow\">yellow win<br>({})</font>".format(drop_board_score.ply))
                else:
                    self.drop_labels[col].setText("indeterminate")

                self.drop_labels[col].setVisible(self.move_info_checkbox.isChecked())

        score = app.lookup(board)

        if not moves_available:
            if score.outcome == Outcome.A_WINS:
                status_message = "game over; <font color=\"red\">red</font> wins."
            elif score.outcome == Outcome.B_WINS:
                status_message = "game over; <font color=\"yellow\">yellow</font> wins."
            else:
                status_message = "game over; draw."
        else:
            if self.game_state_checkbox.isChecked():
                if mover == Player.A:
                    if score.outcome == Outcome.A_WINS:
                        status_message = "<font color=\"red\">red</font> to move and has a {}-ply win.".format(score.ply)
                    elif score.outcome == Outcome.B_WINS:
                        status_message = "<font color=\"red\">red</font> to move but <font color=\"yellow\">yellow</font> has a {}-ply win.".format(score.ply)
                    else:
                        status_message = "<font color=\"red\">red</font> to move and can reach a {}-ply draw.".format(score.ply)
                elif mover == Player.B:
                    if score.outcome == Outcome.B_WINS:
                        status_message = "<font color=\"yellow\">yellow</font> to move and has a {}-ply win.".format(score.ply)
                    elif score.outcome == Outcome.A_WINS:
                        status_message = "<font color=\"yellow\">yellow</font> to move but <font color=\"red\">red</font> has a {}-ply win.".format(score.ply)
                    else:
                        status_message = "<font color=\"yellow\">yellow</font> to move and can reach a {}-ply draw.".format(score.ply)
            else:
                if mover == Player.A:
                    status_message = "<font color=\"red\">red</font> to move."
                elif mover == Player.B:
                    status_message = "<font color=\"yellow\">yellow</font> to move."

        self.status_widget.setText(status_message)

        self.takeback_button.setEnabled(len(self.move_record_model) != 0)
        self.reset_button.setEnabled(len(self.move_record_model) != 0)


class MyMainWindow(QMainWindow):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setWindowTitle("Connect-4")
        connect4_widget = Connect4Widget()
        self.setCentralWidget(connect4_widget)


class MyApplication(QApplication):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._lookup_table = LookupTable("connect4_6x5.new", 6)
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
