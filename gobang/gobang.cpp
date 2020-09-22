#include "gobang.h"
#include <iostream>
extern int tempi;
extern int tempj;
extern int ** gobang;
extern MODE mode;
extern int sum[2];
extern int SIZE;
extern MODE modetemp;
extern int sumrest[2];
int choices[4] = {0};
int k = 0;

void deletegobang(int type)
{
    int i = tempi;
    int j = tempj;
    int jud_val = 0;
    if (mode == WHITE)
        jud_val = 1;
    else if (mode == BLACK)
        jud_val = 2;
    else
    {
        if (modetemp == WHITE)
            jud_val = 1;
        else if (modetemp == BLACK)
            jud_val = 2;
    }
    switch(type) {
        case 0: // 水平
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (j < SIZE)
                    j++;
            }
            if (tempj > 0)
                j = tempj - 1;
            i = tempi;
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (j > 0)
                    j--;
            }
            break;
        case 1: // 竖直
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (i < SIZE)
                    i++;
            }
            if (tempi > 0)
                i = tempi - 1;
            j = tempj;
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (i > 0)
                    i--;
            }
            break;
        case 2: // 斜左
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (i > 0 && j < SIZE)
                    j++, i--;
            }
            if (i < SIZE && j > 0)
                i = tempi + 1, j = tempj - 1;
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (i < SIZE && j > 0)
                    j--, i++;
            }
            break;
        case 3: // 斜右
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (j < SIZE && i < SIZE)
                    j++, i++;
            }
            if (j > 0 && i > 0)
                i = tempi - 1, j = tempj - 1;
            while (gobang[i][j] == jud_val) {
                gobang[i][j] = 0;
                sum[jud_val - 1]++;
                if (j > 0 && i > 0)
                    j--,i--;
            }
            break;
    }
}

void dealwithpos()
{
    if (k > 1) {
        modetemp = mode;
        mode = CHOOSE;
    }
    else if (k == 1) {
        deletegobang(choices[0]);
    }
}

void judge(int i, int j)
{
    int flag[4] = {0};
    int jud_val = 0;
    if (mode == WHITE)
        jud_val = 1;
    else if (mode == BLACK)
        jud_val = 2;
    tempi = i;
    tempj = j;
    // 水平方向
    while (gobang[i][j] == jud_val) {
        flag[0]++;
        if (j < SIZE)
            j++;
    }
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[0]++;
        if (j > 0)
            j--;
    }
    flag[0]--;  // 因为i，j位置实际上算多了一次
    // 竖直方向
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[1]++;
        if (i < SIZE)
            i++;
    }
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[1]++;
        if (i > 0)
            i--;
    }
    flag[1]--;
    // 斜左方向
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[2]++;
        if (i > 0 && j < SIZE)
            j++, i--;
    }
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[2]++;
        if (i < SIZE && j > 0)
            j--, i++;
    }
    flag[2]--;
    // 斜右方向
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[3]++;
        if (j < SIZE && i < SIZE)
            j++, i++;
    }
    i = tempi;
    j = tempj;
    while (gobang[i][j] == jud_val) {
        flag[3]++;
        if (j > 0 && i > 0)
            j--,i--;
    }
    flag[3]--;
    
    k = 0;  // 记录选择的数目
    choices[0] = choices[1] = choices[2] = choices[3] = 0;
    // 记录下可供消失的选择
    for (int i = 0; i < 4; i++) {
        if (flag[i] >= 5)
            choices[k++] = i;
    }
//    // 调试用
//    std::cout << mode << "\n";
//    for (int i = 0; i < 4; i++)
//        std::cout << flag[i] << "\n";
//    std::cout << std::endl;
    
    // 调用处理函数
    dealwithpos();
}

bool whowin()
{
//    // 调试用
//    std::cout << "WHITE WIN: " << sum[0] << "\nWHITE REST: " << sumrest[0] << "\nBLACK WIN: " << sum[1] << "\nBLACK REST: " << sumrest[1] << "\n\n";
//    // ---------
    int count0 = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (gobang[i][j] == 0)
                count0++;
    if (sumrest[0] == 0 || sumrest[1] == 0 || count0 == 0) {
        if (sum[1] == sum[0])
            std::cout << "TIE!\n";
        else if (sum[1] > sum[0])
            std::cout << "BLACK WIN!\n";
        else if (sum[1] < sum[0])
            std::cout << "WHITE WIN!\n";
        return true;
    }
    return false;
}

MODE changemode(MODE m)
{
    if (m == WHITE)
        return BLACK;
    else if (m == BLACK)
        return WHITE;
    else
        return CHOOSE;
}

void Initgobang()
{
    gobang = new int * [SIZE + 3];
    for (int i = 0; i < SIZE + 3; i++)
        gobang[i] = new int [SIZE + 3]{0};
    mode = WHITE;
    modetemp = WHITE;
    sum[0] = sum[1] = 0;
    sumrest[0] = sumrest[1] = 20;
}
