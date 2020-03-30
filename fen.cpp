#include "fen.h"

FEN::FEN() : turn(Turn::WHITE), movesCount(1), halfmovesCount(0), enpassantStr("-") {
    castlings.append(Castling::W_KINGSIDE);
    castlings.append(Castling::B_KINGSIDE);
    castlings.append(Castling::W_QUEENSIDE);
    castlings.append(Castling::B_QUEENSIDE);
}

void FEN::setBoardFEN(QString b) {
    boardFEN = b;
}

QString FEN::getFEN() {
    updateInfos();
    return fen;
}

void FEN::incrementHalfmoveClock() {
    halfmovesCount++;
}

void FEN::resetHalfmoveClock() {
    halfmovesCount = 0;
}

Turn FEN::nextTurn() {
    if(turn == Turn::WHITE)
        turn = Turn::BLACK;
    else {
        turn = Turn::WHITE;
        movesCount++;
    }
    enpassantStr = "-";
    return turn;
}

void FEN::setEnpassant(Square sqr) {
    enpassantStr = sqr.toString();
}

void FEN::updateInfos() {
    QString turnStr, castlingStr;

    // ======= TURNS
    switch (turn) {
    case Turn::WHITE:
        turnStr = "w";
        break;
    case Turn::BLACK:
        turnStr = "b";
        break;
    }

    // ======= CASTLING
    if(castlings.contains(Castling::W_KINGSIDE))
        castlingStr += "K";
    if(castlings.contains(Castling::W_QUEENSIDE))
        castlingStr += "Q";
    if(castlings.contains(Castling::B_KINGSIDE))
        castlingStr += "k";
    if(castlings.contains(Castling::B_QUEENSIDE))
        castlingStr += "q";
    if(castlingStr.isEmpty())
        castlingStr = "-";


    // ======= BUILDING FEN
    fen = QString("%1 %2 %3 %4 %5 %6")
            .arg(boardFEN, turnStr, castlingStr, enpassantStr, QString::number(movesCount), QString::number(halfmovesCount));
}

QString FEN::getBoardFEN() const {
    return boardFEN;
}

Turn FEN::getTurn() const {
    return turn;
}

QList<Castling> FEN::getCastlings() const {
    return castlings;
}

int FEN::getMovesCount() const {
    return movesCount;
}

int FEN::getHalfmovesCount() const {
    return halfmovesCount;
}


