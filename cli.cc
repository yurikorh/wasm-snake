#include <cstdio>
#include <termio.h>
#include "snake.hh"
#include <cstdlib>
#include <fcntl.h>
#include <chrono>
#include <string>

#define MOVETO(x, y) printf("\033[%d;%dH", (x), (y))

static Snake snake = Snake(10, 10);

char scanKeyboard()
{
    char input = 0;
    struct termios new_termios;
    struct termios old_termios;
    tcgetattr(0, &old_termios);
    new_termios = old_termios;
    new_termios.c_lflag &= ~ICANON;
    new_termios.c_cc[VTIME] = 0;
    tcgetattr(0, &old_termios);
    new_termios.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_termios);
    input = getchar();
    tcsetattr(0, TCSANOW, &old_termios);
    return input;
}
void input()
{
    char keyCode = scanKeyboard();
    if (keyCode == 'w')
        snake.setStep(snake.getStepUp());
    if (keyCode == 'a')
        snake.setStep(snake.getStepLeft());
    if (keyCode == 's')
        snake.setStep(snake.getStepDown());
    if (keyCode == 'd')
        snake.setStep(snake.getStepRight());
}
void initScreen()
{

    std::string buffer = "";
    for (int i = 0; i <= snake.rows; i++)
    {
        for (int j = 0; j <= snake.cols; j++)
        {
            char state = snake.map[i * snake.mapWidth + j];
            // printf("%d ", state);
            if (state == Wall)
            {
                buffer +=  "I ";
            }
            else if (state == 3)
            {
                buffer += "U ";
            }
            else if (state == Apple)
            {
                buffer += "x ";
            }

            else if (state == Ground)
            {
                buffer += "  ";
            }
            else
            {
                buffer += "O ";
            }
        }
        buffer += '\n';
    }

    system("clear");
    std::cout << buffer;
}

bool mainloop()
{

    auto ris = snake.tick();

    if (ris.size() == 0)
    {
        return false;
    }

    initScreen();
    // MOVETO(ris[0].end.x + 1, ris[0].end.y);
    // printf("\bO");
    // MOVETO(ris[1].start.x + 1, ris[1].start.y);
    // printf("\b ");
    // if(ris.size() > 2)
    // {
    //     MOVETO(ris[2].start.x + 1, ris[2].start.y);
    //     printf("\bx");
    // }
    return true;
}

// get current local time stamp
int64_t getCurrentLocalTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    return tmp.count();

    // return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main()
{
    fcntl(0, F_SETFL, O_NONBLOCK);
    initScreen();
    auto start = getCurrentLocalTimeStamp();
    while (true)
    {
        input();
        time_t now = getCurrentLocalTimeStamp();
        if (now - start > 500)
        {
            start = now;
            bool r = mainloop();
            if (!r)
                break;
        }
    }
    return 0;
}