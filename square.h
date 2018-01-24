#ifndef SQUARE_H
#define SQUARE_H

#include <QVariant>
#include <QString>

static enum {
    W_PAWN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_QUEEN,
    W_KING,
    B_PAWN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_QUEEN,
    B_KING
} Piece;

class Square : QVariant {
    public:
        static int getSquare(char col, int row);
        Square(char col, int row, int piece = -1);
        QString toString();
        void setPiece(int piece);
        int getPiece();
        int c();
        int r();
    private:
        char col;
        int row;
        int piece;
};

#endif // SQUARE_H
