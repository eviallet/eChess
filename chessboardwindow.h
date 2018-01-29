#ifndef CHESSBOARDWINDOW_H
#define CHESSBOARDWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPainter>

#include "square.h"
#include "boardmodel.h"
#include "stockfish.h"
#include "analyzer.h"

enum class Turn {
    HUMAN,
    STOCKFISH,
    GAME_OVER
};

enum class Castling {
    W_KINGSIDE,
    W_QUEENSIDE,
    B_KINGSIDE,
    B_QUEENSIDE
};

namespace Ui {
    class ChessBoardWindow;
}

class ChessBoardWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit ChessBoardWindow(QWidget *parent = 0);
        ~ChessBoardWindow();
    public slots:
        void initBoard();
        void exit();
        void clicked(QModelIndex index);
        void bestMove(QString mv);
        void uciok();
        void readyok();
        void info(QString info);
    private slots:
        void updateBoard(bool send);
        void updateBoard(QList<Square> legalMoves);
        void move(Square from, Square to, Turn turn);
        void move(char fromCol, int fromRow, char toCol, int toRow);
        void castling(Castling type);
        void think();
        void setDifficulty(int dif);

        Square *square(char col, int row);
private:
        Ui::ChessBoardWindow *ui;
        Stockfish *stockfish;

        QString game;
        QTableView *boardView;
        BoardModel *model;
        Square board[COLS][ROWS];
        Turn turn;
        QList<Square> legalMoves;

        Square *lastClick;
};

#endif // CHESSBOARDWINDOW_H
