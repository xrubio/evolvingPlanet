//
//  Position.h
//  simulplay
//
//  Created by Guillem Laborda on 18/08/14.
//
//

#ifndef __simulplay__Position__
#define __simulplay__Position__

class Position {
public:
    Position(int init_x = 0, int init_y = 0);
    Position(Position const&) {};
    Position& operator=(Position const&);

    void setPosition(int init_x, int init_y);
    int getX() const;
    int getY() const;

private:
    int x;
    int y;
};

#endif /* defined(__simulplay__Position__) */
