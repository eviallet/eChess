#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);
    boardView = ui->board;
    for(int i=0; i<COLS*ROWS; i++)
        board[i] = Square(i%8,i/8);

    model = new BoardModel();
    model->insertColumns(0,8);
    model->insertRows(0,8);
    for(int i=0; i<COLS*ROWS; i++)
        model->setData(model->index(i%8,i/8),board[i]);
    boardView->setModel(model);
    boardView->horizontalHeader()->setVisible(false);
    boardView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    boardView->horizontalHeader()->setDefaultSectionSize(SQUARE_SIZE);
    boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardView->verticalHeader()->setVisible(false);
    boardView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    boardView->verticalHeader()->setDefaultSectionSize(SQUARE_SIZE);
    boardView->verticalHeader()->setAutoScroll(false);
    boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardView->setSelectionMode(QAbstractItemView::SingleSelection);

    setFixedSize(QSize(
                     ui->board->size().width(),
                     ui->board->size().height()
                     +ui->mainToolBar->size().height()
                     +ui->statusBar->size().height()));

    stockfish = new Stockfish(this);
    stockfish->send("isready");
    stockfish->send("uci");
    initBoard();
}

/*
void ChessBoardWindow::click() {
    move(board[Square::getSquare('e',2)],board[Square::getSquare('e',4)]);
}
*/

void ChessBoardWindow::initBoard() {
    game = "position startpos moves"; // TODO FEN

    for(int i=0; i<COLS; i++) {
        board[8*(2-1)+i].setPiece(W_PAWN);  // row 2
        board[8*(7-1)+i].setPiece(B_PAWN);  // row 7
    }

    board[0].setPiece(W_ROOK);      // A1
    board[1].setPiece(W_KNIGHT);    // B1
    board[2].setPiece(W_BISHOP);    // ..
    board[3].setPiece(W_QUEEN);
    board[4].setPiece(W_KING);
    board[5].setPiece(W_BISHOP);
    board[6].setPiece(W_KNIGHT);
    board[7].setPiece(W_ROOK);      // H1

    board[56+0].setPiece(B_ROOK);   // A8
    board[56+1].setPiece(B_KNIGHT); // B8
    board[56+2].setPiece(B_BISHOP); // ..
    board[56+3].setPiece(B_QUEEN);
    board[56+4].setPiece(B_KING);
    board[56+5].setPiece(B_BISHOP);
    board[56+6].setPiece(B_KNIGHT);
    board[56+7].setPiece(B_ROOK);   // H8

    updateBoard();
}

void ChessBoardWindow::updateBoard() {
    for(int i=0; i<COLS*ROWS; i++)
        model->setData(model->index(i%8,i/8),board[i],Qt::DecorationRole);
    stockfish->send(game);
}

void ChessBoardWindow::move(Square from, Square to) {
    board[to.getIndex()].setPiece(board[from.getIndex()].getPiece());
    board[from.getIndex()].setPiece(NONE);
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
