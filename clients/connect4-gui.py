#! /usr/bin/env -S python3 -B

import argparse
import sys
import random

from PyQt5.QtCore import Qt, QAbstractTableModel, QVariant
from PyQt5.QtGui import QPalette, QColor, QBrush, QPen
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QLabel, QHBoxLayout, QVBoxLayout, \
    QPushButton, QTableView, QCheckBox, QGraphicsView, QGraphicsScene

from utils.simple_types import Player, Outcome
from utils.board import Board
from utils.game_info import GameInfo
from utils.find_optimal_moves import find_optimal_moves




class MoveRecordModel(QAbstractTableModel):
    """A Qt table-model for the move list."""

    def __init__(self, manager: GameManager, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.manager = manager

    def __len__(self):
        return len(self.manager.moves)

    def rowCount(self, parent):
        return (len(self.manager.moves) + 1) // 2

    def columnCount(self, parent):
        return 2

    def data(self, index, role):
        row = index.row()
        col = index.column()
        move_index = row * 2 + col
        if role == Qt.DisplayRole:
            if move_index < len(self.manager.moves):
                return QVariant(str(self.manager.moves[move_index] + 1))
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


class BoardWidget(QGraphicsView):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        scene = QGraphicsScene()

        pen_color = QColor("#ff0000")
        pen_brush = QBrush(pen_color, Qt.SolidPattern)
        pen_width = 1.0 / 40.0
        pen_style = Qt.SolidLine
        pen_capstyle = Qt.SquareCap
        pen_joinstyle = Qt.BevelJoin

        pen = QPen(pen_brush, pen_width, pen_style, pen_capstyle, pen_joinstyle)

        scene.addRect(0, 0, 7, 6, pen)

        for x in range(7):
            for y in range(x + 1):
                scene.addEllipse(x, y, 1, 1, pen)

        self.setScene(scene)

        self.fitInView(scene.sceneRect(), Qt.KeepAspectRatio)


class Connect4Widget(QWidget):
    """A widget for playing connect-4."""

    CHIP_SIZE = 60

    BLUE_BACKGROUND_COLOR  = QColor("#3399ff")
    PLAYER_NONE_COLOR = Qt.white
    PLAYER_A_COLOR    = Qt.red
    PLAYER_B_COLOR    = Qt.yellow

    BUTTON_DISABLED_TEXT_COLOR       = QColor("#bbbbbb")
    BUTTON_DISABLED_BACKGROUND_COLOR = QColor("#cccccc")
    BUTTON_ENABLED_BACKGROUND_COLOR  = QColor("#3399ff")
    TITLE_BACKGROUND_COLOR           = QColor("#99ccff")

    def __init__(self, manager, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.manager = manager

        info = manager.info

        self.move_record_model = MoveRecordModel(manager)

        board_widget = BoardWidget()

        self.status_widget = QLabel()
        font = self.status_widget.font()
        font.setPointSize(18)
        self.status_widget.setFont(font)

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

        blue_widget = QWidget()

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
        left_layout.addWidget(board_widget)
        left_layout.addStretch()
        left_layout.addLayout(left_checkbox_layout)

        self.move_record_widget = QTableView()
        self.move_record_widget.setModel(self.move_record_model)

        reset_button = QPushButton("Reset")
        reset_button.clicked.connect(self.reset_game)

        undo_move_button = QPushButton("Undo")
        undo_move_button.clicked.connect(self.undo_move)

        computer_move_button = QPushButton("Move!")
        computer_move_button.clicked.connect(self.computer_move)

        right_button_layout = QHBoxLayout()
        right_button_layout.addStretch()
        right_button_layout.addWidget(computer_move_button)
        right_button_layout.addStretch()
        right_button_layout.addWidget(reset_button)
        right_button_layout.addStretch()
        right_button_layout.addWidget(undo_move_button)
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
        self.computer_move_button = computer_move_button
        self.undo_move_button = undo_move_button
        self.reset_button = reset_button

        self.update_gui()

    def reset_game(self) -> None:
        self.move_record_model.beginResetModel()
        self.manager.reset_game()
        self.move_record_model.endResetModel()
        self.update_gui()

    def execute_move(self, col: int) -> None:
        self.move_record_model.beginResetModel()
        self.manager.execute_move(col)
        self.move_record_model.endResetModel()
        self.update_gui()

    def undo_move(self) -> None:
        self.move_record_model.beginResetModel()
        self.manager.undo_move()
        self.move_record_model.endResetModel()
        self.update_gui()

    def computer_move(self) -> None:
        """Let the computer pick a good move."""
        board = self.manager.current_board()
        optimal_moves = find_optimal_moves(board)
        if len(optimal_moves) == 1:
            col = optimal_moves[0]
        else:
            col = random.choice(optimal_moves)
        self.execute_move(col)

    def update_gui(self) -> None:

        self.manager.current_board().print()

        #info = self.info

        #board = self.make_board()

        #mover = board.mover()

        #status_messages = []

        #moves_available = False
        #for col in range(info.h_size):
        #    drop_board = board.execute_move(col)

        #    if drop_board is None:

        #        palette = self.drop_buttons[col].palette()
        #        palette.setColor(QPalette.ColorRole.Button, self.BUTTON_DISABLED_BACKGROUND_COLOR)
        #        palette.setColor(QPalette.ColorRole.ButtonText, self.BUTTON_DISABLED_TEXT_COLOR)
        #        self.drop_buttons[col].setPalette(palette)

        #        self.drop_buttons[col].setEnabled(False)
        #        self.drop_labels[col].setText("n/a")
        #    else:
        #        moves_available = True
        #        self.drop_buttons[col].setEnabled(True)

        #        palette = self.drop_buttons[col].palette()
        #        palette.setColor(QPalette.ColorRole.Button, self.BUTTON_ENABLED_BACKGROUND_COLOR)
        #        if mover == Player.A:
        #            palette.setColor(QPalette.ColorRole.ButtonText, self.PLAYER_A_COLOR)
        #        elif mover == Player.B:
        #            palette.setColor(QPalette.ColorRole.ButtonText, self.PLAYER_B_COLOR)
        #        self.drop_buttons[col].setPalette(palette)

        #        drop_board_score = info.lookup(drop_board)
        #        if drop_board_score.outcome == Outcome.DRAW:
        #            self.drop_labels[col].setText("draw\n({})".format(drop_board_score.ply + 1))
        #        elif drop_board_score.outcome == Outcome.A_WINS:
        #            self.drop_labels[col].setText("<font color=\"red\">red win<br>({})</font>".format(drop_board_score.ply + 1))
        #        elif drop_board_score.outcome == Outcome.B_WINS:
        #            self.drop_labels[col].setText("<font color=\"yellow\">yellow win<br>({})</font>".format(drop_board_score.ply + 1))
        #        else:
        #            self.drop_labels[col].setText("indeterminate")

        #    self.drop_labels[col].setVisible(self.move_info_checkbox.isChecked())

        #score = info.lookup(board)

        #self.move_button.setEnabled(moves_available)

        #if not moves_available:
        #    if score.outcome == Outcome.A_WINS:
        #        status_message = "game over; <font color=\"red\">red</font> wins"
        #    elif score.outcome == Outcome.B_WINS:
        #        status_message = "game over; <font color=\"yellow\">yellow</font> wins"
        #    else:
        #        status_message = "game over; draw."
        #else:
        #    if self.game_state_checkbox.isChecked():
        #        if mover == Player.A:
        #            if score.outcome == Outcome.A_WINS:
        #                status_message = "<font color=\"red\">red</font> to move and has a {}-ply win".format(score.ply)
        #            elif score.outcome == Outcome.B_WINS:
        #                status_message = "<font color=\"red\">red</font> to move but <font color=\"yellow\">yellow</font> has a {}-ply win".format(score.ply)
        #            else:
        #                status_message = "<font color=\"red\">red</font> to move and can reach a {}-ply draw".format(score.ply)
        #        elif mover == Player.B:
        #            if score.outcome == Outcome.B_WINS:
        #                status_message = "<font color=\"yellow\">yellow</font> to move and has a {}-ply win".format(score.ply)
        #            elif score.outcome == Outcome.A_WINS:
        #                status_message = "<font color=\"yellow\">yellow</font> to move but <font color=\"red\">red</font> has a {}-ply win".format(score.ply)
        #            else:
        #                status_message = "<font color=\"yellow\">yellow</font> to move and can reach a {}-ply draw".format(score.ply)
        #    else:
        #        if mover == Player.A:
        #            status_message = "<font color=\"red\">red</font> to move"
        #        elif mover == Player.B:
        #            status_message = "<font color=\"yellow\">yellow</font> to move"

        #self.status_widget.setText(status_message)

        self.undo_move_button.setEnabled(len(self.manager.moves) != 0)
        self.reset_button.setEnabled(len(self.manager.moves) != 0)
        self.computer_move_button.setEnabled(self.manager.current_board().outcome() == Outcome.INDETERMINATE)


class MyMainWindow(QMainWindow):
    """The GUI main window."""
    def __init__(self, manager: GameManager, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setWindowTitle(manager.info.basename)
        connect4_widget = Connect4Widget(manager)
        self.setCentralWidget(connect4_widget)


class MyApplication(QApplication):
    """The GUI application."""
    def __init__(self, manager: GameManager, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.main_window = MyMainWindow(manager)
        self.main_window.show()


def main():

    default_database_filename = "connect4_7x6.dat"
    parser = argparse.ArgumentParser(description="GUI for Connect-4.")
    parser.add_argument("-f", "--filename", default=default_database_filename, help="database filename (default: {!r})".format(default_database_filename))

    args = parser.parse_args()

    with GameInfo(args.filename) as info:
        manager = GameManager(info)
        app = MyApplication(manager, sys.argv)
        exitcode = app.exec_()

    if exitcode != 0:
        sys.exit(exitcode)


if __name__ == "__main__":
    main()
