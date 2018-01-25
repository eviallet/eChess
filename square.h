#ifndef SQUARE_H
#define SQUARE_H

#include <QVariant>
#include <QString>

static enum {
    NONE,
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

class Square : public QVariant {
    public:
        static int getSquare(char col, int row);
        Square();
        Square(char col, int row, int piece = NONE);
        QString toString();
        void setPiece(int piece);
        int getPiece();
        int c();
        int r();
        int getIndex();
    private:
        char col;
        int row;
        int piece;
};

#endif // SQUARE_H
