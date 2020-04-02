# eChess

The goal of this [Qt](https://www.qt.io/) program is to control a real life chessboard, to play locally against [Stockfish](https://github.com/official-stockfish/Stockfish), a chess engine, or online, on [lichess.org](https://lichess.org/).

This program will later be adapted to work from a Raspberry Pi 3 and WiringPi to control a LED matrix and read a matrix of reed switches.

## Local play

As of today, you can play in front of your computer thanks to the Qt GUI.
The pieces and chessboard tiles are assets from lichess.org.

![screenshot](/GUI.PNG)

You can play as white or black, vs Stockfish or another local player.

## Move validation

Even for human turns, Stockfish computes legal moves and checks for checks.

Legal moves appear green on the UI, king being checked appear red, checkers appear yellow, and selected piece appear blue.

Castling, pawn promotion and en passant are checked manually to sync the GUI with the Stockfish internal board. 

In a second time, this will allow to light the real chessboard accordingly !

## Online play

For now, no Internet play is possible, but it will happen.
I will use [lichess.org Board API](https://lichess.org/api#tag/Board) to play against online players.

In a first time, the realboard player will have to follow light indications to play the online opponent's move.
If everything works, I might add a stepper to move pieces around automatically!

## Setup

To work correctly, stockfish.exe should be on C: root, like : "C:/stockfish.exe"
