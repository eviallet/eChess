#ifndef SQUARE_H
#define SQUARE_H

#include <QStandardItem>
#include <QVariant>
#include <QString>
#include <QPixmap>
#include <QDebug>

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

typedef struct {
    bool analyzer_isEnpassant; // true if a pawn jumps 2 rows
    bool analyzer_isEnemy; // true if a player can take another player piece, resulting in a "x" in the PGN
    bool analyzer_isCastling; // true if a move here is a castling (for kings only)
    bool analyzer_isNecessaryForCastling; // true for king and rooks : if one of them is moved, castling possibilities for the player will be recalculated
    bool analyzer_isPawnPromotion; // true if it is a square in front or diagonal of a pawn reaching the row before the last
} AnalyzerHandle;

class Square : public QVariant {
    public:
        Square();
        Square(int null);
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

        AnalyzerHandle* getAnalyzerHandle();
        void clearAnalyzerHandle();
public slots:
        static bool contains(const QList<Square> list, const Square s);
        static Piece fenToPiece(char fen);
private:
        char col;
        int row;
        Piece piece;

        AnalyzerHandle analyzerHandle = {0,0,0,0,0};
};

#endif // SQUARE_H
