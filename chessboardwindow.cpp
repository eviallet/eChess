#include "chessboardwindow.h"
#include "ui_chessboardwindow.h"

// TODO save/load FEN
// TODO indicateur de tour

ChessBoardWindow::ChessBoardWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChessBoardWindow) {
    ui->setupUi(this);

    /// board setup
    boardView = ui->board;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            board[c][r] = Square(c,r);

    /// stockfish setup
    stockfish = new Stockfish(this);

    /// overlay setup for legal moves
    overlayCircle = new QImage(CIRCLE_SIZE, CIRCLE_SIZE, QImage::Format_ARGB32);
    setupOverlay(overlayCircle, Qt::green);

    /// overlay setup for last click
    overlaySquare = new QImage(SQUARE_SIZE, SQUARE_SIZE, QImage::Format_ARGB32);
    setupOverlay(overlaySquare, Qt::blue, true, 0.15);

    /// overlay setup for check
    overlayCheck = new QImage(SQUARE_SIZE, SQUARE_SIZE, QImage::Format_ARGB32);
    setupOverlay(overlayCheck, Qt::red, true, 0.55);

    /// overlay setup for checkers
    overlayCheckers = new QImage(CIRCLE_SIZE, CIRCLE_SIZE, QImage::Format_ARGB32);
    setupOverlay(overlayCheckers, Qt::yellow, false, 0.75);

    /// model setup
    model = new BoardModel();
    model->insertColumns(0,8);
    model->insertRows(0,8);
    whitePlyr = Player::HUMAN_W;
    blackPlyr = Player::STOCKFISH;

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
    connect(ui->actionDifficulte,&QAction::triggered,[&] {
        bool ok;
        int dif = QInputDialog::getInt(this, "Choisir la difficulté", "Difficulté (1350-2850)", 1350, 1350, 2850, 1, &ok);
        if(ok)
            setDifficulty(dif);
    });
    connect(ui->actionJoueurs,&QAction::triggered,[&] {
        bool ok;
        QString item = QInputDialog::getItem(this, "Choisir les joueurs", "Blanc / Noir",
        {"Humain / Humain", "Humain / PC", "PC / Humain", "PC / PC"}, 1, false, &ok);
        if(ok) {
            if(item == "Humain / Humain") {
                whitePlyr = Player::HUMAN_W;
                blackPlyr = Player::HUMAN_B;
            }
            else if(item == "Humain / PC") {
                whitePlyr = Player::HUMAN_W;
                blackPlyr = Player::STOCKFISH;
            }
            else if(item == "PC / Humain") {
                whitePlyr = Player::STOCKFISH;
                blackPlyr = Player::HUMAN_B;
            }
            else { // item == "PC / PC"
                whitePlyr = Player::STOCKFISH;
                blackPlyr = Player::STOCKFISH;
            }

            if(!humanToPlay())
                think();
            else
                updateLegalMoves();
        }
    });

    connect(boardView,SIGNAL(clicked(QModelIndex)),this,SLOT(clicked(QModelIndex)));
    connect(stockfish,SIGNAL(bestMove(QString)),this,SLOT(bestMove(QString)));
    connect(stockfish,SIGNAL(info(QString)),this,SLOT(info(QString)));
    connect(stockfish,SIGNAL(uciok()),this,SLOT(uciok()));
    connect(stockfish,SIGNAL(readyok()),this,SLOT(readyok()));
    connect(stockfish,SIGNAL(move(QString)),this,SLOT(receivedLegalMove(QString)));
    connect(stockfish,SIGNAL(endMove(int)),this,SLOT(allLegalMovesReceived(int)));
    connect(stockfish,SIGNAL(fen(QString)),this,SLOT(receivedFEN(QString)));
    connect(stockfish,SIGNAL(check(QString)),this,SLOT(receivedCheckers(QString)));


    stockfish->send("isready");
}

void ChessBoardWindow::setupOverlay(QImage* overlay, QColor color, bool isRect, qreal alpha) {
    overlay->fill(Qt::transparent);
    QPainter painter(overlay);
    painter.setBrush(color);
    painter.setOpacity(alpha);
    if(isRect)
        painter.drawRect(0,0,overlay->size().width()-1,overlay->size().height()-1);
    else
        painter.drawEllipse(0,0,overlay->size().width()-1,overlay->size().height()-1);
    painter.end();
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
    checkersSquares.clear();
    turn = Turn::WHITE;
    enpassant = "";
    lastClick = "";

    setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    stockfish->send(game);

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

            // draw red rect under king if in check
            if(checkersSquares.size()>0 &&
                    ((turn == Turn::WHITE && board[c][r].getPiece() == Piece::W_KING) ||
                     (turn == Turn::BLACK && board[c][r].getPiece() == Piece::B_KING)))
                painter.drawImage(overlay.width()/2-SQUARE_SIZE/2, overlay.height()/2-SQUARE_SIZE/2, *overlayCheck);

            // draw blue rect under current selection
            if(!lastClick.isEmpty() && board[c][r] == lastClick)
                painter.drawImage(overlay.width()/2-SQUARE_SIZE/2, overlay.height()/2-SQUARE_SIZE/2, *overlaySquare);

            // draw pieces
            if(board[c][r].getPiece()!=Piece::NONE)
                painter.drawImage(0, 0, board[c][r].getPic().scaled(SQUARE_SIZE,SQUARE_SIZE));

            // draw orange circle on checkers
            if(checkersSquares.contains(board[c][r]))
                painter.drawImage(overlay.width()/2-CIRCLE_SIZE/2, overlay.height()/2-CIRCLE_SIZE/2, *overlayCheckers);

            // draw blue circle on legal moves
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

    if(lastClick.isEmpty() && !selected.isEmpty()) { // first click on a piece
        lastClick = selected.toString();
        updateLegalMovesShown(selected);
    }
    else if(!lastClick.isEmpty() && selected != lastClick) { // if, after selecting a piece, we selected another square
        if(legalMoves.contains(lastClick, selected)) { // if this is a legal move, then move : lastClick -> selected
            legalMovesShown.clear();
            Square from(lastClick);
            lastClick = "";
            move(from, selected);
        }
        else if(!selected.isEmpty()) { // if this is not a legal move, we selected another piece
            lastClick = selected.toString();
            updateLegalMovesShown(selected);
        }
    }
    else { // if, after selecting a piece, we selected the same one, clear lastClick (ie releasing pawn at the same spot)
        lastClick = "";
        updateLegalMovesShown();
    }
}

void ChessBoardWindow::move(char fromCol, int fromRow, char toCol, int toRow) {
    move(*square(fromCol,fromRow),*square(toCol,toRow));
}

void ChessBoardWindow::move(Square from, Square to) {
    board[to.c()][to.r()].setPiece(board[from.c()][from.r()].getPiece());
    board[from.c()][from.r()].setPiece(Piece::NONE);

    lastFrom = board[from.c()][from.r()];
    lastTo = board[to.c()][to.r()];

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

    // EN PASSANT
    else if(!enpassant.isEmpty()) {
        if(lastTo == enpassant) {
            if(turn == Turn::WHITE && lastTo.getPiece() == Piece::W_PAWN)
                board[lastTo.c()][lastTo.r()-1].setPiece(Piece::NONE);
            else if(turn == Turn::BLACK && lastTo.getPiece() == Piece::B_PAWN)
                board[lastTo.c()][lastTo.r()+1].setPiece(Piece::NONE);
        }

        enpassant = "";
        proceedToNextTurn();
    }

    // ELSE
    else
        proceedToNextTurn();
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

void ChessBoardWindow::pawnPromotionChosen(char p) {
    lockChessboard = false;
    lastPawnPromotion = p;

    board[lastTo.c()][lastTo.r()].setPiece(Square::fenToPiece(p));
    proceedToNextTurn();
}

void ChessBoardWindow::proceedToNextTurn() {
    if(gameOver) {
        ui->statusBar->showMessage("Partie terminée.");
        lockChessboard = true;
        return;
    }

    if(checkersSquares.size()>0)
        checkersSquares.clear();

    nextTurn();

    QString lastMove = lastFrom.toString() + lastTo.toString();
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
///                            STOCKFISH QUERIES
/// =========================================================================

void ChessBoardWindow::think() {
    stockfish->send("go");
    ui->statusBar->showMessage("Calcul du prochain coup...");
}

void ChessBoardWindow::queryFEN() {
    stockfish->send("d");
}

void ChessBoardWindow::setDifficulty(int dif) {
    stockfish->setELO(dif);
}

void ChessBoardWindow::updateLegalMoves() {
    lockChessboard = true;
    ui->statusBar->showMessage("Calcul des coups légaux...");

    legalMoves.clear();

    stockfish->send("go perft 1");
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

void ChessBoardWindow::allLegalMovesReceived(int n) {
    if(n == 0) {
        gameOver = true;
        proceedToNextTurn();
        return;
    }
    queryFEN();
}

void ChessBoardWindow::receivedFEN(QString f) {
    fen = f;
    QStringList details = fen.split(" ");
    if(details.at(3) != "-")
        enpassant = details.at(3);
}

void ChessBoardWindow::receivedCheckers(QString c) {
    if(c.length()>1) {
        QStringList checkers = c.split(" ");
        for(QString chk : checkers)
            checkersSquares.append(Square(chk));
        updateBoard();
    }

    lockChessboard = false;
    ui->statusBar->showMessage("En attente du prochain coup.");
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


