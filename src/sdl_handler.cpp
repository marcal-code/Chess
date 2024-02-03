
#include "sdl_handler.hpp"

#include "board.hpp"
#include "chess.hpp"
#include "piece.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

void SDL_HANDLER::init() {

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
}

void SDL_HANDLER::cleanUp(SDL_Window *window, SDL_Renderer *renderer) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

SDL_Window *SDL_HANDLER::createWindow() {

    SDL_Window *window = SDL_CreateWindow("Chess v2.0", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, 0);
    if (window == nullptr) {
        std::cerr << "Failed to create a window: " << SDL_GetError() << std::endl;
    }

    return window;
}

SDL_Renderer *SDL_HANDLER::createRenderer(SDL_Window *window) {

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        std::cerr << "Failed to create Renderer: " << SDL_GetError() << std::endl;
    }

    return renderer;
}

void SDL_HANDLER::mainLoop(SDL_Renderer *renderer) {

    Board board;

    // board.fenReader("r3k2r/pp1n2pp/2p2q2/b2p1n2/BP1Pp3/P1N2P2/2PB2PP/R2Q1RK1");
    // board.fenReader("kbK5/pp6/1P6/8/8/8/8/R7");
    // board.fenReader("6k1/5p2/1p5p/p4Np1/5q2/Q6P/PPr5/3R3K");

    board.fenReader("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    SDL_Event event;

    bool gameOver = false;

    while (SDL_WaitEvent(&event)) {

        if (event.type == SDL_QUIT) break;

        // updating stuff
        if (event.type == SDL_MOUSEBUTTONDOWN && !gameOver) {
            mouseHandler(event.button, board);
        }

        if (event.type == SDL_KEYUP) {

            if (keyboardHandler(event.key, board) == -1) break;
        }

        if (Chess::isInCheckMate(board, Piece::Color::BLACK) && !gameOver) {
            gameOver = true;
            std::cout << "White Wins!";
        }

        if (Chess::isInCheckMate(board, Piece::Color::WHITE) && !gameOver) {
            gameOver = true;
            std::cout << "Black Wins!";
        }

        // drawing stuff
        SDL_RenderClear(renderer);
        drawChessBoard(board, renderer);

        if (gameOver) displayFog(renderer);

        SDL_RenderPresent(renderer);
    }
}

void SDL_HANDLER::drawChessBoard(const Board &board, SDL_Renderer *renderer) {

    // Loops over all the squares on the board from top left
    // to bottom right displaying the appropriate square and piece

    // Set the blend mode for the renderer to enable transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int rank = 0; rank < BOARD_SIZE; rank++) {

        for (int file = 0; file < BOARD_SIZE; file++) {

            // the current Square
            SDL_Rect current_square = {file * SQUARE_SIZE, rank * SQUARE_SIZE,
                                       SQUARE_SIZE, SQUARE_SIZE};

            // the background color of a square (dark or light)
            if ((rank + file) % 2) {
                // the dark square color
                SDL_SetRenderDrawColor(renderer, 119, 104, 193, 255);
            } else {
                // the light square color
                SDL_SetRenderDrawColor(renderer, 255, 255, 240, 255);
            }

            SDL_RenderFillRect(renderer, &current_square);

            // deals with selection on the board
            Square selected_square = board.getSelectedSquare();

            if (board.isSquareSelected() && selected_square == Square{rank, file}) {

                // color of square upon its selection
                SDL_SetRenderDrawColor(renderer, 2, 204, 214, 150);
                SDL_RenderFillRect(renderer, &current_square);
            }

            // deals with legal moves indicator
            if (board.isSquareSelected()) {

                bool isValid = Chess::isValidMove(board, selected_square, Square{rank, file});

                if (isValid) {

                    SDL_SetRenderDrawColor(renderer, 59, 66, 82, 100);
                    SDL_RenderFillRect(renderer, &current_square);
                }
            }

            // deals with the rendering of the piece on the square if any
            Piece piece = board.getPieceAt(Square{rank, file});

            if (piece != PIECE::EMPTY_SQUARE) {

                std::string piece_file_path = getPieceFileName(piece);

                SDL_Surface *piece_image = IMG_Load(piece_file_path.c_str());

                if (piece_image == nullptr) {

                    std::cerr << "Could not open file: " << piece_file_path;
                    std::cerr << "\nReason: " << SDL_GetError() << "\n";
                }

                SDL_Texture *piece_texture = SDL_CreateTextureFromSurface(renderer, piece_image);

                SDL_RenderCopy(renderer, piece_texture, NULL, &current_square);

                SDL_DestroyTexture(piece_texture);
                SDL_FreeSurface(piece_image);
            }
        }
    }
}

std::string SDL_HANDLER::getPieceFileName(Piece piece) {

    std::string file_path = "./res/";

    std::string color = (piece.color == Piece::Color::WHITE) ? "w" : "b";

    std::string piece_type;

    switch (piece.type) {

    case Piece::Type::PAWN:
        piece_type = "Pawn";
        break;
    case Piece::Type::KNIGHT:
        piece_type = "Knight";
        break;
    case Piece::Type::BISHOP:
        piece_type = "Bishop";
        break;
    case Piece::Type::ROOK:
        piece_type = "Rook";
        break;
    case Piece::Type::QUEEN:
        piece_type = "Queen";
        break;
    case Piece::Type::KING:
        piece_type = "King";
        break;
    default:
        std::cerr << "Invalid Piece!\n";
        break;
    }

    std::string extention = ".png";

    file_path = file_path + color + piece_type + extention;

    return file_path;
}

void SDL_HANDLER::mouseHandler(SDL_MouseButtonEvent mouse_event, Board &board) {

    // Right click
    if (mouse_event.button == SDL_BUTTON_RIGHT) {

        // for now the right click just resets the selection
        board.resetSelection();
        return;
    }

    // Left click
    if (mouse_event.button == SDL_BUTTON_LEFT) {

        Square clicked_square = pixelToBoardConverter(mouse_event.x, mouse_event.y);
        Piece clicked_piece = board.getPieceAt(clicked_square);

        if (!board.isSquareSelected()) {

            // only add selection to the square if the piece on it belong to
            // the player whose turn it currently is

            if (clicked_piece.color == board.getTurn()) {

                board.setSelection(clicked_square);
            } else {

                board.resetSelection();
            }

            return;
        }

        else {

            // move the piece from the selected square to the clicked square
            // only if the clicked square is a legal square that the selected
            // piece can move to

            bool isValid = Chess::isValidMove(board, board.getSelectedSquare(), clicked_square);

            if (isValid) {
                
                board.movePiece(board.getSelectedSquare(), clicked_square);
                board.changeTurn();
            }

            board.resetSelection();
            return;
        }
    }
}

int SDL_HANDLER::keyboardHandler(SDL_KeyboardEvent keyboard_event,
                                 Board &board) {

    SDL_Keycode key_pressed = keyboard_event.keysym.sym;

    if (key_pressed == SDLK_ESCAPE) return -1;

    if (key_pressed == SDLK_f) {

        board.flip_board();
        board.resetSelection();
    }

    return 0;
}

Square SDL_HANDLER::pixelToBoardConverter(int pixel_x, int pixel_y) {

    Square square;
    square.file = pixel_x / SQUARE_SIZE;
    square.rank = pixel_y / SQUARE_SIZE;

    return square;
}

void SDL_HANDLER::displayFog(SDL_Renderer *renderer) {

    SDL_Rect screen;

    screen.h = SCREEN_HEIGHT;
    screen.w = SCREEN_WIDTH;
    screen.x = 0;
    screen.y = 0;

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    SDL_RenderFillRect(renderer, &screen);
}
