#ifndef ANALYZER_H
#define ANALYZER_H

#include <QList>
#include <QString>
#include <math.h>
#include "square.h"
#include "chessboardwindow.h"

class Analyzer {
    public slots:
        static QList<Square> getLegalMoves(const Square board[COLS][ROWS], const Square from, Square ignoring = Square(-1));
    private slots:
        static bool isPinned(const Square board[COLS][ROWS] , const Square* ignoring);
        static bool isEnemy(const Square *from, const Square *to);
        static bool kingCanMove(const Square *from, const Square *to);
        static bool isProtected(const Square board[COLS][ROWS], const Square *from);
        static bool legalDistanceBetweenKings(const Square board[COLS][ROWS], const Square *from, const Square *king1);
        static bool isCheck(const Square board[COLS][ROWS], const Square *king);
};

#endif // ANALYZER_H
