#ifndef FEN_H
#define FEN_H

#include <QString>
#include <QList>
#include "square.h"

enum class Turn {
    WHITE,
    BLACK
};

enum class Player {
    HUMAN,
    STOCKFISH
};

enum class Castling {
    W_KINGSIDE,
    W_QUEENSIDE,
    B_KINGSIDE,
    B_QUEENSIDE
};

class FEN {
public:
    FEN();

    void setBoardFEN(QString b);
    Turn nextTurn();
    void setEnpassant(Square sqr);
    void incrementHalfmoveClock();
    void resetHalfmoveClock();

    QString getFEN();

    QString getBoardFEN() const;
    Turn getTurn() const;
    QList<Castling> getCastlings() const;
    int getMovesCount() const;
    int getHalfmovesCount() const;
private:
    void updateInfos();


    QString boardFEN;
    Turn turn;
    QList<Castling> castlings;
    int movesCount;
    int halfmovesCount;
    QString enpassantStr;

    QString fen;
};

#endif // FEN_H
