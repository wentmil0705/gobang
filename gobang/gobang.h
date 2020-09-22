#ifndef GOBANG_H_
#define GOBANG_H_
enum MODE {WHITE = 0, BLACK = 1, CHOOSE = 2, END};
void judge(int x, int y);
void displaychoice();
bool whowin();
void mymouse(int button, int state, int x, int y);
void display();
void deletegobang(int type);
MODE changemode(MODE m);
void Initgobang();
void InitGLScene();
#endif
