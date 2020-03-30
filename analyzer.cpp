#include "analyzer.h"

// todo : en passant
// todo : roque  = !hasPlayerCasteled && !hasPlayerMoved king && !hasPlayerMovedRook && !checkInKingPath
// todo : echec

QList<Square> Analyzer::getLegalMoves(const Square board[COLS][ROWS], const Square from, const Square ignoring) {

    QList<Square> moves;
    int b = 1;
    switch(from.getPiece()) {
    if(!isPinned(board,&from)) {
        // --------------------------------------------------- PAWNS ---------------------------------------------------
        case Piece::W_PAWN:
            if(board[from.c()][from.r()+1].isEmpty()) {
                moves.append(board[from.c()][from.r()+1]);
                if(from.r()==1&&board[from.c()][from.r()+2].isEmpty()) {
                    moves.append(board[from.c()][from.r()+2]);
                    ((Square*)(&board[from.c()][from.r()+2]))->getAnalyzerHandle()->analyzer_isEnpassant = true;
                }
            }
            if(isEnemy(&from,&board[from.c()+1][from.r()+1])) {
                moves.append(board[from.c()+1][from.r()+1]);
                ((Square*)(&board[from.c()+1][from.r()+1]))->getAnalyzerHandle()->analyzer_isEnemy = true;
            }
            if(isEnemy(&from,&board[from.c()-1][from.r()+1]))
                moves.append(board[from.c()-1][from.r()+1]);
            break;
        case Piece::B_PAWN:
            if(board[from.c()][from.r()-1].isEmpty()) {
                moves.append(board[from.c()][from.r()-1]);
                if(from.r()==1&&board[from.c()][from.r()-2].isEmpty())
                    moves.append(board[from.c()][from.r()-2]);
            }
            if(isEnemy(&from,&board[from.c()+1][from.r()-1]))
                moves.append(board[from.c()+1][from.r()-1]);
            if(isEnemy(&from,&board[from.c()-1][from.r()-1]))
                moves.append(board[from.c()-1][from.r()-1]);
             break;

        // --------------------------------------------------- ROOKS ---------------------------------------------------
        case Piece::W_ROOK:
        case Piece::B_ROOK:
            for(int i=from.c()-1; i>=0; i--) {
                if(board[i][from.r()].isEmpty()||board[i][from.r()]==ignoring)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.c()+1; i<COLS; i++) {
                if(board[i][from.r()].isEmpty()||board[i][from.r()]==ignoring)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()-1; i>=0; i--) {
                if(board[from.c()][i].isEmpty()||board[from.c()][i]==ignoring)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()+1; i<ROWS; i++) {
                if(board[from.c()][i].isEmpty()||board[from.c()][i]==ignoring)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            break;
        // --------------------------------------------------- KNIGHTS ---------------------------------------------------
        case Piece::W_KNIGHT:
        case Piece::B_KNIGHT:
            if(from.c()+2<COLS&&from.r()+1<ROWS&&(board[from.c()+2][from.r()+1].isEmpty()||isEnemy(&from,&board[from.c()+2][from.r()+1])))
                moves.append(board[from.c()+2][from.r()+1]);
            if(from.c()+2<COLS&&from.r()-1>=0&&(board[from.c()+2][from.r()-1].isEmpty()||isEnemy(&from,&board[from.c()+2][from.r()-1])))
                moves.append(board[from.c()+2][from.r()-1]);
            if(from.c()+1<COLS&&from.r()+2<ROWS&&(board[from.c()+1][from.r()+2].isEmpty()||isEnemy(&from,&board[from.c()+1][from.r()+2])))
                moves.append(board[from.c()+1][from.r()+2]);
            if(from.c()+1<COLS&&from.r()-2>=0&&(board[from.c()+1][from.r()-2].isEmpty()||isEnemy(&from,&board[from.c()+1][from.r()-2])))
                moves.append(board[from.c()+1][from.r()-2]);
            if(from.c()-1>=0&&from.r()+2<ROWS&&(board[from.c()-1][from.r()+2].isEmpty()||isEnemy(&from,&board[from.c()-1][from.r()+2])))
                moves.append(board[from.c()-1][from.r()+2]);
            if(from.c()-1>=0&&from.r()-2>=0&&(board[from.c()-1][from.r()-2].isEmpty()||isEnemy(&from,&board[from.c()-1][from.r()-2])))
                moves.append(board[from.c()-1][from.r()-2]);
            if(from.c()-2>=0&&from.r()+1<ROWS&&(board[from.c()-2][from.r()+1].isEmpty()||isEnemy(&from,&board[from.c()-2][from.r()+1])))
                moves.append(board[from.c()-2][from.r()+1]);
            if(from.c()-2>=0&&from.r()-1>=0&&(board[from.c()-2][from.r()-1].isEmpty()||isEnemy(&from,&board[from.c()-2][from.r()-1])))
                moves.append(board[from.c()-2][from.r()-1]);
            break;
        // --------------------------------------------------- BISHOPS ---------------------------------------------------
        case Piece::W_BISHOP:
        case Piece::B_BISHOP:
            b=1;
            while(from.c()+b<COLS&&from.r()+b<ROWS) {
                if(board[from.c()+b][from.r()+b].isEmpty()||board[from.c()+b][from.r()+b]==ignoring)
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
                if(board[from.c()+b][from.r()-b].isEmpty()||board[from.c()+b][from.r()-b]==ignoring)
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
                if(board[from.c()-b][from.r()+b].isEmpty()||board[from.c()-b][from.r()+b]==ignoring)
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
                if(board[from.c()-b][from.r()-b].isEmpty()||board[from.c()-b][from.r()-b]==ignoring)
                    moves.append(board[from.c()-b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()-b])) {
                    moves.append(board[from.c()-b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            break;
        // --------------------------------------------------- QUEENS ---------------------------------------------------
        case Piece::W_QUEEN:
        case Piece::B_QUEEN:
            for(int i=from.c()-1; i>=0; i--) {
                if(board[i][from.r()].isEmpty()||board[i][from.r()]==ignoring)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.c()+1; i<COLS; i++) {
                if(board[i][from.r()].isEmpty()||board[i][from.r()]==ignoring)
                    moves.append(board[i][from.r()]);
                else if(isEnemy(&from,&board[i][from.r()])) {
                    moves.append(board[i][from.r()]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()-1; i>=0; i--) {
                if(board[from.c()][i].isEmpty()||board[from.c()][i]==ignoring)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            for(int i=from.r()+1; i<ROWS; i++) {
                if(board[from.c()][i].isEmpty()||board[from.c()][i]==ignoring)
                    moves.append(board[from.c()][i]);
                else if(isEnemy(&from,&board[from.c()][i])) {
                    moves.append(board[from.c()][i]);
                    break;
                } else
                    break;
            }
            b=1;
            while(from.c()+b<COLS&&from.r()+b<ROWS) {
                if(board[from.c()+b][from.r()+b].isEmpty()||board[from.c()+b][from.r()+b]==ignoring)
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
                if(board[from.c()+b][from.r()-b].isEmpty()||board[from.c()+b][from.r()-b]==ignoring)
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
                if(board[from.c()-b][from.r()+b].isEmpty()||board[from.c()-b][from.r()+b]==ignoring)
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
                if(board[from.c()-b][from.r()-b].isEmpty()||board[from.c()-b][from.r()-b]==ignoring)
                    moves.append(board[from.c()-b][from.r()-b]);
                else if(isEnemy(&from,&board[from.c()-b][from.r()-b])) {
                    moves.append(board[from.c()-b][from.r()-b]);
                    break;
                } else
                    break;
                b++;
            }
            break;
    } // if(!isPinned)
        // --------------------------------------------------- KINGS ---------------------------------------------------
        case Piece::W_KING:
        case Piece::B_KING:
            if((from.r()+1<ROWS&&(board[from.c()][from.r()+1].isEmpty()||(isEnemy(&from,&board[from.c()][from.r()+1])&&!isProtected(board, &from))))&&legalDistanceBetweenKings(board,&from,&board[from.c()][from.r()+1])&&!isCheck(board,&board[from.c()][from.r()+1]))
                moves.append(board[from.c()][from.r()+1]);
            if((from.c()+1<COLS&&from.r()+1<ROWS&&(board[from.c()+1][from.r()+1].isEmpty()||(isEnemy(&from,&board[from.c()+1][from.r()+1])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()+1][from.r()+1])&&!isCheck(board,&board[from.c()+1][from.r()+1])))
                moves.append(board[from.c()+1][from.r()+1]);
            if((from.c()-1>=0&&from.r()+1<ROWS&&(board[from.c()-1][from.r()+1].isEmpty()||(isEnemy(&from,&board[from.c()-1][from.r()+1])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()-1][from.r()+1])&&!isCheck(board,&board[from.c()-1][from.r()+1])))
                moves.append(board[from.c()-1][from.r()+1]);
            if((from.r()-1>=0&&(board[from.c()][from.r()-1].isEmpty()||(isEnemy(&from,&board[from.c()][from.r()-1])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()][from.r()-1])&&!isCheck(board,&board[from.c()][from.r()-1])))
                moves.append(board[from.c()][from.r()-1]);
            if((from.c()+1<COLS&&from.r()-1>=0&&(board[from.c()+1][from.r()-1].isEmpty()||(isEnemy(&from,&board[from.c()+1][from.r()-1])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()+1][from.r()-1])&&!isCheck(board,&board[from.c()+1][from.r()-1])))
                moves.append(board[from.c()+1][from.r()-1]);
            if((from.c()-1>=0&&from.r()-1>=0&&(board[from.c()-1][from.r()-1].isEmpty()||(isEnemy(&from,&board[from.c()-1][from.r()-1])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()-1][from.r()-1])&&!isCheck(board,&board[from.c()-1][from.r()-1])))
                moves.append(board[from.c()-1][from.r()-1]);
            if((from.c()+1<COLS&&(board[from.c()+1][from.r()].isEmpty()||(isEnemy(&from,&board[from.c()+1][from.r()])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()+1][from.r()])&&!isCheck(board,&board[from.c()+1][from.r()])))
                moves.append(board[from.c()+1][from.r()]);
            if((from.c()-1>=0&&(board[from.c()-1][from.r()].isEmpty()||(isEnemy(&from,&board[from.c()-1][from.r()])&&!isProtected(board, &from)))&&legalDistanceBetweenKings(board,&from,&board[from.c()-1][from.r()])&&!isCheck(board,&board[from.c()-1][from.r()])))
                moves.append(board[from.c()-1][from.r()]);
            break;
        case Piece::NONE:
            break;
    }
    return moves;
}

bool Analyzer::isEnemy(const Square* from, const Square* to) {
    return from->isPieceWhite() != to->isPieceWhite();
}

bool Analyzer::isCheck(const Square board[COLS][ROWS], const Square* king) {
    QList<Square> enemyPieces;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            if(isEnemy(king,&board[c][r]))
                enemyPieces.append(board[c][r]);
    for(int i=0; i<enemyPieces.size(); i++)
        if(Square::contains(getLegalMoves(board,enemyPieces.at(i)),*king))
            return true;
    return false;
}

bool Analyzer::legalDistanceBetweenKings(const Square board[COLS][ROWS], const Square* from, const Square* king1) {
    Square king2;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++) {
            if(from->getPiece()==Piece::B_KING&&board[c][r].getPiece()==Piece::W_KING) {
                king2 = board[c][r];
                break;
            }
            if(from->getPiece()==Piece::W_KING&&board[c][r].getPiece()==Piece::B_KING) {
                king2 = board[c][r];
                break;
            }
        }

    if(abs(king1->r()-king2.r())<=1&&abs(king1->c()-king2.c())<=1)
        return false;
    return true;
}

bool Analyzer::isPinned(const Square board[COLS][ROWS], const Square* ignoring) {
    QList<Square> enemyPieces;
    Square king;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            if(isEnemy(ignoring,&board[c][r]))
                enemyPieces.append(board[c][r]);
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++) {
            if(ignoring->getPiece()<=Piece::W_KING&&ignoring->getPiece()!=Piece::NONE) {
                if(board[c][r].getPiece()==Piece::W_KING) {
                    king = board[c][r];
                    break;
                }
            } else {
                if(board[c][r].getPiece()==Piece::B_KING&&ignoring->getPiece()!=Piece::NONE) {
                    king = board[c][r];
                    break;
                }
            }
        }
    for(int i=0; i<enemyPieces.size(); i++)
        if(Square::contains(getLegalMoves(board,enemyPieces.at(i)),king))
            return true;
    return false;

}


bool Analyzer::isProtected(const Square board[COLS][ROWS], const Square* from) {
    QList<Square> allyPieces;
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++) {
            if(from->getPiece()<=Piece::W_KING) {
                if(board[c][r].getPiece()!=Piece::NONE&&board[c][r].getPiece()<=Piece::W_KING)
                    allyPieces.append(board[c][r]);
            } else {
                if(board[c][r].getPiece()!=Piece::NONE&&board[c][r].getPiece()>Piece::W_KING)
                    allyPieces.append(board[c][r]);
            }
        }
    QList<Square> allPossibleMoves;
    for(int i=0; i<allyPieces.size(); i++)
        allPossibleMoves.append(getLegalMoves(board,allyPieces.at(i)));
    if(Square::contains(allPossibleMoves,*from))
        return true;
    else
        return false;

}

