#include "movelist.h"

MoveList::MoveList() {}

void MoveList::clear() {
    moves.clear();
}

void MoveList::append(QString move) {
    char fromCol = move[0].toLatin1() - 'a';
    char fromRow = move[1].toLatin1() - '1';
    char toCol =   move[2].toLatin1() - 'a';
    char toRow =   move[3].toLatin1() - '1';

    Square from(fromCol, fromRow);
    Square to(toCol, toRow);

    moves.append(new Move{from, to, 0, 0, 0, 0, 0});
}

void MoveList::append(Move move) {
    moves.append(new Move{move.from, move.to, 0, 0, 0, 0, 0});
}

void MoveList::analyzeMoves(const Square board[COLS][ROWS]) {
    // ======= CASTLING
    int index = indexOf("e1","g1");
    if(index != -1 && board['e'-'a'][0].getPiece() == Piece::W_KING)
        moves[index]->isCastling = true;
    index = indexOf("e1","c1");
    if(index != -1 && board['e'-'a'][0].getPiece() == Piece::W_KING)
        moves[index]->isCastling = true;
    index = indexOf("e8","c8");
    if(index != -1 && board['e'-'a'][7].getPiece() == Piece::B_KING)
        moves[index]->isCastling = true;
    index = indexOf("e8","g8");
    if(index != -1 && board['e'-'a'][7].getPiece() == Piece::B_KING)
        moves[index]->isCastling = true;
}

MoveList MoveList::movesFrom(Square from) {
    MoveList res;

    for(Move* move : moves)
        if(move->from == from)
            res.append(*move);

    return res;
}

int MoveList::indexOf(QString from, QString to) {
    for(int i = 0; i < moves.size(); i++)
        if(moves[i]->from == from && moves[i]->to == to)
            return i;
    return -1;
}

bool MoveList::contains(Square from, Square to) {
    for(int i = 0; i < moves.size(); i++)
        if(moves[i]->from == from && moves[i]->to == to)
            return true;
    return false;
}

bool MoveList::contains(QString from, Square to) {
    for(int i = 0; i < moves.size(); i++)
        if(moves[i]->from == from && moves[i]->to == to)
            return true;
    return false;
}

bool MoveList::contains(Square to) {
    for(int i = 0; i < moves.size(); i++)
        if(moves[i]->to == to)
            return true;
    return false;
}

int MoveList::size() {
    return moves.size();
}
