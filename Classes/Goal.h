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
    Goal(){};
    Goal(int min, int max, int average, int color = 0);
    Goal(Goal const&) {};
    Goal& operator=(Goal const&);
    
    int getMinTime(void);
    void setMinTime(int min);
    int getMaxTime(void);
    void setMaxTime(int max);
    int getAverageTime(void);
    void setAverageTime(int average);
    int getColorZone(void);
    void setColorZone(int color);

private:
    int minTime;
    int maxTime;
    int averageTime;
    int colorZone;
};

#endif /* defined(__simulplay__Goal__) */
