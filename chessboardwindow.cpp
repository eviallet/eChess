#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

// TODO save/load FEN
// TODO handle pawn promotion
// TODO human playing black and flip board

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);

    /// board setup
    boardView = ui->board;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            board[c][r] = Square(c,r);

    /// stockfish setup
    stockfish = new Stockfish(this);

    /// circle setup
    circle = new QImage(CIRCLE_SIZE, CIRCLE_SIZE, QImage::Format_ARGB32);
    circle->fill(Qt::transparent);
    QPainter painter(circle);
    painter.setBrush(Qt::green);
    painter.drawEllipse(0,0,CIRCLE_SIZE-1,CIRCLE_SIZE-1);
    painter.end();

    /// model setup
    model = new BoardModel();
    model->insertColumns(0,8);
    model->insertRows(0,8);
    lastClick = NULL;
    whitePlyr = Player::HUMAN;
    blackPlyr = Player::STOCKFISH;
    turn = Turn::WHITE;
    initBoard();

    /// views setup
    boardView->setModel(model);
    boardView->setShowGrid(false);
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
                     +ui->statusBar->size().height()+5));
    setWindowIcon(QIcon(QPixmap::fromImage(QImage(":/pieces/WN"))));
    setWindowTitle("ChessOS");

    /// signals/slots
    connect(ui->actionNouvelle_partie,SIGNAL(triggered()),this,SLOT(initBoard()));
    connect(ui->actionQuitter,SIGNAL(triggered()),this,SLOT(exit()));
    connect(boardView,SIGNAL(clicked(QModelIndex)),this,SLOT(clicked(QModelIndex)));
    connect(stockfish,SIGNAL(bestMove(QString)),this,SLOT(bestMove(QString)));
    connect(stockfish,SIGNAL(info(QString)),this,SLOT(info(QString)));
    connect(stockfish,SIGNAL(uciok()),this,SLOT(uciok()));
    connect(stockfish,SIGNAL(readyok()),this,SLOT(readyok()));


    stockfish->send("isready");
}


/// =========================================================================
///                             BOARD MANAGEMENT
/// =========================================================================

void ChessBoardWindow::setFEN(QString fen) {
    unsigned char col = 0, row = 0;

    for(int i=0; i<fen.length(); i++) {
        char fenChar = fen[i].toLatin1();

        if(fenChar == '/') {
            col = 0;
            row++;
        } else if('1' <= fenChar && fenChar <= '8') {
            unsigned char pos = fenChar - '0';
            for(unsigned char c = col; c < col + pos; c++)
                board[c][7-row].setPiece(Piece::NONE); // FEN starts at rank 8
            col += fenChar;
        } else if(fenChar == ' ') {
            break;
        } else {
            board[col][7-row].setPiece(Square::fenToPiece(fenChar));
            col++;
        }
    }

    updateBoard();
}

void ChessBoardWindow::initBoard() {
    stockfish->send("ucinewgame");
    pgn = "";
    gameOver = false;
    lastPGNMove = 0;

    setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    if(whitePlyr == Player::STOCKFISH)
        think();
}

void ChessBoardWindow::printBoardDebug() {
    QString boardDebug;
    for(int r=ROWS-1; r>=0; r--) {
        boardDebug = "";
        for(int c=0; c<COLS; c++) {
            char p = board[c][r].getPieceFEN();
            if(p == 0)
                p = ' ';
            boardDebug += p;
            boardDebug.append(" ");
            if(c == COLS - 1)
                boardDebug.append("  ").append(QString::number(r+1));
        }
        qDebug() << boardDebug;
    }
    qDebug() << "\"               ";
    qDebug() << "\"A B C D E F G H";
}

void ChessBoardWindow::updateFEN() {
    char emptyCount;

    QString boardFEN = "";

    for(int r=ROWS-1; r>=0; r--) {
        emptyCount = 0;

        for(int c=0; c<COLS; c++) { // FEN starts from rank 8
            char p = board[c][r].getPieceFEN();
            if(p == 0)
                emptyCount++;
            else if(emptyCount > 0) {
                boardFEN += QString::number(emptyCount);
                emptyCount = 0;
                boardFEN += p;
            } else
                boardFEN += p;

        }

        if(emptyCount > 0) {
            boardFEN += QString::number(emptyCount);
            emptyCount = 0;
        }
        // for each row except the last one, add '/' to denote end of row
        if(r!=0)
            boardFEN += "/";
    }

    fen.setBoardFEN(boardFEN);
    qDebug() << fen.getFEN();
}

void ChessBoardWindow::updateBoard(QList<Square> legalMoves) {
    this->legalMoves = legalMoves;
    for(int c=0; c<COLS; c++) {
        for(int r=0; r<ROWS; r++) {
            QPixmap overlay(SQUARE_SIZE, SQUARE_SIZE);
            overlay.fill(Qt::transparent);
            QPainter painter(&overlay);

            if(board[c][r].getPiece()!=Piece::NONE)
                painter.drawImage(0, 0, board[c][r].getPic().scaled(SQUARE_SIZE,SQUARE_SIZE));

            if(Square::contains(legalMoves, board[c][r]))
                painter.drawImage(overlay.width()/2-CIRCLE_SIZE/2, overlay.height()/2-CIRCLE_SIZE/2, *circle);

            painter.end();

            // model is rotated = r & c flipped and r starts from 8 instead of 1
            model->setData(model->index(7-r,c), overlay);
        }
    }

    updateFEN();
}

/// =========================================================================
///                                     MOVING
/// =========================================================================

bool ChessBoardWindow::humanToPlay() {
    if(turn == Turn::WHITE && whitePlyr == Player::HUMAN)
        return true;
    else if(turn == Turn::BLACK && blackPlyr == Player::HUMAN)
        return true;
    return false;
}

void ChessBoardWindow::clicked(QModelIndex index) {
    if(gameOver || !humanToPlay())
        return;

    Square selected = board[index.column()][7-index.row()];
    if(selected.getPiece()<=Piece::W_KING) { /// couleur joueur
        if(lastClick==NULL) {
            //qDebug() << "Clicked from "+selected.toString();
            lastClick = new Square(selected);
            updateBoard(Analyzer::getLegalMoves(board,selected));
        } else if(lastClick->toString() != selected.toString()){
            if(Square::contains(legalMoves,selected)) {
                //qDebug() << "Clicked to "+selected.toString();
                move(*lastClick,selected);
                lastClick = NULL;
            }
        } else {
            //qDebug() << "Cancelled click";
            updateBoard();
            lastClick = NULL;
        }
    }
}

void ChessBoardWindow::move(Square from, Square to) {
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

    turn = fen.nextTurn();

    int movesCount = fen.getMovesCount();
    if(movesCount != lastPGNMove) {
        lastPGNMove = movesCount;
        pgn.append(QString(" %1.").arg(QString::number(movesCount)));
    }
    pgn.append(" ").append(from.toString()).append(to.toString());


    updateBoard();
    qDebug() << pgn;

    if(gameOver) {
        ui->statusBar->showMessage("Partie terminée.");
    } else if(!humanToPlay()) { // Stockfish's turn
        think();
    }
    // else : human turn
}

void ChessBoardWindow::move(char fromCol, int fromRow, char toCol, int toRow) {
    move(*square(fromCol,fromRow),*square(toCol,toRow));
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
    stockfish->send(QString("position fen ").append(fen.getFEN()));
    stockfish->send("go");
    ui->statusBar->showMessage("Calcul du prochain coup...");
}

Square* ChessBoardWindow::square(char col, int row) {
    return &board[col-'a'][row-1];
}

/// =========================================================================
///                            STOCKFISH ANSWERS
/// =========================================================================

void ChessBoardWindow::readyok() {
    stockfish->send("uci");
}


void ChessBoardWindow::uciok() {

}

void ChessBoardWindow::info(QString info) {
    //qDebug() << info;
    if(info.contains("score mate 1"))
        gameOver = true;
}

void ChessBoardWindow::bestMove(QString mv) {
    ui->statusBar->showMessage("En attente du prochain coup.");
    mv = mv.section(" ",1,2);
    move(mv[0].toLatin1(),mv[1].digitValue(),mv[2].toLatin1(),mv[3].digitValue());
}

/// =========================================================================
///                             STOCKFISH OPTIONS
/// =========================================================================

void ChessBoardWindow::setDifficulty(int dif) {
    stockfish->setDifficulty(dif);
}

/// =========================================================================
///                             WINDOW MANAGEMENT
/// =========================================================================

ChessBoardWindow::~ChessBoardWindow() {
    delete ui;
}

void ChessBoardWindow::exit() {
    stockfish->kill();
    QCoreApplication::quit();
}


