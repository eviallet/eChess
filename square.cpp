#include "square.h"


Square::Square(char col, int row, int piece) {
    this->col = col+'a';
    this->row = row;
    this->piece = piece;
}

void Square::setPiece(int piece) {
    this->piece = piece;
}

int Square::getPiece() {
    return piece;
}

int Square::c() {
    return col-'a';
}

int Square::r() {
    return row;
}

QString Square::toString() {
    return QString(col).append(QString::number(row));
}

int Square::getSquare(char col, int row) {
    return ;
}
