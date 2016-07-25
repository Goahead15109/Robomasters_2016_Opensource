//
//  PIDRegulator.hpp
//  Visual
//
//  Created by Summer on 7/13/16.
//  Copyright © 2016 summer. All rights reserved.
//

#ifndef PIDRegulator_hpp
#define PIDRegulator_hpp

#include <stdio.h>

class PIDRegulator {
private:
    float kp, ki, kd; //比例系数

    float setPoint; //设定值
    float prevError; // 误差值
    float lastTime; //上次测量时间 ms
    
public:
    PIDRegulator(float p, float i, float d);
    PIDRegulator();
    void setPID(float p, float i, float d);
    void setSetPoint(float pos);
    float setMeasuredValue(float value);
    
};

#endif /* PIDRegulator_hpp */
