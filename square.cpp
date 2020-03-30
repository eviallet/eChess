#include "square.h"

/// =========================================================================
///                                 CONSTUCTORS
/// =========================================================================

Square::Square() : QVariant() {

}

Square::Square(int null) : QVariant() {
    this->col = null;
    this->row = null;
    this->piece = Piece::NONE;
}

Square::Square(const Square &other) : QVariant() {
    this->col = other.c()+'a';
    this->row = other.r();
    this->piece = other.getPiece();
}

Square::Square(char col, int row, Piece piece) : QVariant() {
    this->col = col+'a';
    this->row = row;
    this->piece = piece;
}

/// =========================================================================
///                                  PIECE
/// =========================================================================

void Square::setPiece(Piece piece) {
    this->piece = piece;
}

Piece Square::getPiece() const {
    return piece;
}

bool Square::isPieceWhite() const {
    return piece <= Piece::W_KING;
}

char Square::getPieceFEN() const {
    char p;

    switch(piece) {
    case Piece::NONE:
        return 0;
    case Piece::B_KING:
    case Piece::W_KING:
        p = 'k'; break;
    case Piece::B_ROOK:
    case Piece::W_ROOK:
        p = 'r'; break;
    case Piece::B_PAWN:
    case Piece::W_PAWN:
        p = 'p'; break;
    case Piece::B_KNIGHT:
    case Piece::W_KNIGHT:
        p = 'n'; break;
    case Piece::B_BISHOP:
    case Piece::W_BISHOP:
        p = 'b'; break;
    case Piece::B_QUEEN:
    case Piece::W_QUEEN:
        p = 'q'; break;
    }

    if(isPieceWhite())
        p += ('A' - 'a'); // toUpper

    return p;
}

Piece Square::fenToPiece(char fen) {
    switch(fen) {
    case 'k': return Piece::B_KING;
    case 'K': return Piece::W_KING;
    case 'r': return Piece::B_ROOK;
    case 'R': return Piece::W_ROOK;
    case 'p': return Piece::B_PAWN;
    case 'P': return Piece::W_PAWN;
    case 'n': return Piece::B_KNIGHT;
    case 'N': return Piece::W_KNIGHT;
    case 'b': return Piece::B_BISHOP;
    case 'B': return Piece::W_BISHOP;
    case 'q': return Piece::B_QUEEN;
    case 'Q': return Piece::W_QUEEN;
    default:  return Piece::NONE;
    }
}

bool Square::isEmpty() const {
    return piece==Piece::NONE;
}

int Square::c() const {
    return col-'a';
}

QImage Square::getPic() const {
    switch(piece) {
        case Piece::W_PAWN:
            return QImage(":/pieces/WP");
        case Piece::W_ROOK:
            return QImage(":/pieces/WR");
        case Piece::W_KNIGHT:
            return QImage(":/pieces/WN");
        case Piece::W_BISHOP:
            return QImage(":/pieces/WB");
        case Piece::W_QUEEN:
            return QImage(":/pieces/WQ");
        case Piece::W_KING:
            return QImage(":/pieces/WK");
        case Piece::B_PAWN:
            return QImage(":/pieces/BP");
        case Piece::B_ROOK:
            return QImage(":/pieces/BR");
        case Piece::B_KNIGHT:
            return QImage(":/pieces/BN");
        case Piece::B_BISHOP:
            return QImage(":/pieces/BB");
        case Piece::B_QUEEN:
            return QImage(":/pieces/BQ");
        case Piece::B_KING:
            return QImage(":/pieces/BK");
        default:
            return QImage();
    }
}

/// =========================================================================
///                         BOARD COORDINATES (COL, ROW)
/// =========================================================================

int Square::r() const {
    return row;
}

QString Square::toString() const {
    return QString(col).append(QString::number(row+1));
}

int Square::getSquare(char col, int row) {
    if(col>='a')
        col-='a';
    return row*8+col;
}

/// =========================================================================
///                           ANALYZER FUNCTIONS
/// =========================================================================


bool Square::contains(const QList<Square> list, const Square s) {
    for(int i=0; i<list.size(); i++)
        if(list.at(i)==s)
            return true;
    return false;
}

AnalyzerHandle* Square::getAnalyzerHandle() {
    return &analyzerHandle;
}

void Square::clearAnalyzerHandle() {
    analyzerHandle = {0,0,0,0,0};
}


