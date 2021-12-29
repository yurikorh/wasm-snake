#include "web.hh"

GS_DIRECTION input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            int32_t code = event.key.keysym.sym - SDLK_RIGHT;
            if (code >= 0 && code < 4)
            {
                // 0: right 1: left 2: down 3: up
                return GS_DIRECTION(code);
            }
        }
    }

    return GS_NONE;
}

void drawApple(context *ctx, Vec2c pos, Scalar s)
{
    SDL_Rect rect;

    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    rect.x = (pos.x) * 50;
    rect.y = (pos.y) * 50;
    rect.w = 50;
    rect.h = 50;
    SDL_RenderFillRect(ctx->renderer, &rect);
}

SDL_Rect rectRight(Vec2c pos)
{
    SDL_Rect rect = {pos.x * 50 + 2, pos.y * 50 + 2, 50, 50 - 4};
    return rect;
}
SDL_Rect rectLeft(Vec2c pos)
{
    SDL_Rect rect = {pos.x * 50 - 2, pos.y * 50 + 2, 50, 50 - 4};
    return rect;
}
SDL_Rect rectDown(Vec2c pos)
{
    SDL_Rect rect = {pos.x * 50 + 2, pos.y * 50 + 2, 50 - 4, 50};
    return rect;
}
SDL_Rect rectUp(Vec2c pos)
{
    SDL_Rect rect = {pos.x * 50 + 2, pos.y * 50 - 2, 50 - 4, 50};
    return rect;
}
GS_DIRECTION getDirection(GS_STEP step)
{
    // return GS_DIRECTION((step >> 8) | (((~step) * 2) & 2));
    return GS_DIRECTION((step > 0 ? 0 : 1) + (step % 2 == 0 ? 2 : 0));
}
SDL_Rect (*rectGens[4])(Vec2c pos) = {rectRight, rectLeft, rectDown, rectUp};
void drawBody(context *ctx, int8_t *ptr, Scalar s)
{
    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    Vec2c pos = ctx->snake->getCoordinate(ptr);
    SDL_Rect rect = rectGens[getDirection(*ptr)](pos);
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawHead(context *ctx, Scalar s0)
{
    RenderIns renderIns = ctx->ris[0];
    int current = ctx->now - ctx->start;
    int duration = ctx->duration;
    int x = renderIns.start.x * (duration - current);
    x += renderIns.end.x * current;
    // x = x * 50 / duration;
    x = x * 50 * ctx->frequency;

    int y = renderIns.start.y * (duration - current);
    y += renderIns.end.y * current;
    // y = y * 50 / duration;
    y = y * 50 * ctx->frequency;

    SDL_Rect rect = {x, y, 50, 50};
    SDL_SetRenderDrawColor(ctx->renderer, s0.r, s0.g, s0.b, 255);
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawTail(context *ctx, Scalar s)
{
    RenderIns renderIns = ctx->ris[1];
    int current = ctx->now - ctx->start;
    int duration = ctx->duration;
    int x = renderIns.start.x * (duration - current);
    x += renderIns.end.x * current;
    // x = x * 50 / duration;
    x = x * 50 * ctx->frequency;

    int y = renderIns.start.y * (duration - current);
    y += renderIns.end.y * current;
    // y = y * 50 / duration;
    y = y * 50 * ctx->frequency;

    SDL_Rect rect = {x, y, 50, 50};

    if (renderIns.start.y == renderIns.end.y)
    {
        rect.y += 2;
        rect.h -= 4;
        rect.w += 2;
        if (renderIns.start.x > renderIns.end.x)
            rect.x -= 2;
    }
    else
    {
        rect.x += 2;
        rect.w -= 4;
        rect.h += 2;
        if (renderIns.start.y > renderIns.end.y)
            rect.y -= 2;
    }
    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawSnake(context *ctx)
{
    const Scalar bodyColor = Scalar(255, 196, 225);
    const Scalar appleColor = Scalar(238, 215, 206);
    const Scalar HeadColor = Scalar(255, 135, 202);

    for(int8_t *ptr = ctx->snake->tail; ptr != ctx->snake->head; ptr += *ptr)
    {
        drawBody(ctx, ptr, bodyColor);
    }

    drawApple(ctx, ctx->snake->applePos, appleColor);

    drawHead(ctx, HeadColor);
    // if(ctx->ris.size() == 2)
    {
        drawTail(ctx, bodyColor);
    }
    
}
void draw(context *ctx)
{
    SDL_SetRenderDrawColor(ctx->renderer, 234, 234, 234, 255);
    SDL_RenderClear(ctx->renderer);

    drawSnake(ctx);
    SDL_RenderPresent(ctx->renderer);
}

void gameOver(context *ctx)
{
}

void mainloop(void *arg)
{
    context *ctx = (context *)arg;
    // std::cout << "Mat data: " << *(ctx->snake) << std::endl;
    ctx->now = SDL_GetTicks();

    if (ctx->now - ctx->start > ctx->duration)
    {
        ctx->start = ctx->now;
        GS_DIRECTION in = input();
        ctx->ris = ctx->snake->tick(in);
    }
    if (ctx->ris.size() == 0)
    {
        gameOver(ctx);
        // draw(ctx);
    }
    else
    {
        draw(ctx);
    }
}

int main()
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    int cols = 15;
    int rows = 15;
    SDL_CreateWindowAndRenderer(cols * 50, rows * 50, 0, &window, &renderer);

    Snake snake = Snake(cols, rows);

    std::vector<RenderIns> Rins = std::vector<RenderIns>();
    int start = SDL_GetTicks();
    int duration = 150;
    float frequency = 1.0f / duration;
    context ctx = {renderer, &snake, Rins, start, start, duration, frequency};

    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = 0;                    // call the function as fast as the browser wants to render (typically 60fps)

    std::cout << "Snake version 0.1.0" << std::endl;
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}