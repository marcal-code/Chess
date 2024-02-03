#pragma once

#include "piece.hpp"

#include <string>

// Location of a square on the board
struct Square {

    int rank;
    int file; 

    bool operator==(const Square &square) {
        return (rank == square.rank && file == square.file);
    }

    bool operator!=(const Square &square) {
        return (rank != square.rank || file != square.file);
    }
};

constexpr int BOARD_SIZE = 8;

class Board {

  public:

    // converts a fen string into a position on the boards
    void fenReader(const std::string &fenString);

    bool isSquareSelected() const;
    void setSelection(Square square);
    Square getSelectedSquare() const;
    void resetSelection();

    Piece::Color getTurn() const;
    void changeTurn();

    Piece getPieceAt(Square square) const;

    void movePiece(Square from, Square to);

    void flip_board();
    bool isFlipped() const;

  private:
    // the current state of the board
    Piece board[BOARD_SIZE][BOARD_SIZE];

    // highlighted piece on the board
    Square selected_piece = {-1, -1};

    // keeps track of whose turn is it
    Piece::Color turn = Piece::Color::WHITE;

    // the orientation of the board
    bool is_flipped = false;
};
