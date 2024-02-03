#pragma once

struct Piece {

    enum class Type {
        NONE,
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    } type;

    enum class Color {
        NONE,
        WHITE,
        BLACK
    } color;

    bool operator==(const Piece &piece) const {
        return (type == piece.type && color == piece.color);
    }

    bool operator!=(const Piece &piece) const {
        return (type != piece.type || color != piece.color);
    }
};

// A namespace containing all the pieces on the board of the type 'Piece'
namespace PIECE  {

    const Piece EMPTY_SQUARE = {Piece::Type::NONE, Piece::Color::NONE};
    const Piece WHITE_PAWN = {Piece::Type::PAWN, Piece::Color::WHITE};
    const Piece BLACK_PAWN = {Piece::Type::PAWN, Piece::Color::BLACK};
    const Piece WHITE_KNIGHT = {Piece::Type::KNIGHT, Piece::Color::WHITE};
    const Piece BLACK_KNIGHT = {Piece::Type::KNIGHT, Piece::Color::BLACK};
    const Piece WHITE_BISHOP = {Piece::Type::BISHOP, Piece::Color::WHITE};
    const Piece BLACK_BISHOP = {Piece::Type::BISHOP, Piece::Color::BLACK};
    const Piece WHITE_ROOK = {Piece::Type::ROOK, Piece::Color::WHITE};
    const Piece BLACK_ROOK = {Piece::Type::ROOK, Piece::Color::BLACK};
    const Piece WHITE_QUEEN = {Piece::Type::QUEEN, Piece::Color::WHITE};
    const Piece BLACK_QUEEN = {Piece::Type::QUEEN, Piece::Color::BLACK};
    const Piece WHITE_KING = {Piece::Type::KING, Piece::Color::WHITE};
    const Piece BLACK_KING = {Piece::Type::KING, Piece::Color::BLACK};

} // namespace PIECE
