#include "gobang.h"
#include <iostream>
#include <GLUT/GLUT.h>
#include <cmath>

extern int SIZE;
extern MODE mode;
extern MODE modetemp;
const double window_square_size = 800.0;
extern int ** gobang;
extern int choices[4];
extern int k;
extern int sumrest[2];
extern int sum[2];
//// 调试用
//extern int tempi;
//extern int tempj;
//// -----

void mymouse(int button, int state, int x, int y)
{
    int my_x = int((double(x)  - 800.0 / (SIZE + 2) / 2 ) / (window_square_size / (SIZE + 2))) + 1;
    int my_y = int((double(y)  - 800.0 / (SIZE + 2) / 2 ) / (window_square_size / (SIZE + 2))) + 1;
    if (state == GLUT_DOWN) {
        if (my_x == SIZE + 1 && my_y == SIZE - 1) {
            std::cout << "GAME END!\n";
            exit(EXIT_FAILURE);
        }
        if (my_x == SIZE + 1 && my_y == SIZE - 2) {
            Initgobang();
            display();
        }
        if (mode == WHITE || mode == BLACK) {
            if (my_x < SIZE && my_y < SIZE && my_y > 0 && my_x > 0) {
                if (gobang[my_x][my_y] == 0) {
                    gobang[my_x][my_y] = mode + 1;
                    sumrest[mode]--;
//                    // 调试用
//                    std::cout << "POINTSTATE:\n" << gobang[my_x][my_y] << std::endl;
//                    // -----------
                    judge(my_x, my_y);  //因为调用judge的过程中会出现mode的改变因此需要再做一次判断，可以后期优化一下
                    if (mode == CHOOSE)
                        displaychoice();
                    else
                        display();
                    mode = changemode(mode);
                }
                if (whowin()) {
                    std::cout << "GAME END.";
                    mode = END;
                }
            }
        }
        else {
            if (my_x == SIZE + 1 && my_y == 1) { // 选择水平
//                // 调试用
//                std::cout << "tempi = " << tempi << "\ntempj = " << tempj << "\n\n";
//                // --------
                deletegobang(0);
                mode = changemode(modetemp);
                display();
            }
            else if (my_x == SIZE + 1 && my_y == 2) { // 选择竖直
//                // 调试用
//                std::cout << "tempi = " << tempi << "\ntempj = " << tempj << "\n\n";
//                // --------
                deletegobang(1);
                mode = changemode(modetemp);
                display();
            }
            else if (my_x == SIZE + 1 && my_y == 3) { // 选择斜右
//                // 调试用
//                std::cout << "tempi = " << tempi << "\ntempj = " << tempj << "\n\n";
//                // --------
                deletegobang(2);
                mode = changemode(modetemp);
                display();
            }
            else if (my_x == SIZE + 1 && my_y == 4) { // 选择斜左
//                // 调试用
//                std::cout << "tempi = " << tempi << "\ntempj = " << tempj << "\n\n";
//                // --------
                deletegobang(3);
                mode = changemode(modetemp);
                display();
            }
        }
    }
}

// 画棋子
void drawchess(int x, int y, int type)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    double n = 50;
    float R = 0.3;
    int i = 0;
    glPushMatrix();
    if (type == 1)
        glColor3f(0.8f, 0.8f, 0.8f);
    else
        glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= n; i++)
        glVertex2f((R + 0.2) * cos(2 * 3.1415926 / n * i) + x, (R + 0.2) * sin(2 * 3.1415926 / n * i) + y);
    if (type == 1)
        glColor3f(0.9f, 0.9f, 0.9f);
    else
        glColor3f(0.1f, 0.1f, 0.1f);
    for (int i = 0; i <= n; i++)
        glVertex2f(R * cos(2 * 3.1415926 / n * i) + x, R * sin(2 * 3.1415926 / n * i) + y);
    glEnd();
    glPopMatrix();
}

// 画叉
void drawx(int x, int y)
{
    glLineWidth(5.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.2, y - 0.2);
    glVertex2f(x + 0.2, y + 0.2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x + 0.2, y - 0.2);
    glVertex2f(x - 0.2, y + 0.2);
    glEnd();
    glLineWidth(1.0);
}

void drawtext(float x, float y, char ch)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ch);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.545, 0.47, 0.08);
    glLineWidth(3);
    // 网格
    for (int j = 0; j <= SIZE; j++) {
        glBegin(GL_LINES);
        glVertex2f(0, j);
        glVertex2f(SIZE, j);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(j, 0);
        glVertex2f(j, SIZE);
        glEnd();
    }
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (gobang[i][j] == 1) {
                drawchess(i, j, 1);
            }
            else if (gobang[i][j] == 2) {
                drawchess(i, j, 2);
            }
        }
    glPointSize(window_square_size / SIZE - 20);
    glBegin(GL_POINTS);
    glColor3f(0.7f, 0.6f, 0.5f);
    glVertex2f(SIZE + 1, SIZE - 1);    // END
    glColor3f(0.4f, 0.9f, 0.9f);
    glVertex2f(SIZE + 1, SIZE - 2);    // RETRY
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(SIZE + 1, SIZE - 3);    // 显示白旗的剩余与赢数
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(SIZE + 1, SIZE - 5);    // 显示黑棋剩余与赢数
    if (mode == WHITE) {
        // 调试用
        std::cout << "1MODE: " << mode << "\nMODETEMP: " << modetemp << "\n\n";
        // ---------
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(SIZE + 1, SIZE / 2);
    }
    else if (mode == BLACK) {
        // 调试用
        std::cout << "2MODE: " << mode << "\nMODETEMP: " << modetemp << "\n\n";
        // ---------
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(SIZE + 1, SIZE / 2);
    }
    else {
        // 调试用
        std::cout << "3MODE: " << mode << "\nMODETEMP: " << modetemp << "\n\n";
        // ---------
        if (modetemp == WHITE) {
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(SIZE + 1, SIZE / 2);
        }
        else if (modetemp == BLACK) {
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex2f(SIZE + 1, SIZE / 2);
        }
    }
    glEnd();
//    drawtext(SIZE + 1 - 0.1, SIZE - (1 - 0.07), 'E');
    drawx(SIZE + 1, SIZE - 1);
//    drawtext(SIZE + 1 - 0.1, SIZE - (2 - 0.07), 'R');
    std::string str = "NEW";
    int i = 0;
    while (str[i]) {
        glRasterPos2f(SIZE + 1 + (i - 1.7) * 0.18, SIZE - 2 + 0.1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        i++;
    }
    drawtext(SIZE + 0.5, SIZE - 4, '0' + sumrest[0] / 10);
    drawtext(SIZE + 0.7, SIZE - 4, '0' + sumrest[0] % 10);
    drawtext(SIZE + 1, SIZE - 4, '/');
    drawtext(SIZE + 1.2, SIZE - 4, '0' + sum[0] / 10);
    drawtext(SIZE + 1.4, SIZE - 4, '0' + sum[0] % 10);
    drawtext(SIZE + 0.5, SIZE - 6, '0' + sumrest[1] / 10);
    drawtext(SIZE + 0.7, SIZE - 6, '0' + sumrest[1] % 10);
    drawtext(SIZE + 1, SIZE - 6, '/');
    drawtext(SIZE + 1.2, SIZE - 6, '0' + sum[1] / 10);
    drawtext(SIZE + 1.4, SIZE - 6, '0' + sum[1] % 10);
    glFlush();
}

void displaychoice()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.545, 0.47, 0.08);
    glLineWidth(3);
    // 网格
    for (int j = 0; j <= SIZE; j++) {
        glBegin(GL_LINES);
        glVertex2f(0, j);
        glVertex2f(SIZE, j);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(j, 0);
        glVertex2f(j, SIZE);
        glEnd();
    }
//    // 调试用
//    std::cout << "CHOICES:\n";
//    for (int i = 0; i < 4; i++)
//        std::cout << choices[i] << std::endl;
//    std::cout << "NUM:\n";
//    std::cout << k << "\n\n";
//    // --------
    glPointSize(window_square_size / SIZE - 20);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            if (gobang[i][j] == 1) {
                drawchess(i, j, 1);
            }
            else if (gobang[i][j] == 2) {
                drawchess(i, j, 2);
            }
        }
    glLineWidth(10);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < k; i++) {
        switch(choices[i]) {
            case 0:
                glBegin(GL_LINES);
                glVertex2f(SIZE + 1, 0.5);
                glVertex2f(SIZE + 1, 1.5);
                glEnd();
                break;
            case 1:
                glBegin(GL_LINES);
                glVertex2f(SIZE + 0.5, 2);
                glVertex2f(SIZE + 1.5, 2);
                glEnd();
                break;
            case 2:
                glBegin(GL_LINES);
                glVertex2f(SIZE + 0.5, 3.0);
                glVertex2f(SIZE + 1.5, 2.5);
                glEnd();
                break;
            case 3:
                glBegin(GL_LINES);
                glVertex2f(SIZE + 0.5, 3.5);
                glVertex2f(SIZE + 1.5, 4.0);
                glEnd();
                break;
        }
    }
    glEnd();
    glBegin(GL_POINTS);
    glColor3f(0.7f, 0.6f, 0.5f);
    glVertex2f(SIZE + 1, SIZE - 1);    // END
    glColor3f(0.4f, 0.9f, 0.9f);
    glVertex2f(SIZE + 1, SIZE - 2);    // RETRY
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(SIZE + 1, SIZE - 3);    // 显示白旗的剩余与赢数
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(SIZE + 1, SIZE - 5);    // 显示黑棋剩余与赢数
    if (mode == WHITE || modetemp == WHITE) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(SIZE + 1, SIZE / 2);
    }
    if (mode == BLACK || modetemp == BLACK) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(SIZE + 1, SIZE / 2);
    }
    glEnd();
//    drawtext(SIZE + 1 - 0.1, SIZE - (1 - 0.07), 'E');
//    drawtext(SIZE + 1 - 0.1, SIZE - (2 - 0.07), 'R');
    drawx(SIZE + 1, SIZE - 1);
    std::string str = "NEW";
    int i = 0;
    while (str[i]) {
        glRasterPos2f(SIZE + 1 + (i - 1.7) * 0.18, SIZE - 2 + 0.1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        i++;
    }
    drawtext(SIZE + 0.5, SIZE - 4, '0' + sumrest[0] / 10);
    drawtext(SIZE + 0.7, SIZE - 4, '0' + sumrest[0] % 10);
    drawtext(SIZE + 1, SIZE - 4, '/');
    drawtext(SIZE + 1.2, SIZE - 4, '0' + sum[0] / 10);
    drawtext(SIZE + 1.4, SIZE - 4, '0' + sum[0] % 10);
    drawtext(SIZE + 0.5, SIZE - 6, '0' + sumrest[1] / 10);
    drawtext(SIZE + 0.7, SIZE - 6, '0' + sumrest[1] % 10);
    drawtext(SIZE + 1, SIZE - 6, '/');
    drawtext(SIZE + 1.2, SIZE - 6, '0' + sum[1] / 10);
    drawtext(SIZE + 1.4, SIZE - 6, '0' + sum[1] % 10);
//     调试用
//        glBegin(GL_POINTS);
//    glColor3f(0.7f, 0.6f, 0.5f);
//        for (int i = 0; i < k; i++) {
//            switch(choices[i]) {
//                case 0:
//    glVertex2f(SIZE + 1, 1);
//                    break;
//                case 1:
//    glVertex2f(SIZE + 1, 2);
//                    break;
//                case 2:
//    glVertex2f(SIZE + 1, 3);
//                    break;
//                case 3:
//    glVertex2f(SIZE + 1, 4);
//                    break;
//            }
//        }
//        glEnd();
//    glPointSize(window_square_size / SIZE - 20);
//    glBegin(GL_POINTS);
//    glColor3f(0.7f, 0.6f, 0.5f);
//    glVertex2f(SIZE + 1, 1);
//    glVertex2f(SIZE + 1, 2);
//    glVertex2f(SIZE + 1, 3);
//    glVertex2f(SIZE + 1, 4);
//    glEnd();
    // -------
    glFlush();
}

void InitGLScene()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(window_square_size, window_square_size);
    glutCreateWindow("GOBANG game start!");
    
    glClearColor(1.0, 0.85, 0.725, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, SIZE+2, SIZE+2, 0);
}
