#ifndef SQUARE_H
#define SQUARE_H

#include <QStandardItem>
#include <QVariant>
#include <QString>
#include <QPixmap>

#define ROWS 8
#define COLS 8

enum class Piece {
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
};

class Square : public QVariant {
    public:
        Square();
        Square(const Square &other);
        Square(char col, int row, Piece piece = Piece::NONE);
        QString toString() const;
        void setPiece(Piece piece);
        Piece getPiece() const;
        int c() const;
        int r() const;
        QVariant getPic() const;
        static int getSquare(char col, int row);
        inline bool operator==(const Square& other) { // inline = function declared entirely in class .h ~ friend : can be accessed from outside class
            return col==other.c()&&row==other.r()&&piece==other.getPiece();
        }
        inline bool operator!=(const Square& other) {
            return !operator==(other);
        }
        inline bool operator==(const QString& other) {
            return toString()==other;
        }
        inline bool operator!=(const QString& other) {
            return !operator==(other);
        }
    private:
        char col;
        int row;
        Piece piece;
};

#endif // SQUARE_H
