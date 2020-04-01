#ifndef CONSTANTS_H
#define CONSTANTS_H

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

enum class Turn {
    WHITE,
    BLACK
};

enum class Player {
    HUMAN_W,
    HUMAN_B,
    STOCKFISH
};

enum class Castling {
    W_KINGSIDE,
    W_QUEENSIDE,
    B_KINGSIDE,
    B_QUEENSIDE
};

#endif // CONSTANTS_H
