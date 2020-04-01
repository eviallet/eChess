#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

// TODO save/load FEN
// TODO human playing black = flip board

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);

    /// board setup
    boardView = ui->board;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            board[c][r] = Square(c,r);

    /// stockfish setup
    stockfish = new Stockfish(this);

    /// overlayCircle setup for legal moves
    overlayCircle = new QImage(CIRCLE_SIZE, CIRCLE_SIZE, QImage::Format_ARGB32);
    overlayCircle->fill(Qt::transparent);
    QPainter painter(overlayCircle);
    painter.setBrush(Qt::green);
    painter.drawEllipse(0,0,CIRCLE_SIZE-1,CIRCLE_SIZE-1);
    painter.end();

    /// square setup for last click
    overlaySquare = new QImage(SQUARE_SIZE, SQUARE_SIZE, QImage::Format_ARGB32);
    overlaySquare->fill(Qt::transparent);
    QPainter painter2(overlaySquare);
    painter2.setBrush(Qt::blue);
    painter2.setOpacity(0.15);
    painter2.drawRect(0,0,SQUARE_SIZE-1,SQUARE_SIZE-1);
    painter2.end();

    /// model setup
    model = new BoardModel();
    model->insertColumns(0,8);
    model->insertRows(0,8);
    lastClick = NULL;
    whitePlyr = Player::HUMAN_W;
    blackPlyr = Player::STOCKFISH;
    turn = Turn::WHITE;

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
    connect(stockfish,SIGNAL(move(QString)),this,SLOT(receivedLegalMove(QString)));
    connect(stockfish,SIGNAL(endMove()),this,SLOT(allLegalMovesReceived()));
    connect(stockfish,SIGNAL(fen(QString)),this,SLOT(receivedFEN(QString)));


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
    game = "position startpos moves";
    gameOver = false;
    lastPawnPromotion = ' ';
    lockChessboard = false;

    setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    if(whitePlyr == Player::STOCKFISH)
        think();
    else
        updateLegalMoves();
}


void ChessBoardWindow::updateBoardFEN() {
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

    qDebug() << boardFEN;
}

void ChessBoardWindow::updateLegalMoves() {
    lockChessboard = true;
    ui->statusBar->showMessage("Calcul des coups légaux...");

    legalMoves.clear();

    stockfish->send("go perft 1");
}

void ChessBoardWindow::updateLegalMovesShown(Square from) {
    if(from.getPiece() == Piece::NONE)
        legalMovesShown.clear();
    else
        legalMovesShown = legalMoves.movesFrom(from);

    updateBoard();
}

void ChessBoardWindow::updateBoard() {
    for(int c=0; c<COLS; c++) {
        for(int r=0; r<ROWS; r++) {
            QPixmap overlay(SQUARE_SIZE, SQUARE_SIZE);
            overlay.fill(Qt::transparent);
            QPainter painter(&overlay);

            if(lastClick != NULL && board[c][r] == *lastClick)
                painter.drawImage(overlay.width()/2-SQUARE_SIZE/2, overlay.height()/2-SQUARE_SIZE/2, *overlaySquare);

            if(board[c][r].getPiece()!=Piece::NONE)
                painter.drawImage(0, 0, board[c][r].getPic().scaled(SQUARE_SIZE,SQUARE_SIZE));

            if(legalMovesShown.contains(board[c][r]))
                painter.drawImage(overlay.width()/2-CIRCLE_SIZE/2, overlay.height()/2-CIRCLE_SIZE/2, *overlayCircle);


            painter.end();

            // model is rotated = r & c flipped and r starts from 8 instead of 1
            model->setData(model->index(7-r,c), overlay);
        }
    }
}

/// =========================================================================
///                                     MOVING
/// =========================================================================

bool ChessBoardWindow::humanToPlay() {
    if((turn == Turn::WHITE && whitePlyr == Player::HUMAN_W) ||
       (turn == Turn::BLACK && blackPlyr == Player::HUMAN_B)) {
        return true;
    }
    return false;
}

void ChessBoardWindow::clicked(QModelIndex index) {
    if(gameOver || !humanToPlay() || lockChessboard)
        return;

    Square selected = board[index.column()][7-index.row()];

    if(lastClick==NULL && !selected.isEmpty()) {
        //qDebug() << "Clicked from "+selected.toString();
        lastClick = new Square(selected);
        updateLegalMovesShown(selected);
    } else if(lastClick!=NULL && (lastClick->toString() != selected.toString())){
        if(legalMoves.contains(*lastClick, selected)) {
            //qDebug() << "Clicked to "+selected.toString();
            legalMovesShown.clear();
            move(*lastClick,selected);
            lastClick = NULL;
        } else if(!selected.isEmpty()) {
            //qDebug() << "Changed selection to "+selected.toString();
            lastClick = new Square(selected);
            updateLegalMovesShown();
        }
    } else {
        //qDebug() << "Cancelled click";
        lastClick = NULL;
        updateLegalMovesShown();
    }
}

void ChessBoardWindow::move(Square from, Square to) {
    lastFrom = &board[from.c()][from.r()];
    lastTo = &board[to.c()][to.r()];

    board[to.c()][to.r()].setPiece(board[from.c()][from.r()].getPiece());
    board[from.c()][from.r()].setPiece(Piece::NONE);

    // CASTLING
    if(from=="e1"&&to=="g1"&&board[to.c()][to.r()].getPiece()==Piece::W_KING) {
        castling(Castling::W_KINGSIDE);
    } else if(from=="e1"&&to=="c1"&&board[to.c()][to.r()].getPiece()==Piece::W_KING) {
        castling(Castling::W_QUEENSIDE);
    } else if(from=="e8"&&to=="g8"&&board[to.c()][to.r()].getPiece()==Piece::B_KING) {
        castling(Castling::B_KINGSIDE);
    } else if(from=="e8"&&to=="c8"&&board[to.c()][to.r()].getPiece()==Piece::B_KING) {
        castling(Castling::B_QUEENSIDE);
    }

    // PAWN PROMOTION
    else if((from.getPiece() == Piece::W_PAWN && from.r()==6) || (from.getPiece() == Piece::B_PAWN && from.r()==1)) {
        if(!humanToPlay()) {
            pawnPromotionChosen(lastPawnPromotion);
        } else {
            lockChessboard = true;
            PromotionDialog *p = new PromotionDialog(this, turn);
            connect(p, SIGNAL(pieceSelected(char)), this, SLOT(pawnPromotionChosen(char)));
            p->show();
        }
    }

    // ELSE
    else
        proceedToNextTurn();
}

void ChessBoardWindow::pawnPromotionChosen(char p) {
    lockChessboard = false;
    lastPawnPromotion = p;

    lastTo->setPiece(Square::fenToPiece(p));
    proceedToNextTurn();
}

void ChessBoardWindow::proceedToNextTurn() {
    if(gameOver) {
        ui->statusBar->showMessage("Partie terminée.");
        lockChessboard = true;
        return;
    }

    nextTurn();

    QString lastMove = lastFrom->toString() + lastTo->toString();
    if(lastPawnPromotion != ' ') {
        lastMove += lastPawnPromotion;
        lastPawnPromotion = ' ';
    }
    game.append(" ").append(lastMove);

    updateBoard();
    //qDebug() << game;
    stockfish->send(game);

    if(!humanToPlay()) // Stockfish's turn
        think();
    else // human turn
        updateLegalMoves();
}

void ChessBoardWindow::move(char fromCol, int fromRow, char toCol, int toRow) {
    move(*square(fromCol,fromRow),*square(toCol,toRow));
}

void ChessBoardWindow::castling(Castling type) {
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

    proceedToNextTurn();
}


void ChessBoardWindow::think() {
    stockfish->send("go");
    ui->statusBar->showMessage("Calcul du prochain coup...");
}

void ChessBoardWindow::queryFEN() {
    stockfish->send("d");
}

void ChessBoardWindow::nextTurn() {
    if(turn == Turn::WHITE)
        turn = Turn::BLACK;
    else
        turn = Turn::WHITE;
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
    initBoard();
}

void ChessBoardWindow::info(QString info) {
    //qDebug() << info;
    if(info.contains("score mate 0"))
        gameOver = true;
}

void ChessBoardWindow::receivedFEN(QString f) {
    fen = f;
    qDebug() << "Received fen from Stockfish : " << fen;
}

void ChessBoardWindow::bestMove(QString mv) {
    if(mv.contains("(none)")) { // score mate 0
        gameOver = true;
        proceedToNextTurn();
        return;
    }

    ui->statusBar->showMessage("En attente du prochain coup.");
    mv = mv.section(" ",1,2);
    if(mv.length() >= 5 && mv[4].toLatin1() != ' ') { // pawn promotion
        lastPawnPromotion = mv[4].toLatin1();
        if(turn == Turn::WHITE)
            lastPawnPromotion += ('A' - 'a');
    }
    move(mv[0].toLatin1(),mv[1].digitValue(),mv[2].toLatin1(),mv[3].digitValue());
}

void ChessBoardWindow::receivedLegalMove(QString move) {
    legalMoves.append(move);
}

void ChessBoardWindow::allLegalMovesReceived() {
    lockChessboard = false;
    ui->statusBar->showMessage("En attente du prochain coup.");
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


