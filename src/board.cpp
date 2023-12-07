
#include "board.hpp"

void Board::fenReader(const std::string &fenString) {

    int file = 0;
    int rank = 0;

    for(auto ch : fenString) {

        // new file
        if (ch == '/') {
            rank++;
            file = 0;
            continue;
        } 

        // empty spaces
        else if (isdigit(ch)) {

            int empty_squares = static_cast<int>(ch - '0');

            for (int j = 0; j < empty_squares; j++) {

                board[rank][file] = PIECE::EMPTY_SQUARE;
                file++;
            }
        } 

        // piece on the square
        else {
            
            Piece::Color color = std::isupper(ch) ? Piece::Color::WHITE : Piece::Color::BLACK;
            ch = std::tolower(ch);

            Piece::Type type;
            switch (ch)
            {
            case 'p':
                type = Piece::Type::PAWN;
                break;
            case 'n':
                type = Piece::Type::KNIGHT;
                break;
            case 'b':
                type = Piece::Type::BISHOP;
                break;
            case 'r':
                type = Piece::Type::ROOK;
                break;
            case 'q':
                type = Piece::Type::QUEEN;
                break;
            case 'k':
                type = Piece::Type::KING;
                break;
            default:
                break;
            }
            
            board[rank][file] = {type, color};
            file++;
        }
    }
}

void Board::setSelection(Square square) {

    selected_piece = square;
}

void Board::resetSelection() {

    selected_piece = {-1, -1};
}

Piece::Color Board::getTurn() const {

    return turn;
}

bool Board::isSquareSelected() const {

    return (selected_piece.rank != -1 && selected_piece.file != -1);
}

Square Board::getSelectedSquare() const {

    return selected_piece;
}

Piece Board::getPieceAt(Square square) const {

    return (board[square.rank][square.file]);
};

void Board::movePiece(Square from, Square to) {

    Piece PieceToMove = getPieceAt(from);

    board[from.rank][from.file] = PIECE::EMPTY_SQUARE;

    board[to.rank][to.file] = PieceToMove;
}

void Board::changeTurn() {

    if(turn == Piece::Color::WHITE) {
        turn = Piece::Color::BLACK;
        return;
    }

     if (turn == Piece::Color::BLACK) {
        turn = Piece::Color::WHITE;
        return;
     }
}