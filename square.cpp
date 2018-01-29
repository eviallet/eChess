#include "square.h"

Square::Square() {

}

Square::Square(const Square &other) {
    this->col = other.c()+'a';
    this->row = other.r();
    this->piece = other.getPiece();
}

Square::Square(char col, int row, Piece piece) {
    this->col = col+'a';
    this->row = row;
    this->piece = piece;
}

void Square::setPiece(Piece piece) {
    this->piece = piece;
}

Piece Square::getPiece() const {
    return piece;
}

int Square::c() const {
    return col-'a';
}

int Square::r() const {
    return row;
}

QString Square::toString() const {
    return QString(col).append(QString::number(row+1));
}

int Square::getSquare(char col, int row) {
    if(col>='a')
        col-='a';
    return row*8+col;
}

QVariant Square::getPic() const {
    switch(piece) {
        case Piece::W_PAWN:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WP")));
        case Piece::W_ROOK:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WR")));
        case Piece::W_KNIGHT:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WN")));
        case Piece::W_BISHOP:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WB")));
        case Piece::W_QUEEN:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WQ")));
        case Piece::W_KING:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/WK")));
        case Piece::B_PAWN:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BP")));
        case Piece::B_ROOK:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BR")));
        case Piece::B_KNIGHT:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BN")));
        case Piece::B_BISHOP:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BB")));
        case Piece::B_QUEEN:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BQ")));
        case Piece::B_KING:
            return QVariant(QPixmap::fromImage(QImage(":/pieces/BK")));
        default:
            return QVariant();
    }
}

