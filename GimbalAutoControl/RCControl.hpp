//
//  RCControl.hpp
//  Visual
//
//  Created by Summer on 7/13/16.
//  Copyright © 2016 summer. All rights reserved.
//

#ifndef RCControl_hpp
#define RCControl_hpp

#include <stdio.h>
#include <iostream>
#include "DBus.hpp"
#include "Serial.h"
#include "PIDRegulator.hpp"
#include "unistd.h"
class RCControl {
private:
    int lastX, lastY;
    DBusFrame dbusFrame;
    Serial serial;
    PIDRegulator xPID, yPID;
    DBusRawFrame rawFrame;
    
public:
    RCControl();
    void setPoint(int, int);
    void measure(int, int);
    static void *runner(void *self)
    {
        int count;
        RCControl* th = (RCControl *)self;
        while(1)
        {
            th->rawFrame = th->dbusFrame.encode();
            th->serial.Write((char *)th->rawFrame.data, 18);
            ++count;
            if(count%40 == 0)
            {
                std::cout<<th->rawFrame<<std::endl;
                count = 1;
            }
            usleep(7000);
        }
    }
};
#endif /* RCControl_hpp */
