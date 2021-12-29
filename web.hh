#include <SDL2/SDL.h>
#include <emscripten.h>
#include <iostream>
#include "snake.hh"
typedef struct
{
    SDL_Renderer *renderer;
    Snake *snake;
    std::vector<RenderIns> ris;
    int start;
    int now;
    int duration;
    float frequency;
} context;

struct Scalar
{
    int r;
    int g;
    int b;
    Scalar(int r, int g, int b) : r(r), g(g), b(b) {}
};