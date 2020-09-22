#ifndef CUBE_H_
#define CUBE_H_

class Cube
{
private:
    int cube[4][2];
    int type;
    void typeforcube();
    bool isstart;
    bool rotatecube();
    double color[3];
public:
    Cube();
    ~Cube();
    void reset();
    void updatecube();
    int ** usecube() const;
    int usetype() const;
    void movecube(char key);
    void setstatestart();
    bool isstatestart() const;
    bool isrotate;
    double * paintcolor();
};

#endif
