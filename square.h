#ifndef SQUARE_H
#define SQUARE_H

#include <QStandardItem>
#include <QVariant>
#include <QString>
#include <QPixmap>
#include <QDebug>

#include "constants.h"

class Square : public QVariant {
    public:
        Square();
        Square(int null);
        Square(QString str);
        Square(const Square &other);
        Square(char col, int row, Piece piece = Piece::NONE);
        QString toString() const;
        void setPiece(Piece piece);
        Piece getPiece() const;
        bool isPieceWhite() const;
        char getPieceFEN() const;
        int c() const;
        int r() const;
        QImage getPic() const;
        static int getSquare(char col, int row);
        inline bool operator==(const Square& other) const {
            return toString()==other.toString();
        }
        inline bool operator!=(const Square& other) const {
            return !operator==(other);
        }
        inline bool operator==(const QString& other) const {
            return toString()==other;
        }
        inline bool operator!=(const QString& other) const {
            return !operator==(other);
        }
        bool isEmpty() const;

        static Piece fenToPiece(char fen);
        static QIcon getPic(Piece piece);
        static QString getPath(Piece piece);
private:
        char col;
        int row;
        Piece piece;
};

#endif // SQUARE_H
