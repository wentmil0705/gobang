#include <GLUT/glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

int MINENUM = 30;
int **Hide;
int **Surface;
int SIZE = 40;
int flag;
enum MODE {normalmode, flagmode};
int MODE = normalmode;
int **FlagSurface;

// 显示周围所有0
void showallzero(int i, int j)
{
    if (Hide[i][j] != 0 && Hide[i][j] != -1) {
        Surface[i][j] = 1;
        flag++;
        return;
    }
    else if (Hide[i][j] == 0 && i < SIZE && j < SIZE) {
        Surface[i][j] = 1;
        flag++;
        if (i - 1 > 0 && j - 1 > 0 && Surface[i - 1][j - 1] == 0)
            showallzero(i - 1, j - 1);
        if (i - 1 > 0 && Surface[i - 1][j] == 0)
            showallzero(i - 1, j);
        if (i - 1 > 0 && j + 1 < SIZE && Surface[i - 1][j + 1] == 0)
            showallzero(i - 1, j + 1);
        if (j - 1 > 0 && Surface[i][j - 1] == 0)
            showallzero(i, j - 1);
        if (j + 1 < SIZE && Surface[i][j + 1] == 0)
            showallzero(i, j + 1);
        if (i + 1 < SIZE && j - 1 >0 && Surface[i + 1][j - 1] == 0)
            showallzero(i + 1, j - 1);
        if (i + 1 < SIZE && Surface[i + 1][j] == 0)
            showallzero(i + 1, j);
        if (i + 1 < SIZE && j + 1 < SIZE && Surface[i + 1][j + 1] == 0)
            showallzero(i + 1, j + 1);
    }
}

// 画New！
void drawnew(int x, int y)
{
    std::string str = "NEW";
    int i = 0;
    while (str[i]) {
        glRasterPos2f(x + (i - 1.7) * 0.18, y + 0.1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, str[i]);
        i++;
    }
}
// 画雷
void drawmine(int x, int y)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    double n = 50;  // 分段数
    float R = 0.2;
    int i = 0;
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= n; i++)
        glVertex2f(R * cos(2 * 3.1415926 / n * i) + x, R * sin(2 * 3.1415926 / n * i) + y);
    glEnd();
    glPopMatrix();
    for (i = 0; i <= n / 5; i++) {
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f((R + 0.1 )* cos(2 * 3.1415926 / (n / 5) * i) + x , (R + 0.1 ) * sin(2 * 3.1415926 / (n / 5) * i) + y );
        glEnd();
    }
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

// 画旗子
struct Vertex {
    float x, y, z;
    float r, g, b;
};

void drawflags(int x, int y)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y - 0.3);
    glVertex2f(x, y + 0.3);
    glEnd();
    Vertex triangle[] =
    {
        {static_cast<float>(x), static_cast<float>(y - 0.3), 0, 0.7, 0.13, 0.13},
        {static_cast<float>(x), static_cast<float>(y), 0, 0.7, 0.13, 0.13},
        {static_cast<float>(x + 0.3), static_cast<float>(y), 0, 0.7, 0.13, 0.13}
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), triangle);
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), &triangle[0].r);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

void randmine()
{
    std::srand(std::time(0));
    int i = 0, x, y;
    while (i < MINENUM) {
        x = rand() % (SIZE - 1) + 1;
        y = rand() % (SIZE - 1) + 1;
        if (Hide[x][y] != -1) {
            Hide[x][y] = -1;
            i++;
        }
    }
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Hide[i][j] != -1) {
                int temp = 0;
                if (i-1 > 0 && j-1 > 0 && Hide[i-1][j-1] == -1)
                    temp++;
                if (i-1 > 0 && j > 0 && Hide[i-1][j] == -1)
                    temp++;
                if (i-1 > 0 && j+1 < SIZE && Hide[i-1][j+1] == -1)
                    temp++;
                if (i > 0 && j-1 > 0 && Hide[i][j-1] == -1)
                    temp++;
                if (i > 0 && j+1 < SIZE && Hide[i][j+1] == -1)
                    temp++;
                if (i+1 < SIZE && j-1 > 0 && Hide[i+1][j-1] == -1)
                    temp++;
                if (i+1 < SIZE && j > 0 && Hide[i+1][j] == -1)
                    temp++;
                if (i+1 < SIZE && j+1 < SIZE && Hide[i+1][j+1] == -1)
                    temp++;
                Hide[i][j] = temp;
            }
        }
}

void drawtext(int x, int y)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x - 0.1, y + 0.1);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Hide[x][y] + '0');
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(800 / SIZE - 10);
    glBegin(GL_POINTS);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Surface[i][j] == 0) {
                if (MODE == normalmode) {
                    glColor3f(0.9f, 0.9f, 0.9f);
                }
                else
                    glColor3f(0.7f, 0.8f, 0.7f);
                glVertex2f(i, j);
            }
            else {
                if (Hide[i][j] == 0)
                    glColor3f(0.4f, 0.5f, 0.6f);
                else
                    glColor3f(0.2f, 0.3f, 0.4f);
                glVertex2f(i, j);
            }
        }
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(SIZE, SIZE);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(SIZE, SIZE - 1);
    glColor3f(0.7f, 0.6f, 0.3f);    // 正常
    glVertex2f(SIZE, SIZE - 2);
    glColor3f(0.6f, 0.5f, 0.4f);    // 插旗
    glVertex2f(SIZE, SIZE - 3);
    glEnd();
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Surface[i][j] != 0 && Hide[i][j] != 0){
                drawtext(i, j); // 字符书写应在glbegin之外进行
            }
        }
    glColor3f(1.0f, 1.0f, 1.0f);
//    glRasterPos2f(SIZE - 0.1, SIZE + 0.1);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
    drawx(SIZE, SIZE);
//    glRasterPos2f(SIZE - 0.3, SIZE - 0.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
    drawnew(SIZE, SIZE - 1);
//    glRasterPos2f(SIZE - 0.1, SIZE - 1.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
    drawmine(SIZE, SIZE - 2);
//    glRasterPos2f(SIZE - 0.1, SIZE - 2.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
    drawflags(SIZE, SIZE - 3);
    // 插旗显示
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Surface[i][j] == 0 && FlagSurface[i][j] == 1) {
//                glRasterPos2f(i, j);
//                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
                drawflags(i, j);
            }
        }
    glFlush();
}

void displayallmine()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(800 / SIZE - 10);
    glBegin(GL_POINTS);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Hide[i][j] == -1) {
                glColor3f(0.9f, 0.9f, 0.9f);
                glVertex2f(i, j);
            }
            else {
                if (Hide[i][j] == 0)
                    glColor3f(0.4f, 0.5f, 0.6f);
                else
                    glColor3f(0.2f, 0.3f, 0.4f);
                glVertex2f(i, j);
            }
        }
    glColor3f(1.0f, 1.0f, 1.0f);    // 结束
    glVertex2f(SIZE, SIZE);
    glColor3f(1.0f, 1.0f, 0.0f);    // 重刷
    glVertex2f(SIZE, SIZE - 1);
    glColor3f(0.7f, 0.6f, 0.3f);    // 正常
    glVertex2f(SIZE, SIZE - 2);
    glColor3f(0.6f, 0.5f, 0.4f);    // 插旗
    glVertex2f(SIZE, SIZE - 3);
    glEnd();
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Hide[i][j] == -1) {
                drawmine(i, j);
            }
        }
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++)
            if (Hide[i][j] != -1 && Hide[i][j] != 0)
                drawtext(i, j); // 字符书写应在glbegin之外进行
    glColor3f(1.0f, 1.0f, 1.0f);
//    glRasterPos2f(SIZE - 0.1, SIZE + 0.1);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
    drawx(SIZE, SIZE);
//    glRasterPos2f(SIZE - 0.1, SIZE - 0.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
    drawnew(SIZE, SIZE - 1);
//    glRasterPos2f(SIZE - 0.1, SIZE - 1.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
    drawmine(SIZE, SIZE - 2);
//    glRasterPos2f(SIZE - 0.1, SIZE - 2.9);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
    drawflags(SIZE, SIZE - 3);
    // 插旗显示
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Surface[i][j] == 0 && FlagSurface[i][j] == 1) {
//                glRasterPos2f(i, j);
//                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
                drawflags(i, j);
            }
        }
    glFlush();
}

void displaywinmine()
{
    glPointSize(800 / SIZE - 10);
    glBegin(GL_POINTS);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Hide[i][j] == -1) {
                glColor3f(0.9f, 0.9f, 0.9f);
                glVertex2f(i, j);
            }
            else {
                if (Hide[i][j] == 0)
                    glColor3f(0.4f, 0.5f, 0.6f);
                else
                    glColor3f(0.2f, 0.3f, 0.4f);
                glVertex2f(i, j);
            }
        }
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(SIZE, SIZE);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(SIZE, SIZE - 1);
    glColor3f(0.7f, 0.6f, 0.3f);    // 正常
    glVertex2f(SIZE, SIZE - 2);
    glColor3f(0.6f, 0.5f, 0.4f);    // 插旗
    glVertex2f(SIZE, SIZE - 3);
    glEnd();
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Hide[i][j] == -1) {
                drawmine(i, j);
            }
        }
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++)
            if (Hide[i][j] != -1 && Hide[i][j] != 0)
                drawtext(i, j); // 字符书写应在glbegin之外进行
    glColor3f(1.0f, 1.0f, 1.0f);
//    glRasterPos2f(SIZE, SIZE);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
    drawx(SIZE, SIZE);
//    glRasterPos2f(SIZE, SIZE - 1);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
    drawnew(SIZE, SIZE - 1);
//    glRasterPos2f(SIZE, SIZE - 2);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
    drawmine(SIZE, SIZE - 2);
//    glRasterPos2f(SIZE, SIZE - 3);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'FLAG');
    drawflags(SIZE, SIZE - 3);
    // 插旗显示
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 1; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++) {
            if (Surface[i][j] == 0 && FlagSurface[i][j] == 1) {
//                glRasterPos2f(i, j);
//                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'F');
                drawflags(i, j);
            }
        }
    glFlush();
}

void Initmine(){
    Hide = new int *[SIZE + 1]{0};
    Surface = new int *[SIZE + 1]{0};
    FlagSurface = new int *[SIZE + 1]{0};
    for (int i = 0; i < SIZE + 1; i++) {
        Hide[i] = new int[SIZE + 1]{0};
        Surface[i] = new int[SIZE + 1]{0};
        FlagSurface[i] = new int[SIZE + 1]{0};
    }
    flag = 0;
    randmine();
}


int Judgemine(int x, int y)
{
    if (x == SIZE && y == SIZE - 2) {
        MODE = normalmode;
        display();
        return 1;
    }
    if (x == SIZE && y == SIZE - 3) {
        MODE = flagmode;
        display();
        return 1;
    }
    if (x == SIZE && y == SIZE) {
        std::cout << "GAME END!\n";
        exit(EXIT_FAILURE);
    }
    if (x == SIZE && y == SIZE - 1) {
        Initmine();
        display();
        return 1;
    }
    if (MODE == normalmode) {
        if (Hide[x][y] == -1) {
            displayallmine();
            return 0;
        }
        if (Hide[x][y] == 0)
            showallzero(x, y);
        if (Surface[x][y] != 1) {
            Surface[x][y] = 1;
            flag++;
        }
        if ((SIZE - 1) * (SIZE - 1) - flag == MINENUM) {
            std::cout << "WIN!\n";
            displaywinmine();
            return 1;
        }
    }
    else {
        if (FlagSurface[x][y] == 0)
            FlagSurface[x][y] = 1;
        else
            FlagSurface[x][y] = 0;
    }
    display();
    return 2;
}

void InitGLScene()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Minesweeper game start!");
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, SIZE+1, SIZE+1, 0);
}

void mymouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN)
            Judgemine(int((double(x) - 800.0 / (SIZE) / 2 ) / (800.0 / (SIZE + 1))) + 1, int((double(y) - 800.0 / (SIZE) / 2) / (800.0 / (SIZE + 1))) + 1);
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN)
            Judgemine(int((double(x) - 800.0 / (SIZE) / 2 ) / (800.0 / (SIZE + 1))) + 1, int((double(y) - 800.0 / (SIZE) / 2) / (800.0 / (SIZE + 1))) + 1);
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    InitGLScene();
    Initmine();
//    for (int i = 1; i < SIZE; i++) {
//        for (int j = 1; j < SIZE; j++)
//            std::cout << Hide[i][j] << "\t";
//        std::cout << "\n";
//    }
    glutDisplayFunc(display);
    glutMouseFunc(mymouse);
    glutMainLoop();
    return 0;
}
