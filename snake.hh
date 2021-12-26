#include <iostream>
#include <vector>
#include <queue>

enum State
{
    Ground = 0,
    Wall = 2,
    Apple = -2,
    Head = 3
};

// 0: right 1: left 2: down 3: up
typedef enum 
{
    GS_NONE = -1,
    GS_RIGHT,
    GS_LEFT,
    GS_DOWN,
    GS_UP,
    
} GS_DIRECTION;
typedef int8_t GS_STEP;

struct Vec2c
{
    Vec2c() : x(0), y(0) {}
    Vec2c(int8_t x, int8_t y) : x(x), y(y) {}
    int8_t x;
    int8_t y;
    friend std::ostream &operator<<(std::ostream &os, const Vec2c v)
    {
        os << static_cast<int>(v.x) << ", " << static_cast<int>(v.y);
        return os;
    }
};

struct RenderIns
{
    RenderIns() : start(Vec2c()), end(Vec2c()) {}
    RenderIns(Vec2c s, Vec2c e) : start(s), end(e) {}
    Vec2c start;
    Vec2c end;
};
class Snake
{
public:
    Snake(int cols, int rows);
    Snake(int cols, int rows, int length);
    ~Snake();
    int8_t *map;
    int8_t cols;
    int8_t rows;
    int8_t mapWidth;

    GS_STEP getStep(GS_DIRECTION dir);
    Vec2c getCoordinate(int offset);
    Vec2c getCoordinate(int8_t *pointer);
    std::vector<RenderIns> tick(GS_DIRECTION input);
    int size();
    int8_t getValueAt(int8_t x, int8_t y);
    Vec2c applePos;
    int8_t *head;
    int8_t *tail;
    // static GS_DIRECTION getDirection(GS_STEP);
    friend std::ostream& operator<<(std::ostream& os, Snake o)
    {
        for(int row = 0; row < o.rows; ++row)
        {
            for(int col = 0; col < o.cols; ++col)
            {
                // os << static_cast<int>(o.map[i * o.mapWidth + j]) << " ";
                os << static_cast<int>(o.map[row * o.mapWidth + col] ) << " ";
            }
            os << std::endl;
        }
        return os;
    }
    
private:
    void initRandomApples(int length);
    int *apples;
    int *nextApple;
    int8_t step;
    
    int getBits(int i);
    Vec2c getApple();
    RenderIns moveHead(int8_t *dst);
    RenderIns dropTail();
    int8_t steps[4];
};
