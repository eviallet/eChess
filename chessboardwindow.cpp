#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);

    for(int i=0; i<COLS; i++)
        board[i] = new Square(i%8,i/8);

    boardView = ui->board;
    boardView->setModel(board);

    stockfish = new Stockfish(this);
    stockfish->send("isready");
    stockfish->send("uci");
    initBoard();

    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(click()));
}


void ChessBoardWindow::click() {
    move(*board[-'a'+'e'][2],*board[-'a'+'e'][4]);
}

void ChessBoardWindow::initBoard() {
    game = "position startpos moves"; // TODO FEN
    stockfish->send(game);
}

void ChessBoardWindow::updateBoard() {
    stockfish->send(game);
}

void ChessBoardWindow::move(Square from, Square to) {
    board[to.c()][to.r()] = board[from.c()][from.r()];
    game.append(" ").append(from.toString()).append(to.toString());
    qDebug() << game;
    updateBoard();
}

void ChessBoardWindow::think() {
    stockfish->send("go");
}

void ChessBoardWindow::setDifficulty(int dif) {
    stockfish->setDifficulty(dif);
}

ChessBoardWindow::~ChessBoardWindow() {
    delete ui;
}
