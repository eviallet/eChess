#include "square.h"

Square::Square() {

}

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

int Square::getIndex() {
    int column = col-'a';
    return row*8+column;
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
    if(col>='a')
        col-='a';
    return row*8+col;
}

