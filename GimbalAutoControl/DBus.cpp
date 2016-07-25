//
//  DBUS.cpp
//  Visual
//
//  Created by Summer on 5/22/16.
//  Copyright © 2016 summer. All rights reserved.
//

#include "DBus.hpp"
#include <string.h>
#include <iostream>
#include <iomanip>

using namespace std;

    DBusRawFrame::DBusRawFrame()
    {
        memset(data, 0, sizeof(data));
    }
    std::ostream& operator<<(std::ostream &os, const Control & frame)
    {
        os << "ch0: " << frame.ch0 << "\t";
        os << "ch1: " << frame.ch1 << "\t";
        os << "ch2: " << frame.ch2 << "\t";
        os << "ch3: " << frame.ch3 << endl;
        os << "s1: " << (int)frame.s1 << "\t";
        os << "s2: " << (int)frame.s2 ;
        return os;
    }
    std::ostream& operator<<(std::ostream &os, const Mouse & frame)
    {
        os << "x:" << frame.x << "\t";
        os << "y:" << frame.y << "\t";
        os << "z:" << frame.z << endl;
        os << "l:" << (int) frame.lbutton << "\t";
        os << "r:" << (int) frame.rbutton ;
        return os;
    }
    
    std::ostream& operator<<(std::ostream &os, const Keyboard & frame)
    {
        
        if((frame.value & Keyboard::KEY_W) > 0)
            os << "W";
        else
            os << "N";
        
        if((frame.value & Keyboard::KEY_A) > 0)
            os << "A";
        else
            os << "N";
        
        if((frame.value & Keyboard::KEY_S) > 0)
            os << "S";
        else
            os << "N";
        
        
        if((frame.value & Keyboard::KEY_D) > 0)
            os << "D";
        else
            os << "N";
        
        if((frame.value & Keyboard::KEY_Q) > 0)
            os << "Q";
        else
            os << "N";
        
        
        if((frame.value & Keyboard::KEY_E) > 0)
            os << "E";
        else
            os << "N";
        
        
        if((frame.value & Keyboard::KEY_SHIFT) > 0)
            os << " SH ";
        else
            os << " NU ";
        
        if((frame.value & Keyboard::KEY_CTRL) > 0)
            os << " CL ";
        else
            os << " NU ";
        
        
        return os;
    }
    std::ostream& operator<<(std::ostream &os, const DBusRawFrame & frame)
    {
        for(int i=0; i<DBUS_FRAME_SIZE; ++i)
            os << hex << (int)frame.data[i] << " ";
        return os;
    }
    
    std::ostream& operator<<(std::ostream &os, const DBusFrame & frame)
    {
        os << frame.rc << endl;
        os << frame.mouse << endl;
        os << frame.keyboard << endl;
        
        return os;
    }
    
    
    DBusRawFrame DBusFrame::encode(void)
    {
        DBusRawFrame rawFrame;
        unsigned char *pData = rawFrame.data;
        
        pData[0] = rc.ch0 & 0xFF;
        pData[1] = ((rc.ch0 >> 8) & 0x07) + ((rc.ch1 << 3) & 0xF8);
        pData[2] = ((rc.ch1 >> 5) & 0x3F) + ((rc.ch2 << 6) & 0xC0);
        pData[3] = (rc.ch2 >> 2) & 0xFF;
        pData[4] = ((rc.ch2 >> 10) & 0x01) + ((rc.ch3 << 1) & 0xFE);
        pData[5] = ((rc.ch3 >> 7) & 0x0F) + ((rc.s1 << 6) & 0xC0) + ((rc.s2 << 4) & 0x30);
        
        pData[6] = mouse.x & 0xFF;
        pData[7] = (mouse.x >> 8) & 0xFF;
        pData[8] = mouse.y & 0xFF;
        pData[9] = (mouse.y >> 8) & 0xFF;
        pData[10] = mouse.z & 0xFF;
        pData[11] = (mouse.z >> 8) & 0xFF;
        pData[12] = mouse.lbutton;
        pData[13] = mouse.rbutton;
        
        pData[14] = keyboard.value;
        
        pData[15] = 0;
        pData[16] = 0;
        pData[17] = 0;
        return rawFrame;
    }

