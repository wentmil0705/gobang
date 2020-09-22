#include "cube.h"
#include <ctime>
#include <cstdlib>
#include <cmath>

extern int SIZE;
extern int **map;

Cube::Cube()
{
    srand(time(0));
    type = rand() % 6 + 1;
    typeforcube();
    isstart = false;
    isrotate = 0;
    for (int i = 0; i < 3; i++)
        color[i] = double(rand()) / RAND_MAX;
}

void Cube::reset()
{
    type = rand() % 6 + 1;
    typeforcube();
    isrotate = false;
    for (int i = 0; i < 3; i++)
        color[i] = double(rand()) / RAND_MAX;
}

Cube::~Cube()
{
    
}

void Cube::typeforcube()
{
    int r;
    switch(type) {
        case 1:
            r = rand() % (SIZE - 2) + 1;
            cube[0][0] = r + 1, cube[0][1] = 1;
            cube[1][0] = r, cube[1][1] = 1;
            cube[2][0] = r, cube[2][1] = 2;
            cube[3][0] = r + 1, cube[3][1] = 0;
            break;
        case 2:
            r = rand() % (SIZE - 4) + 1;
            cube[0][0] = r + 2, cube[0][1] = 2;
            cube[1][0] = r + 1, cube[1][1] = 2;
            cube[2][0] = r, cube[2][1] = 2;
            cube[3][0] = r + 3, cube[3][1] = 2;
            break;
        case 3:
            r = rand() % (SIZE - 2) + 1;
            cube[0][0] = r, cube[0][1] = 1;
            cube[1][0] = r + 1, cube[1][1] = 1;
            cube[2][0] = r, cube[2][1] = 0;
            cube[3][0] = r + 1, cube[3][1] = 2;
            break;
        case 4:
            r = rand() % (SIZE - 3) + 1;
            cube[0][0] = r + 1, cube[0][1] = 2;
            cube[1][0] = r, cube[1][1] = 1;
            cube[2][0] = r, cube[2][1] = 2;
            cube[3][0] = r + 2, cube[3][1] = 2;
            break;
        case 5:
            r = rand() % (SIZE - 3) + 1;
            cube[0][0] = r + 1, cube[0][1] = 2;
            cube[1][0] = r, cube[1][1] = 2;
            cube[2][0] = r + 2, cube[2][1] = 2;
            cube[3][0] = r + 2, cube[3][1] = 1;
            break;
        case 6:
            r = rand() % (SIZE - 2) + 1;
            cube[0][0] = r, cube[0][1] = 2;
            cube[1][0] = r, cube[1][1] = 1;
            cube[2][0] = r + 1, cube[2][1] = 2;
            cube[3][0] = r + 1, cube[3][1] = 1;
            break;
    }
}

int ** Cube::usecube() const
{
    int ** useCube = new int * [4];
    for (int i = 0; i < 4; i++) {
        useCube[i] = new int[2];
        useCube[i][0] = cube[i][0];
        useCube[i][1] = cube[i][1];
    }
    return useCube;
}

int Cube::usetype() const
{
    return type;
}

void Cube::updatecube()
{
    for (int i = 0; i < 4; i++)
        cube[i][1]++;
}

bool Cube::rotatecube()
{
    if (type != 2 && type != 6) {
        // 记录下九方格矩阵
        int temp[3][3] = {0};
        for (int i = 0; i < 4; i++)
            temp[abs(cube[0][0] - cube[i][0] - 1)][abs(cube[0][1] - cube[i][1] - 1)] = 1;
        // 记录旋转后矩阵
        int temp_r[3][3] = {0};
        for (int i = 0; i < 3; i++)
            for (int j = 2; j >= 0; j--)
                temp_r[2 - j][i] = temp[i][j];
        // 判断变形后是否有被占据的情况
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (temp_r[i][j] == 1 && map[cube[0][1] + j - 1][cube[0][0] + i - 1] == 1 && cube[0][1] + j - 1 < 1 && cube[0][0] + j + 1 > SIZE - 1 && cube[0][0] + i - 1 < 1 && cube[0][0] + i + 1 > SIZE - 1)
                    return false;   // 不旋转
        int k = 1;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (!(i == 1 && j == 1) && temp_r[i][j] == 1) {
                    cube[k][0] = cube[0][0] + i - 1;
                    cube[k++][1] = cube[0][1] + j - 1;
                }
        return true;
    }
    else if (type == 2)
    {
        // 记录下25方格矩阵
        int temp[5][5] = {0};
        for (int i = 0; i < 4; i++)
            temp[abs(cube[0][0] - cube[i][0] - 2)][abs(cube[0][1] - cube[i][1] - 2)] = 1;
        // 记录旋转后矩阵
        int temp_r[5][5] = {0};
        for (int i = 0; i < 5; i++)
            for (int j = 4; j >= 0; j--)
                temp_r[4 - j][i] = temp[i][j];
        // 判断变形后是否有被占据的情况
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (temp_r[i][j] == 1 && map[cube[0][1] + j - 2][cube[0][0] + i - 2] == 1 && cube[0][1] + j - 2 > 1 && cube[0][0] + j + 2 < SIZE && cube[0][0] + i - 2 > 1 && cube[0][0] + i + 2 < SIZE)
                    return false;   // 不旋转
        int k = 1;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (!(i == 2 && j == 2) && temp_r[i][j] == 1) {
                    cube[k][0] = cube[0][0] + i - 2;
                    cube[k++][1] = cube[0][1] + j - 2;
                }
        return true;
    }
    else
        return false;
}

void Cube::movecube(char key)
{
    int flag = 0;
    switch(key) {
        case 'a':
            for (int i = 0; i < 4; i++)
                if (map[cube[i][1]][cube[i][0]-1] != 0) {
                    flag = 1;
                    break;
                }
            if (flag == 0) {
                for (int i = 0; i < 4; i++)
                    cube[i][0]--;
            }
            break;
        case 'd':
            for (int i = 0; i < 4; i++)
                if (map[cube[i][1]][cube[i][0]+1] != 0) {
                    flag = 1;
                    break;
                }
            if (flag == 0) {
                for (int i = 0; i < 4; i++)
                    cube[i][0]++;
            }
            break;
        case 'r':
            isrotate = rotatecube();
            break;
        case 's':
            for (int i = 0; i < 4; i++)
                if (map[cube[i][1]+1][cube[i][0]] != 0) {
                    flag = 1;
                    break;
                }
            if (flag == 0) {
                for (int i = 0; i < 4; i++)
                    cube[i][1]++;
            }
            break;
    }
}

void Cube::setstatestart()
{
    isstart = true;
}

bool Cube::isstatestart() const
{
    return isstart;
}

double * Cube::paintcolor()
{
    return color;
}

