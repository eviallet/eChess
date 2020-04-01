#ifndef MOVELIST_H
#define MOVELIST_H

#include "square.h"
#include "constants.h"
#include <QVector>
#include <QDebug>

typedef struct {
    Square from;
    Square to;
    bool isEnpassant;
    bool isEnemy;
    bool isCastling;
    bool isPawnPromotion;
    bool isPawnMove;
} Move;

class MoveList  {
public:
    MoveList();

    void append(Move move);
    void append(QString move);
    void clear();
    void analyzeMoves(const Square board[COLS][ROWS]);

    MoveList movesFrom(Square from);

    bool contains(Square from, Square to);
    bool contains(Square to);
    int size();
private slots:
    int indexOf(QString from, QString to);
private:
    QVector<Move*> moves;
};

#endif // MOVELIST_H
