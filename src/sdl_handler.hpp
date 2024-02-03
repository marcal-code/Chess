#pragma once

#include "board.hpp"
#include "chess.hpp"
#include "piece.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

namespace SDL_HANDLER {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;

    const int SQUARE_SIZE = SCREEN_HEIGHT / 8;

    void init();
    void cleanUp(SDL_Window *window, SDL_Renderer *renderer);

    SDL_Window *createWindow();
    SDL_Renderer *createRenderer(SDL_Window *window);

    void mainLoop(SDL_Renderer *renderer);

    // chess game specific functions
    void drawChessBoard(const Board &board, SDL_Renderer *renderer);
    std::string getPieceFileName(Piece piece);
    Square pixelToBoardConverter(int pixel_x, int pixel_y);

    // input handling
    void mouseHandler(SDL_MouseButtonEvent button_event, Board &board);
    int keyboardHandler(SDL_KeyboardEvent keyboard_event, Board &board);

    // menu system
    void displayFog(SDL_Renderer *renderer);

} // namespace SDL_HANDLER
