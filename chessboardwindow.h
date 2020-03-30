#ifndef CHESSBOARDWINDOW_H
#define CHESSBOARDWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPainter>

#include "square.h"
#include "boardmodel.h"
#include "stockfish.h"
#include "analyzer.h"
#include "fen.h"


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
        void setFEN(QString fen);
        void updateFEN();
        void updateBoard(QList<Square> legalMoves = QList<Square>());
        void move(Square from, Square to);
        void move(char fromCol, int fromRow, char toCol, int toRow);
        void castling(Castling type);
        void think();
        void setDifficulty(int dif);
        bool humanToPlay();
private:
        Square *square(char col, int row);
        Ui::ChessBoardWindow *ui;
        Stockfish *stockfish;

        QList<Square> legalMoves;
        QImage *circle;
        QString pgn;
        int lastPGNMove;
        FEN fen;
        QTableView *boardView;
        BoardModel *model;
        Square board[COLS][ROWS];
        Turn turn;
        Player whitePlyr;
        Player blackPlyr;
        bool gameOver;

        Square *lastClick;
        void printBoardDebug();
};

#endif // CHESSBOARDWINDOW_H
