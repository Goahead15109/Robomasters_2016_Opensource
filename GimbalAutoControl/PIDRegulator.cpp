//
//  PIDRegulator.cpp
//  Visual
//
//  Created by Summer on 7/13/16.
//  Copyright © 2016 summer. All rights reserved.
//

#include "PIDRegulator.hpp"
#include <time.h>

float getTimeMs()
{
    return (float)clock() / CLOCKS_PER_SEC * 1000;
}

PIDRegulator::PIDRegulator(float p, float i, float d)
{
    kp = p;
    ki = i;
    kd = d;

    setPoint = 0;
    lastTime = getTimeMs();
}
PIDRegulator::PIDRegulator()
{
    setPoint = 0;
    lastTime = getTimeMs();
}
void PIDRegulator::setPID(float p, float i, float d)
{
    kp = p;
    ki = i;
    kd = d;
}

void PIDRegulator::setSetPoint(float pos)
{
    setPoint = pos;
}

float PIDRegulator::setMeasuredValue(float measuredValue)
{
    float curTime = getTimeMs();
    float dt = curTime - lastTime; //时间差
    float error, output;
    float integral; // 积分变量
    float derivative; // 微分变量
    lastTime = curTime;
    
    error = setPoint - measuredValue;
    integral = integral + error * dt / 1000;
    derivative = (error - prevError)/dt * 1000;
    output = kp * error + ki * integral + kd * derivative;
    prevError = error;
    
    return output;
}