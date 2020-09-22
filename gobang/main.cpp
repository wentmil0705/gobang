#include "gobang.h"
#include <GLUT/glut.h>

int tempi;
int tempj;
int ** gobang;
int SIZE = 20;
int * sum;  // 记录赢了的琪数 0 为白，1 为黑
int * sumrest;  // 记录还有多少可用棋 0 为白 1 为黑
MODE mode;
MODE modetemp;


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    InitGLScene();
    Initgobang();
    glutDisplayFunc(display);
    glutMouseFunc(mymouse);
    glutMainLoop();
    return 0;
}
