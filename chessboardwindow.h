#ifndef CHESSBOARDWINDOW_H
#define CHESSBOARDWINDOW_H

#include <QMainWindow>
#include <QTableView>

#define ROWS 8
#define COLS 8
#define SQUARE_SIZE 80

#include "square.h"
#include "boardmodel.h"
#include "stockfish.h"


namespace Ui {
    class ChessBoardWindow;
}

class ChessBoardWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit ChessBoardWindow(QWidget *parent = 0);
        ~ChessBoardWindow();

    private:
        Ui::ChessBoardWindow *ui;
        Stockfish *stockfish;

        QString game;
        QTableView *boardView;
        BoardModel *model;
        Square board[COLS*ROWS];

    private slots:
        void initBoard();
        void updateBoard();
        void move(Square from, Square to);
        void think();
        void setDifficulty(int dif);
};

#endif // CHESSBOARDWINDOW_H
