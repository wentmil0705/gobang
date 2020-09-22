#include "cube.h"
#include <GLUT/GLUT.h>
#include <iostream>
#include <cmath>
using std::cout;
int SIZE = 25;
Cube cube;
Cube cuberemind;
int ** map;
double *** mapcolor;
const int TimerInterval = 200;
int POINTS = 0;

// 判断下层是否满消失函数
void lowestlayer()
{
    int flag = 0;
    int * temp = new int[SIZE]{0};
    for (int i = SIZE - 1; i > 2; i--) {
        int isfull = 1;
        for (int j = 1; j < SIZE - 1; j++)
            if (map[i][j] == 0) {
                isfull = 0;
                break;
            }
        if (isfull == 1) {
            temp[flag++] = i;
        }
    }
    POINTS += flag * SIZE;
    int full = 0;
    while (temp[full] != 0) {
        for (int i = temp[full++]; i > 2; i--) {
            for (int j = 1; j <= SIZE - 1; j++)
                map[i][j] = map[i - flag][j];
        }
    }
    delete [] temp;
}

// 判断gameover
void gameover()
{
    for (int j = 1; j < SIZE - 1; j++) {
        if (map[2][j] == 1) {
            cout << "GAME OVER\n";
            exit(EXIT_FAILURE);
        }
    }
}

// 更新图形和坐标函数
void update()
{
    // 判断是否下一个位置已经被占据，占据则标记此时位置被占据
    int ** temp = cube.usecube();
    double * tempcolor = cube.paintcolor();
    for (int i = 0; i < 4; i++) {
        if (map[temp[i][1] + 1][temp[i][0]] == 1) {
            for (int k = 0; k < 4; k++) {
                map[temp[k][1]][temp[k][0]] = 1;
                mapcolor[temp[k][1]][temp[k][0]][0] = tempcolor[0];
                mapcolor[temp[k][1]][temp[k][0]][1] = tempcolor[1];
                mapcolor[temp[k][1]][temp[k][0]][2] = tempcolor[2];
            }
            cube = cuberemind;
            cuberemind.reset();   // 出现固定时则重新生成新的方块
            break;
        }
    }
    // 不被占据则更新坐标，即使对于新产生的也可以让它露面了
    cube.updatecube();
    // 调用判断下层是否满再更新函数
    lowestlayer();
    gameover();
}

// openGL设置
void InitGLScene()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("TERIS BEGIN!");
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, SIZE, SIZE, 0);
}

struct Vertex {
    float x, y, z;
    float r, g, b;
};

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(800 / SIZE);
    
    glBegin(GL_POINTS);
    double * tempcubecolor = cube.paintcolor();
    glColor3f(tempcubecolor[0], tempcubecolor[1], tempcubecolor[2]);
    int ** temp = cube.usecube();
    for (int i = 0; i < 4; i++) {
        // 游戏中
        if (temp[i][1] > 2 && !(temp[i][1] < 7 && temp[i][0] > 0 && temp[i][0] < 5))
            glVertex2f(temp[i][0], temp[i][1]);
    }
    glEnd();
    // 提示框
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string str = "NEXT ONE";
    for (int i = 0; str[i]; i++) {
        glRasterPos2f(0.625 * i + 0.1, 6);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glLineWidth(1.0);
    glColor3f(0.73f, 0.71f, 0.71f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(5, 0);
    glVertex2f(5, 5);
    glVertex2f(0, 5);
    glEnd();
    glBegin(GL_POINTS);
    double * tempcuberemindcolor = cuberemind.paintcolor();
    int ** tempremind = cuberemind.usecube();
    glColor3f(tempcuberemindcolor[0], tempcuberemindcolor[1], tempcuberemindcolor[2]);
    for (int i = 0; i < 4; i++) {
        glVertex2f(fabs(tempremind[i][0] - tempremind[0][0] + 2.4), fabs(tempremind[i][1] - tempremind[0][1] + 2.5));
    }
    for (int i = 1; i <= SIZE - 1; i++)
        for (int j = 1; j <= SIZE - 1; j++)
            if (map[j][i] == 1) {
                glColor3f(mapcolor[j][i][0], mapcolor[j][i][1], mapcolor[j][i][2]);
                glVertex2f(i, j);
            }
    glEnd();
    // 记分
    glColor3f(1.0f, 1.0f, 1.0f);
    str = "POINTS";
    for (int i = 0; str[i]; i++) {
        glRasterPos2f(SIZE - 5 + i * 0.625, 1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
    glRasterPos2f(SIZE - 5 + 1.7 * 0.625, 2);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + POINTS / 100);
    glRasterPos2f(SIZE - 5 + 2.7 * 0.625, 2);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + POINTS / 10);
    glRasterPos2f(SIZE - 5 + 3.7 * 0.625, 2);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + POINTS % 10);
    // 边框
    glColor3f(0.93f, 0.91f, 0.91f);
    glLineWidth(10.0);
    glBegin(GL_LINES);
    glVertex2f(0.1, SIZE);
    glVertex2f(0.1, 6.2);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(5.3, 6.3);
    glVertex2f(0.1, 6.3);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(5.3, 2.3);
    glVertex2f(5.3, 6.4);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(5.2, 2.3);
    glVertex2f(SIZE, 2.3);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(SIZE - 0.1, 2.3);
    glVertex2f(SIZE - 0.1, SIZE);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(SIZE, SIZE - 0.1);
    glVertex2f(0, SIZE - 0.1);
    glEnd();
    glFlush();
}

void timer(int timeid)
{
    if (cube.isrotate == false) {
        update();
    }
    cube.isrotate = false;
    display();
    glutTimerFunc(TimerInterval, timer, 1);
}

void keypress(unsigned char key, int x, int y)
{
    int ** temp = cube.usecube();
    int flag = 0;
    if (key == 'g' && !cube.isstatestart()) {
        cube.setstatestart();
        cuberemind.setstatestart();
        glutTimerFunc(TimerInterval, timer, 1);
    }
    cube.movecube(key);
    if (key == 'q') {
        cout << "GAME END\n";
        exit(EXIT_FAILURE);
    }
}

void InitMap()
{
    map = new int * [SIZE + 1];
    for (int i = 0; i < SIZE + 1; i++)
        map[i] = new int[SIZE + 1]{0};
    for (int i = 0; i < SIZE + 1; i++) {
        map[SIZE][i] = 1;
        map[i][SIZE] = map[i][0] = -1;
    }
    mapcolor = new double ** [SIZE + 1];
    for (int i = 0; i < SIZE + 1; i++)
        mapcolor[i] = new double * [SIZE + 1];
    for (int i = 0; i < SIZE + 1; i++)
        for (int j = 0; j < SIZE + 1; j++)
            mapcolor[i][j] = new double[3]{0};
}

int main(int argc, char **argv)
{
    InitMap();
    glutInit(&argc, argv);
    InitGLScene();
    glutDisplayFunc(display);
    glutKeyboardFunc(keypress);
    glutMainLoop();
}
