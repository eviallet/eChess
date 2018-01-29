#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

// TODO save/load FEN
// TODO handle pawn promotion

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);
    /// board setup
    boardView = ui->board;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            board[c][r] = Square(c,r);

    /// stockfish setup
    stockfish = new Stockfish(this);
    stockfish->send("isready");
    stockfish->send("uci");

    /// model setup
    model = new BoardModel();
    model->insertColumns(0,8);
    model->insertRows(0,8);
    lastClick = NULL;
    turn = Turn::HUMAN;
    initBoard();

    /// views setup
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
    boardView->setSelectionMode(QAbstractItemView::NoSelection);

    /// window setup
    setFixedSize(QSize(
                     ui->board->size().width(),
                     ui->board->size().height()
                     +ui->statusBar->size().height()));
    setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/pieces/WN"))));
    setWindowTitle("ChessOS");

    /// signals/slots
    connect(ui->actionNouvelle_partie,SIGNAL(triggered()),this,SLOT(initBoard()));
    connect(ui->actionQuitter,SIGNAL(triggered()),this,SLOT(initBoard()));
    connect(boardView,SIGNAL(clicked(QModelIndex)),this,SLOT(clicked(QModelIndex)));
    connect(stockfish,SIGNAL(bestMove(QString)),this,SLOT(bestMove(QString)));
    connect(stockfish,SIGNAL(info(QString)),this,SLOT(info(QString)));
    connect(stockfish,SIGNAL(uciok()),this,SLOT(uciok()));
    connect(stockfish,SIGNAL(readyok()),this,SLOT(readyok()));
}


/// Board management
void ChessBoardWindow::initBoard() {
    stockfish->send("ucinewgame");
    game = "position startpos moves";

    for(int i=0; i<COLS; i++) {
        board[i][1].setPiece(Piece::W_PAWN);  // row 2
        board[i][6].setPiece(Piece::B_PAWN);  // row 7
    }

    board[-'a'+'a'][0].setPiece(Piece::W_ROOK);   // A1
    board[-'a'+'b'][0].setPiece(Piece::W_KNIGHT); // B1
    board[-'a'+'c'][0].setPiece(Piece::W_BISHOP); // ..
    board[-'a'+'d'][0].setPiece(Piece::W_QUEEN);
    board[-'a'+'e'][0].setPiece(Piece::W_KING);
    board[-'a'+'f'][0].setPiece(Piece::W_BISHOP);
    board[-'a'+'g'][0].setPiece(Piece::W_KNIGHT);
    board[-'a'+'h'][0].setPiece(Piece::W_ROOK);   // H1

    board[-'a'+'a'][7].setPiece(Piece::B_ROOK);   // A8
    board[-'a'+'b'][7].setPiece(Piece::B_KNIGHT); // B8
    board[-'a'+'c'][7].setPiece(Piece::B_BISHOP); // ..
    board[-'a'+'d'][7].setPiece(Piece::B_QUEEN);
    board[-'a'+'e'][7].setPiece(Piece::B_KING);
    board[-'a'+'f'][7].setPiece(Piece::B_BISHOP);
    board[-'a'+'g'][7].setPiece(Piece::B_KNIGHT);
    board[-'a'+'h'][7].setPiece(Piece::B_ROOK);   // H8

    updateBoard(true);
}

void ChessBoardWindow::updateBoard(bool send) {
    updateBoard(NULL);
    if(send)
        stockfish->send(game);
}

void ChessBoardWindow::updateBoard(QList<Square> *legalMoves) {
    this->legalMoves = legalMoves;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++) {
            QPixmap background(SQUARE_SIZE, SQUARE_SIZE);
            QPainter painter(&background);
            if((c+r)%2!=0)
                painter.setPen(QColor(Qt::white));
            else
                painter.setPen(QColor(Qt::gray));

            painter.drawImage(0, 0, board[r][7-c].getPic().value<QImage>().scaled(SQUARE_SIZE,SQUARE_SIZE));

            if(Square::contains(legalMoves, board[r][7-c]))
                painter.drawImage(0,0,QImage(":/circle.png").scaled(SQUARE_SIZE/8,SQUARE_SIZE/8));

            painter.end();

            model->setData(model->index(c,r), board[r][7-c].getPic(), Qt::DecorationRole);
        }
}

/// Moving
void ChessBoardWindow::clicked(QModelIndex index) {
    if(turn==Turn::HUMAN) {
        Square selected = board[index.column()][7-index.row()];
        if(lastClick==NULL) {
            qDebug() << "Clicked from "+selected.toString();
            lastClick = new Square(selected);
            updateBoard(Analyzer::getLegalMoves(board,selected));
        } else if(lastClick->toString() != selected.toString()){
            qDebug() << "Clicked to "+selected.toString();
            move(*lastClick,selected,Turn::STOCKFISH);
            lastClick = NULL;
        } else {
            qDebug() << "Cancelled click";
            updateBoard(false);
            lastClick = NULL;
        }
    }
}

void ChessBoardWindow::move(Square from, Square to, Turn turn) {
    this->turn = turn;
    board[to.c()][to.r()].setPiece(board[from.c()][from.r()].getPiece());
    board[from.c()][from.r()].setPiece(Piece::NONE);
    if(from=="e1"&&to=="g1"&&board[to.c()][to.r()].getPiece()==Piece::W_KING) {
        castling(Castling::W_KINGSIDE);
    } else if(from=="e1"&&to=="c1"&&board[to.c()][to.r()].getPiece()==Piece::W_KING) {
        castling(Castling::W_QUEENSIDE);
    } else if(from=="e8"&&to=="g8"&&board[to.c()][to.r()].getPiece()==Piece::B_KING) {
        castling(Castling::B_KINGSIDE);
    } else if(from=="e8"&&to=="c8"&&board[to.c()][to.r()].getPiece()==Piece::B_KING) {
        castling(Castling::B_QUEENSIDE);
    }

    game.append(" ").append(from.toString()).append(to.toString());
    qDebug() << game;
    if(this->turn==Turn::STOCKFISH) {
        updateBoard(true);
        think();
    } else
        updateBoard(false);

}

void ChessBoardWindow::move(char fromCol, int fromRow, char toCol, int toRow) {
    move(*square(fromCol,fromRow),*square(toCol,toRow), Turn::HUMAN);
}

void ChessBoardWindow::castling(Castling type) {
    qDebug() << "castling";
    switch(type) {
        case Castling::W_KINGSIDE:
            square('f',1)->setPiece(square('h',1)->getPiece());
            square('h',1)->setPiece(Piece::NONE);
            break;
        case Castling::W_QUEENSIDE:
            square('d',1)->setPiece(square('a',1)->getPiece());
            square('a',1)->setPiece(Piece::NONE);
            break;
        case Castling::B_KINGSIDE:
            square('f',8)->setPiece(square('h',8)->getPiece());
            square('h',8)->setPiece(Piece::NONE);
            break;
        case Castling::B_QUEENSIDE:
            square('d',8)->setPiece(square('a',8)->getPiece());
            square('a',8)->setPiece(Piece::NONE);
            break;
    }
}



void ChessBoardWindow::think() {
    stockfish->send("go");
    ui->statusBar->showMessage("Calcul du prochain coup...");
}

Square* ChessBoardWindow::square(char col, int row) {
    return &board[col-'a'][row-1];
}

/// STOCKFISH SIGNALS
void ChessBoardWindow::bestMove(QString mv) {
    ui->statusBar->showMessage("En attente du prochain coup.");
    mv = mv.section(" ",1,2);
    move(mv[0].toLatin1(),mv[1].digitValue(),mv[2].toLatin1(),mv[3].digitValue());
}

void ChessBoardWindow::uciok() {

}

void ChessBoardWindow::readyok() {

}

void ChessBoardWindow::info(QString info) {
    qDebug() << info;
    if(info.contains("score mate 1")) {
        ui->statusBar->showMessage("Partie terminée.");
        turn = Turn::GAME_OVER;
    }
}

/// Stockfish options
void ChessBoardWindow::setDifficulty(int dif) {
    stockfish->setDifficulty(dif);
}

/// Window management
ChessBoardWindow::~ChessBoardWindow() {
    delete ui;
}

void ChessBoardWindow::exit() {
    QCoreApplication::quit();
}

/*
QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage) {
    QImage imageWithOverlay = QImage(baseImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}
*/

