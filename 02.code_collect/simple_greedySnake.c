#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DELAY 500000


/**
 * 编译方法
 * gcc test.c -std=c99 -l ucurses
 *
 */

int usleep(useconds_t usec);

int main() {
    int x, y, maxX, maxY; //蛇头的位置和终端窗口的大小
    int direction = KEY_RIGHT; //方向
    int snakeLength = 5; //蛇的长度
    int snakeX[100], snakeY[100]; //蛇身的位置
    int foodX, foodY; //食物的位置
    int score = 0; //得分
    int gameOver = 0; //游戏结束标志

    // 初始化ncurses库
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(0);

    // 获取终端窗口的大小
    getmaxyx(stdscr, maxY, maxX);

    // 初始化蛇的初始位置和长度
    x = maxX / 2;
    y = maxY / 2;
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = x - i;
        snakeY[i] = y;
    }

    // 生成食物的初始位置
    srand(time(NULL));
    foodX = rand() % maxX;
    foodY = rand() % maxY;

    // 游戏循环
    while (!gameOver) {
        clear();

        // 绘制蛇
        for (int i = 0; i < snakeLength; i++) {
            mvprintw(snakeY[i], snakeX[i], "O");
        }

        // 绘制食物
        mvprintw(foodY, foodX, "*");

        // 显示分数
        mvprintw(0, 0, "Score: %d", score);

        // 移动蛇的位置
        int nextX = snakeX[0];
        int nextY = snakeY[0];
        switch (direction) {
            case KEY_UP:
                nextY--;
                break;
            case KEY_DOWN:
                nextY++;
                break;
            case KEY_LEFT:
                nextX--;
                break;
            case KEY_RIGHT:
                nextX++;
                break;
        }

        // 检查是否吃到食物
        if (nextX == foodX && nextY == foodY) {
            score++;
            snakeLength++;
            foodX = rand() % maxX;
            foodY = rand() % maxY;
        } 
        
        // 移动蛇的身体
        for (int i = snakeLength - 1; i > 0; i--) { //后一节移动到前一节的位置
             snakeX[i] = snakeX[i - 1];
             snakeY[i] = snakeY[i - 1];
        }
        

        // 更新蛇头位置
        snakeX[0] = nextX;
        snakeY[0] = nextY;

        // 检查游戏结束条件
        //检查是否越界
        if (nextX < 0 || nextX >= maxX || nextY < 0 || nextY >= maxY) {
            gameOver = 1;
        }
        //检查是否撞到自己的身体
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[i] == nextX && snakeY[i] == nextY) {
                gameOver = 1;
            }
        }

        // 刷新屏幕
        refresh();

        // 延迟一段时间
        usleep(DELAY);
        
        // 获取用户输入
        int key = getch();
        switch (key) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                direction = key;
                break;
            case 'q':
                gameOver = 1;
                break;
        }
    }

    // 清理并退出ncurses库
    endwin();

    printf("Game Over! Your score: %d\n", score);

    return 0;
}
