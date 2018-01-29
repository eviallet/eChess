#ifndef ANALYZER_H
#define ANALYZER_H

#include <QList>
#include "square.h"
#include "chessboardwindow.h"

class Analyzer {
    public:
        static QList<Square>* getLegalMoves(const Square board[COLS][ROWS], const Square from);
    private:
        static bool isEnemy(const Square *from, const Square *to);
};

#endif // ANALYZER_H
