#include "snake.hh"
#include <cstring>
#include <random>

Snake::Snake(int cols, int rows)
{
    new (this) Snake(cols, rows, 3);
}
Snake::Snake(int cols, int rows, int length = 3)
{
    if (cols <= length)
        cols = length + 1;
    this->cols = cols;
    this->rows = rows;
    this->mapWidth = 2 << (getBits(cols + 1) - 1);

    int mapSize = mapWidth * (rows + 2);

    // this->map = new int8_t[mapSize];
    this->map = (int8_t *)malloc(sizeof(int8_t) * mapSize);

    memset(map, Wall, sizeof(int8_t) * mapSize);
    this->map += this->mapWidth;
    for (int i = 0; i < rows; i++)
    {
        memset(this->map + i * this->mapWidth, Ground, sizeof(int8_t) * cols);
    }

    std::random_device random;
    int y = random() % this->rows;

    this->tail = this->map + y * this->mapWidth;

    this->head = this->tail + (length - 1);

    this->steps[GS_RIGHT] = 1;
    this->steps[GS_LEFT] = -1;
    this->steps[GS_DOWN] = this->mapWidth;
    this->steps[GS_UP] = -this->mapWidth;

    for (int8_t *p = this->tail; p < this->head; ++p)
        *p = this->getStep(GS_RIGHT);

    this->step = this->getStep(GS_RIGHT);

    initRandomApples(length + 1);
    this->applePos = this->getApple();
}

Snake::~Snake()
{
    free(map - this->mapWidth);
    free(apples);
}
int Snake::getBits(int i)
{
    int bits = 0;
    while (i)
    {
        i >>= 1;
        bits++;
    }
    return bits;
}

std::vector<RenderIns> Snake::tick(GS_DIRECTION input)
{
    if (input != GS_NONE)
    {
        GS_STEP newStep = steps[input];
        if (newStep != this->step && newStep != -this->step)
        {
            this->step = newStep;
        }
    }

    int8_t *headDst = this->head + this->step;

    std::vector<RenderIns> res;
    if (*headDst == Apple)
    {
        // eat apple
        res.push_back(this->moveHead(headDst));
        res.push_back(RenderIns(this->getCoordinate(this->tail), this->getCoordinate(this->tail)));

        this->applePos = this->getApple();
        res.push_back(RenderIns(this->applePos, this->applePos));
        return res;
    }
    if (*headDst == Ground)
    {
        // move forward
        res.push_back(this->moveHead(headDst));
        res.push_back(this->dropTail());
        return res;
    }
    // not apple or ground, game over
    return res;
}

RenderIns Snake::moveHead(int8_t *dst)
{
    RenderIns headIns = RenderIns(
        this->getCoordinate(this->head),
        this->getCoordinate(dst));
    *(this->head) = this->step;

    this->head = dst;
    *dst = Head;
    return headIns;
}

RenderIns Snake::dropTail()
{
    int8_t *newTail = this->tail + *(this->tail);
    RenderIns tailIns = RenderIns(
        this->getCoordinate(this->tail),
        this->getCoordinate(newTail));

    *(this->tail) = Ground;

    this->tail = newTail;
    return tailIns;
}

void Snake::initRandomApples(int length)
{
    int total = this->size() - length;
    this->apples = new int[total];
    this->nextApple = this->apples;
    std::random_device random;
    for (int i = 0; total; i++, --total)
        this->apples[i] = random() % total;
}
/**
 * @brief get next apple's coordinates from the pre-generated random apple map
 *
 * @return Vec2c coordinates of the next apple
 */
Vec2c Snake::getApple()
{
    int appleIndex = *(this->nextApple);
    this->nextApple++;
    int8_t *p = this->map;
    for (int y = 0; y < this->rows; ++y)
    {
        for (int x = 0; x < this->cols; ++x)
        {
            // int offset = y * this->mapWidth + x;
            if (*(++p) == Ground && appleIndex-- == 0)
            {
                *p = Apple;
                return Vec2c(x, y);
            }
        }
        p = p - this->cols + this->mapWidth;
    }
    // should never reach here, if everything works fine
    std::cout << *(this->nextApple - 1) << std::endl;
    return Vec2c(-1, -1);
}
int Snake::size()
{
    return this->cols * this->rows;
}

Vec2c Snake::getCoordinate(int offset)
{
    return Vec2c(offset % mapWidth, offset / mapWidth);
}

Vec2c Snake::getCoordinate(int8_t *pointer)
{
    return this->getCoordinate(pointer - this->map);
}

GS_STEP Snake::getStep(GS_DIRECTION dir)
{
    return this->steps[dir];
}
// GS_DIRECTION Snake::getDirection(GS_STEP step)
// {
//     return GS_DIRECTION((step >> 8) | (((~step) * 2) & 2));
// }