//
//  RCControl.cpp
//  Visual
//
//  Created by Summer on 7/13/16.
//  Copyright © 2016 summer. All rights reserved.
//

#include "RCControl.hpp"
#include <stdio.h>

RCControl::RCControl() {
    FILE *fp = fopen("options.env", "r");
    if(!fp)
    {
        printf("file open error.\n");
        return ;
    }
    char buf[16];
    float xp = 1, xi = 1, xd = 1;
    float yp = 1, yi = 1, yd = 1;
    float value;
    char serialName[128] = "/dev/cu.usbserial";
    while(EOF != fscanf(fp, "%s = %f", buf, &value))
    {
        if(!strcmp(buf, "xp"))
        {
            xp = value;
        }
        else if(!strcmp(buf, "xi"))
        {
            xi = value;
        }
        else if(!strcmp(buf, "xd"))
        {
            xd = value;
        }
        else if(!strcmp(buf, "yp"))
        {
            yp = value;
        }
        else if(!strcmp(buf, "yi"))
        {
            yi = value;
        }
        else if(!strcmp(buf, "yd"))
        {
            yd = value;
        }
    }
    xPID.setPID(xp, xi, xd);
    yPID.setPID(yp, yi, yd);
    serial.Open(serialName, 115200, 8, EVEN, 1);
}

void RCControl::setPoint(int x, int y)
{
    xPID.setSetPoint(x);
    yPID.setSetPoint(y);
}

void RCControl::measure(int xMea, int yMea)
{
    float xOut, yOut;
    xOut = (int) xPID.setMeasuredValue(xMea);
    yOut = (int) yPID.setMeasuredValue(yMea);
    dbusFrame.rc.ch2 = 1024 - xOut;
    dbusFrame.rc.ch3 = 1024 + yOut;
}
