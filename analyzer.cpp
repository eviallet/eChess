#include "analyzer.h"

QList<Square>* Analyzer:: getLegalMoves(const Square board[COLS][ROWS], const Square from) {
    QList<Square> moves;
    int b = 1;
    switch(from.getPiece()) {
        /// todo
        case Piece::W_PAWN:
            break;
        case Piece::B_PAWN:
            break;
        case Piece::W_ROOK:
        case Piece::B_ROOK:
            for(int i=from.c()-1; i>=0; i--) {
                if(board[i][from.r()].getPiece()==Piece::NONE)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.c()+1; i<COLS; i++) {
                if(board[i][from.r()].getPiece()==Piece::NONE)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()-1; i>=0; i--) {
                if(board[from.c()][i].getPiece()==Piece::NONE)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()+1; i<ROWS; i++) {
                if(board[from.c()][i].getPiece()==Piece::NONE)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            break;
        //// todo
        case Piece::W_KNIGHT:
        case Piece::B_KNIGHT:
            break;
        case Piece::W_BISHOP:
        case Piece::B_BISHOP:
            b=1;
            while(from.c()+b<COLS&&from.r()+b<ROWS) {
                if(board[from.c()+b][from.r()+b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()+b][from.r()+b]);
                else if(isEnemy(&from,&board[from.c()+b][from.r()+b])) {
                    moves.append(board[from.c()+b][from.r()+b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()+b<COLS&&from.r()-b>=0) {
                if(board[from.c()+b][from.r()-b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()+b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()+b][from.r()-b])) {
                    moves.append(board[from.c()+b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()-b>=0&&from.r()+b<ROWS) {
                if(board[from.c()-b][from.r()+b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()-b][from.r()+b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()+b])) {
                    moves.append(board[from.c()-b][from.r()+b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()-b>=0&&from.r()-b>=0) {
                if(board[from.c()-b][from.r()-b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()-b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()-b])) {
                    moves.append(board[from.c()-b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            break;
        case Piece::W_QUEEN:
        case Piece::B_QUEEN:
            for(int i=from.c()-1; i>=0; i--) {
                if(board[i][from.r()].getPiece()==Piece::NONE)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.c()+1; i<COLS; i++) {
                if(board[i][from.r()].getPiece()==Piece::NONE)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()-1; i>=0; i--) {
                if(board[from.c()][i].getPiece()==Piece::NONE)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()+1; i<ROWS; i++) {
                if(board[from.c()][i].getPiece()==Piece::NONE)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            b=1;
            while(from.c()+b<COLS&&from.r()+b<ROWS) {
                if(board[from.c()+b][from.r()+b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()+b][from.r()+b]);
                else if(isEnemy(&from,&board[from.c()+b][from.r()+b])) {
                    moves.append(board[from.c()+b][from.r()+b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()+b<COLS&&from.r()-b>=0) {
                if(board[from.c()+b][from.r()-b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()+b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()+b][from.r()-b])) {
                    moves.append(board[from.c()+b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()-b>=0&&from.r()+b<ROWS) {
                if(board[from.c()-b][from.r()+b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()-b][from.r()+b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()+b])) {
                    moves.append(board[from.c()-b][from.r()+b]);
                    break;
                } else
                    break;
                b++;
            }
            b=1;
            while(from.c()-b>=0&&from.r()-b>=0) {
                if(board[from.c()-b][from.r()-b].getPiece()==Piece::NONE)
                    moves.append(board[from.c()-b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()-b])) {
                    moves.append(board[from.c()-b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            break;
        /// todo
        case Piece::W_KING:
        case Piece::B_KING:
            break;
        case Piece::NONE:
            break;
    }
    for(int i=0; i<moves.size();i++)
        qDebug() << moves[i].toString();
    return &moves;
}

bool Analyzer::isEnemy(const Square* from, const Square* to) {
    if(from->getPiece()<=Piece::W_KING)
        return to->getPiece()>Piece::W_KING;
    else
        return to->getPiece()<=Piece::W_KING;
}
