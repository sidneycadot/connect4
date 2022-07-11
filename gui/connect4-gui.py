#! /usr/bin/env -S python3 -B

import argparse
import sys
import functools
from typing import Optional
import random

from PyQt5.QtCore import Qt, QAbstractTableModel, QVariant
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QLabel, QHBoxLayout, QVBoxLayout, QGridLayout, QPushButton, QTableView, QCheckBox

from simple_types import Player, Outcome
from board import Board
from game_info import GameInfo


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

    CHIP_SIZE = 80

    BUTTON_DISABLED_TEXT_COLOR       = QColor("#bbbbbb")
    BUTTON_DISABLED_BACKGROUND_COLOR = QColor("#cccccc")
    BUTTON_ENABLED_BACKGROUND_COLOR  = QColor("#3399ff")

    TITLE_BACKGROUND_COLOR = QColor("#99ccff")

    BLUE_BACKGROUND_COLOR  = QColor("#3399ff")
    PLAYER_NONE_COLOR = Qt.white
    PLAYER_A_COLOR    = Qt.red
    PLAYER_B_COLOR    = Qt.yellow

    def __init__(self, info, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.info = info

        self.move_record_model = MoveRecordModel()

        self.status_widget = QLabel()
        font = self.status_widget.font()
        font.setPointSize(18)
        self.status_widget.setFont(font)

        grid_layout = QGridLayout()

        #grid_layout.addWidget(self.status_widget, grid_row, 0, 1, info.h_size, Qt.AlignCenter)
        #grid_row += 1

        grid_row = 0

        self.drop_labels = []

        for x in range(info.h_size):
            label = QLabel()
            label.setAlignment(Qt.AlignCenter)
            font = label.font()
            font.setBold(True)
            label.setFont(font)
            grid_layout.addWidget(label, grid_row, x)
            self.drop_labels.append(label)

        grid_row += 1

        self.drop_buttons = []

        for x in range(info.h_size):
            button = QPushButton("↓")

            font = button.font()
            font.setBold(True)
            font.setPointSize(14)
            button.setFont(font)
            button.clicked.connect(functools.partial(self.drop, x))
            grid_layout.addWidget(button, grid_row, x)
            self.drop_buttons.append(button)

        grid_row += 1

        self.rows = []
        for y in range(info.v_size):
            row = []
            for x in range(info.h_size):
                label = QLabel("●")
                font = label.font()
                font.setPointSize(self.CHIP_SIZE)
                label.setFont(font)
                palette = label.palette()
                palette.setColor(QPalette.ColorRole.Foreground, self.PLAYER_NONE_COLOR)
                label.setPalette(palette)
                label.setAlignment(Qt.AlignCenter)
                grid_layout.addWidget(label, grid_row, x)
                row.append(label)
            self.rows.append(row)
            grid_row += 1

        game_state_checkbox = QCheckBox("Show full game state")
        game_state_checkbox.setChecked(True)
        game_state_checkbox.stateChanged.connect(self.update_gui)

        move_info_checkbox = QCheckBox("Show move consequences")
        move_info_checkbox.setChecked(True)
        move_info_checkbox.stateChanged.connect(self.update_gui)

        left_checkbox_layout = QHBoxLayout()
        left_checkbox_layout.addStretch()
        left_checkbox_layout.addWidget(game_state_checkbox)
        left_checkbox_layout.addStretch()
        left_checkbox_layout.addWidget(move_info_checkbox)
        left_checkbox_layout.addStretch()

        blue_layout = QVBoxLayout()
        blue_layout.addLayout(grid_layout)

        blue_widget = QWidget()
        blue_widget.setLayout(blue_layout)

        palette = blue_widget.palette()
        palette.setColor(QPalette.ColorRole.Window, self.BLUE_BACKGROUND_COLOR)
        blue_widget.setPalette(palette)
        blue_widget.setAutoFillBackground(True) 

        left_layout = QVBoxLayout()

        title_layout = QHBoxLayout()
        title_layout.addWidget(self.status_widget, 1, Qt.AlignCenter)

        title_widget = QWidget()
        title_widget.setLayout(title_layout)

        palette = title_widget.palette()
        palette.setColor(QPalette.ColorRole.Background, self.TITLE_BACKGROUND_COLOR)
        title_widget.setPalette(palette)
        title_widget.setAutoFillBackground(True)


        left_layout.addWidget(title_widget)
        left_layout.addStretch()
        left_layout.addWidget(blue_widget)
        left_layout.addStretch()
        left_layout.addLayout(left_checkbox_layout)

        self.move_record_widget = QTableView()
        self.move_record_widget.setModel(self.move_record_model)

        reset_button = QPushButton("Reset")
        reset_button.clicked.connect(self.reset)

        takeback_button = QPushButton("Takeback")
        takeback_button.clicked.connect(self.takeback)

        move_button = QPushButton("Move!")
        move_button.clicked.connect(self.move)

        right_button_layout = QHBoxLayout()
        right_button_layout.addStretch()
        right_button_layout.addWidget(move_button)
        right_button_layout.addStretch()
        right_button_layout.addWidget(reset_button)
        right_button_layout.addStretch()
        right_button_layout.addWidget(takeback_button)
        right_button_layout.addStretch()

        right_layout = QVBoxLayout()
        right_layout.addWidget(self.move_record_widget)
        right_layout.addLayout(right_button_layout)

        h_layout = QHBoxLayout()
        h_layout.addStretch()
        h_layout.addLayout(left_layout)
        h_layout.addStretch()
        h_layout.addLayout(right_layout)
        h_layout.addStretch()

        self.setLayout(h_layout)

        self.game_state_checkbox = game_state_checkbox
        self.move_info_checkbox = move_info_checkbox
        self.move_button = move_button
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
        info = self.info
        for i in range(info.v_size):
            for j in range(info.h_size):
                self.set_field(i, j, Player.NONE)
        self.move_record_model.reset()
        self.update_gui()

    def takeback(self) -> None:
        info = self.info
        col = self.move_record_model.takeback()
        if col is not None:
            for row in range(info.v_size):
                if self.get_field(row, col) != Player.NONE:
                    self.set_field(row, col, Player.NONE)
                    self.update_gui()
                    break

    def move(self) -> None:

        info = self.info

        board = self.make_board()
        mover = board.mover()

        best_score = None
        best_moves = []
        
        for col in range(info.h_size):
            drop_board = board.drop(col)
            if drop_board is None:
                continue

            drop_board_score = info.lookup_table.lookup(drop_board)

            if len(best_moves) == 0:
                best_score = drop_board_score
                best_moves.append(col)
                continue

            if (mover == Player.A and drop_board_score.outcome == Outcome.A_WINS) or (mover == Player.B and drop_board_score.outcome == Outcome.B_WINS):
                # This column wins!
                if best_score.outcome != drop_board_score.outcome:
                    # Replace non-winning best score.
                    best_score = drop_board_score
                    best_moves = [col]                    
                elif drop_board_score.ply < best_score.ply:
                    # Replace winning score that's worse.
                    best_score = drop_board_score
                    best_moves = [col]
                elif drop_board_score.ply == best_score.ply:
                    best_moves.append(col)
                else:
                    pass  # no improvement.
            elif drop_board_score.outcome == Outcome.DRAW:
                if (mover == Player.A and best_score.outcome == Outcome.A_WINS) or (mover == Player.B and best_score.outcome == Outcome.B_WINS):
                    pass # There is already a winning move; no improvement.
                elif best_score.outcome == Outcome.DRAW:
                    if drop_board_score.ply > best_score.ply:
                        best_score = drop_board_score
                        best_moves = [col]
                    elif drop_board_score.ply == best_score.ply:
                        best_moves.append(col)
                    else:
                        pass  # No improvement.
                else:
                    # The best score so far is losing; repace it with this draw.
                    best_score = drop_board_score
                    best_moves = [col]
            else:
                # This column loses.
                if (mover == Player.A and best_score.outcome == Outcome.B_WINS) or (mover == Player.B and best_score.outcome == Outcome.A_WINS):
                    # best move found so far also loses.
                    if drop_board_score.ply > best_score.ply:
                        # Slower loss (give opponent rope to make mistakes).
                        best_score = drop_board_score
                        best_moves = [col]
                    elif drop_board_score.ply > best_score.ply:
                        best_moves.append(col)
                    else:
                        pass # No improvement.

        # Now we select a column and make the move.
        col = random.choice(best_moves)
        self.drop(col)

    def drop(self, col: int) -> None:
        info = self.info
        board = self.make_board()
        mover = board.mover()
        for row in reversed(range(info.v_size)):
            if self.get_field(row, col) == Player.NONE:
                self.set_field(row, col, mover)
                self.move_record_model.append(col)
                self.update_gui()
                break

    def make_board(self) -> Board:
        info = self.info
        entries = []
        for i in reversed(range(info.v_size)):
            for j in range(info.h_size):
                entries.append(self.get_field(i, j))

        return Board(self.info, entries)

    def update_gui(self) -> None:

        info = self.info

        board = self.make_board()

        mover = board.mover()

        status_messages = []

        moves_available = False
        for col in range(info.h_size):
            drop_board = board.drop(col)


            if drop_board is None:

                palette = self.drop_buttons[col].palette()
                palette.setColor(QPalette.ColorRole.Button, self.BUTTON_DISABLED_BACKGROUND_COLOR)
                palette.setColor(QPalette.ColorRole.ButtonText, self.BUTTON_DISABLED_TEXT_COLOR)
                self.drop_buttons[col].setPalette(palette)

                self.drop_buttons[col].setEnabled(False)
                self.drop_labels[col].setText("n/a")
            else:
                moves_available = True
                self.drop_buttons[col].setEnabled(True)

                palette = self.drop_buttons[col].palette()
                palette.setColor(QPalette.ColorRole.Button, self.BUTTON_ENABLED_BACKGROUND_COLOR)
                if mover == Player.A:
                    palette.setColor(QPalette.ColorRole.ButtonText, self.PLAYER_A_COLOR)
                elif mover == Player.B:
                    palette.setColor(QPalette.ColorRole.ButtonText, self.PLAYER_B_COLOR)
                self.drop_buttons[col].setPalette(palette)

                drop_board_score = info.lookup_table.lookup(drop_board)
                if drop_board_score.outcome == Outcome.DRAW:
                    self.drop_labels[col].setText("draw\n({})".format(drop_board_score.ply + 1))
                elif drop_board_score.outcome == Outcome.A_WINS:
                    self.drop_labels[col].setText("<font color=\"red\">red win<br>({})</font>".format(drop_board_score.ply + 1))
                elif drop_board_score.outcome == Outcome.B_WINS:
                    self.drop_labels[col].setText("<font color=\"yellow\">yellow win<br>({})</font>".format(drop_board_score.ply + 1))
                else:
                    self.drop_labels[col].setText("indeterminate")

            self.drop_labels[col].setVisible(self.move_info_checkbox.isChecked())

        score = info.lookup_table.lookup(board)

        self.move_button.setEnabled(moves_available)

        if not moves_available:
            if score.outcome == Outcome.A_WINS:
                status_message = "game over; <font color=\"red\">red</font> wins"
            elif score.outcome == Outcome.B_WINS:
                status_message = "game over; <font color=\"yellow\">yellow</font> wins"
            else:
                status_message = "game over; draw."
        else:
            if self.game_state_checkbox.isChecked():
                if mover == Player.A:
                    if score.outcome == Outcome.A_WINS:
                        status_message = "<font color=\"red\">red</font> to move and has a {}-ply win".format(score.ply)
                    elif score.outcome == Outcome.B_WINS:
                        status_message = "<font color=\"red\">red</font> to move but <font color=\"yellow\">yellow</font> has a {}-ply win".format(score.ply)
                    else:
                        status_message = "<font color=\"red\">red</font> to move and can reach a {}-ply draw".format(score.ply)
                elif mover == Player.B:
                    if score.outcome == Outcome.B_WINS:
                        status_message = "<font color=\"yellow\">yellow</font> to move and has a {}-ply win".format(score.ply)
                    elif score.outcome == Outcome.A_WINS:
                        status_message = "<font color=\"yellow\">yellow</font> to move but <font color=\"red\">red</font> has a {}-ply win".format(score.ply)
                    else:
                        status_message = "<font color=\"yellow\">yellow</font> to move and can reach a {}-ply draw".format(score.ply)
            else:
                if mover == Player.A:
                    status_message = "<font color=\"red\">red</font> to move"
                elif mover == Player.B:
                    status_message = "<font color=\"yellow\">yellow</font> to move"

        self.status_widget.setText(status_message)

        self.takeback_button.setEnabled(len(self.move_record_model) != 0)
        self.reset_button.setEnabled(len(self.move_record_model) != 0)


class MyMainWindow(QMainWindow):

    def __init__(self, info: GameInfo, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setWindowTitle(info.basename)
        connect4_widget = Connect4Widget(info)
        self.setCentralWidget(connect4_widget)


class MyApplication(QApplication):

    def __init__(self, info: GameInfo, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.main_window = MyMainWindow(info)
        self.main_window.show()


def main():

    default_database_filename = "connect4_7x5.new"
    parser = argparse.ArgumentParser(description="GUI for Connect-4 and related games.")
    parser.add_argument("-f", "--filename", default="connect4_7x5.new", help="database filename (default: {!r})".format(default_database_filename))

    args = parser.parse_args()

    with GameInfo(args.filename) as info:
        app = MyApplication(info, sys.argv)
        exitcode = app.exec_()

    if exitcode != 0:
        sys.exit(exitcode)

if __name__ == "__main__":
    main()
