
#include "sdl_handler.hpp"

int main(int argc, char **argv) {

    SDL_HANDLER::init();

    SDL_Window *window = SDL_HANDLER::createWindow();
    SDL_Renderer *renderer = SDL_HANDLER::createRenderer(window);

    SDL_HANDLER::mainLoop(renderer);

    SDL_HANDLER::cleanUp(window, renderer);

    return 0;
}

