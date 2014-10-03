//
//  Goal.h
//  simulplay
//
//  Created by Guillem Laborda on 19/09/14.
//
//

#ifndef __simulplay__Goal__
#define __simulplay__Goal__

class Goal {
public:
    Goal() {};
    Goal(int min, int max, int average, int des2, int des3, int color = 0);
    Goal(Goal const&) {};
    Goal& operator=(Goal const&);

    int getMinTime(void);
    void setMinTime(int min);
    int getMaxTime(void);
    void setMaxTime(int max);
    int getAverageTime(void);
    void setAverageTime(int average);
    int getDesviation2Star(void);
    void setDesviation2Star(int des);
    int getDesviation3Star(void);
    void setDesviation3Star(int des);
    int getColorZone(void);
    void setColorZone(int color);
    bool getCompleted(void);
    void setCompleted(bool c);
    int getScore(void);
    void setScore(int s);

private:
    int minTime;
    int maxTime;
    int averageTime;
    int desviation2Star;
    int desviation3Star;
    int colorZone;
    bool completed = false;
    //1, 2, 3 stars
    int score = 0;
};

#endif /* defined(__simulplay__Goal__) */
