#ifndef CHESSBOARDWINDOW_H
#define CHESSBOARDWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPainter>
#include <QInputDialog>

#include "square.h"
#include "boardmodel.h"
#include "stockfish.h"
#include "movelist.h"
#include "promotiondialog.h"
#include "constants.h"

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
    void receivedFEN(QString f);
    void receivedLegalMove(QString move);
    void allLegalMovesReceived(int);
    void receivedCheckers(QString f);
private slots:
    void setFEN(QString receivedFEN);
    void updateBoard();
    void updateBoardFEN();
    void queryFEN();
    void updateLegalMoves();
    void updateLegalMovesShown(Square from = Square(0));
    void move(Square from, Square to);
    void proceedToNextTurn();
    void move(char fromCol, int fromRow, char toCol, int toRow);
    void castling(Castling type);
    void think();
    void setDifficulty(int dif);
    bool humanToPlay();
    Square *square(char col, int row);
    void nextTurn();
    void pawnPromotionChosen(char p);
    void setupOverlay(QImage *overlay, QColor color, bool isRect = false, qreal alpha = 1.0);
private:
    Ui::ChessBoardWindow *ui;
    Stockfish *stockfish;

    QString game;
    QString fen;

    QTableView *boardView;
    BoardModel *model;
    Square board[COLS][ROWS];
    QImage *overlayCircle;
    QImage *overlaySquare;
    QImage *overlayCheck;
    QImage *overlayCheckers;

    Turn turn;
    Player whitePlyr;
    Player blackPlyr;
    bool gameOver;
    bool lockChessboard = false;

    MoveList legalMoves;
    MoveList legalMovesShown;

    Square lastFrom;
    Square lastTo;
    Square *lastClick;
    QList<Square> checkersSquares;

    char lastPawnPromotion = ' ';

};

#endif // CHESSBOARDWINDOW_H
