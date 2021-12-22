#include "web.hh"

void input(context *ctx)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                ctx->snake->setStep(ctx->snake->getStepUp());
                break;
            case SDLK_DOWN:
                ctx->snake->setStep(ctx->snake->getStepDown());
                break;
            case SDLK_LEFT:
                ctx->snake->setStep(ctx->snake->getStepLeft());
                break;
            case SDLK_RIGHT:
                ctx->snake->setStep(ctx->snake->getStepRight());
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void drawApple(context *ctx, Vec2c pos, Scalar s)
{
    SDL_Rect rect;

    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    rect.x = (pos.x) * 50 ;
    rect.y = (pos.y) * 50;
    rect.w = 50;
    rect.h = 50;
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawBody(context *ctx, int8_t *ptr, Scalar s)
{
    static int8_t lastdir = 0;
    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    Vec2c pos = ctx->snake->getCoordinate(ptr);
    SDL_Rect rect = { pos.x * 50, pos.y * 50, 50, 50 };
    int8_t dir = *ptr;
    if(dir == lastdir)
    {
        if(dir == 1 || dir == -1)
        {
            rect.y += 2;
            rect.h -= 4;
        }
        else
        {
            rect.x += 2;
            rect.w -= 4;
        }
    }
    else
    {
        if(lastdir == -1)
        {
            rect.x += 2;
            rect.w -= 2;
        }
        else if(lastdir == 1)
        {
            rect.w -= 2;
        }
        else if(lastdir < -1)
        {
            rect.y += 2;
            rect.h -= 2;
        }
        else
        {
            rect.h -=2;
        }

        if(dir == -1)
        {
            rect.w -= 2;
        }   
        else if(dir == 1)
        {
            rect.x += 2;
            rect.w -= 2;
        }
        else if(dir < -1)
        {
            rect.h -= 2;
        }
        else
        {
            rect.y += 2;
            rect.h -= 2;
        }
    }
    lastdir = dir;
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawHead(context *ctx, Scalar s0, Scalar s1)
{
    RenderIns renderIns = ctx->ris[0];
    int current = ctx->now - ctx->start;
    int duration = ctx->duration;
    int x = renderIns.start.x * (duration - current);
    x += renderIns.end.x * current;
    x = x * 50 / duration;
    int y = renderIns.start.y * (duration - current);
    y += renderIns.end.y * current;
    y = y * 50 / duration;

    SDL_Rect rect = {x, y, 50, 50};
    SDL_SetRenderDrawColor(ctx->renderer, s0.r, s0.g, s0.b, 255);
    SDL_RenderFillRect(ctx->renderer, &rect);

    int oldX = renderIns.start.x * 50;
    int oldY = renderIns.start.y * 50;
    rect = {oldX, oldY, x - oldX, y - oldY};
    SDL_SetRenderDrawColor(ctx->renderer, s1.r, s1.g, s1.b, 255);
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawTail(context *ctx, Scalar s)
{
    RenderIns renderIns = ctx->ris[1];
    int current = ctx->now - ctx->start;
    int duration = ctx->duration;
    int x = renderIns.start.x * (duration - current);
    x += renderIns.end.x * current;
    x = x * 50 / duration;
    int y = renderIns.start.y * (duration - current);
    y += renderIns.end.y * current;
    y = y * 50 / duration;

    SDL_Rect rect = {x, y, 50, 50};

    if(*(ctx->snake->tail) == 1 || *(ctx->snake->tail) == -1) 
    {
        rect.y += 2;
        rect.h -= 4;
    }
    else
    {
        rect.x += 2;
        rect.w -= 4;
    }
    SDL_SetRenderDrawColor(ctx->renderer, s.r, s.g, s.b, 255);
    SDL_RenderFillRect(ctx->renderer, &rect);
}

void drawSnake(context *ctx)
{
    Scalar bodyColor = Scalar(255, 196, 225);
    Scalar appleColor = Scalar(238, 215, 206);
    Scalar HeadColor = Scalar(255, 135, 202);

    int8_t* ptr = ctx->snake->tail;
    while(ptr != ctx->snake->head)
    {
        drawBody(ctx, ptr, bodyColor);
        // if(*ptr == 1 || *ptr == -1)
        // {
        //     drawRect(ctx, ctx->snake->getCoordinate(ptr), true, bodyColor);
        // }
        // else
        // {
        //     drawRect(ctx, ctx->snake->getCoordinate(ptr), false, bodyColor);    
        // }
        ptr += *ptr;
    }

    drawApple(ctx, ctx->snake->applePos, appleColor);
    
    drawHead(ctx, HeadColor, bodyColor);
    
    drawTail(ctx, bodyColor);
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
    input(ctx);

    if (ctx->now - ctx->start > ctx->duration)
    {
        ctx->start = ctx->now;

        ctx->ris = ctx->snake->tick();
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
    int duration = 175;
    context ctx = {renderer, &snake, Rins, start, start, duration};

    const int simulate_infinite_loop = 1; // call the function repeatedly
    const int fps = 0;                    // call the function as fast as the browser wants to render (typically 60fps)

    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}