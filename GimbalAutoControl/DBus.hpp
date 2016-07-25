//
//  DBUS.hpp
//  Visual
//
//  Created by Summer on 5/22/16.
//  Copyright © 2016 summer. All rights reserved.
//

#ifndef DBUS_hpp
#define DBUS_hpp

//#include <stdio.h>
#include <iostream>
    
    static const int DBUS_FRAME_SIZE = 18;
    
    struct Control{
        unsigned short ch0 = 1024, ch1 = 1024, ch2 = 1024, ch3 = 1024;
        unsigned char s1 = 1, s2 = 1;
        
        friend std::ostream& operator<<(std::ostream &os, const Control & frame);
    };
    
    struct Mouse {
        short x = 0, y = 0, z = 0;
        unsigned char rbutton = 0, lbutton = 0;
        
        friend std::ostream& operator<<(std::ostream &os, const Mouse & frame);
    };
    
    struct Keyboard {
        static const int KEY_W      = 0b1;
        static const int KEY_S      = 0b10;
        static const int KEY_A      = 0b100;
        static const int KEY_D      = 0b1000;
        static const int KEY_Q      = 0b10000;
        static const int KEY_E      = 0b100000;
        static const int KEY_SHIFT  = 0b1000000;
        static const int KEY_CTRL   = 0b10000000;
        
        unsigned char value = 0;
        
        friend std::ostream& operator<<(std::ostream &os, const Keyboard & frame);
    };
    
    struct DBusRawFrame {
        unsigned char data[DBUS_FRAME_SIZE];
        DBusRawFrame();
        friend std::ostream& operator<<(std::ostream &os, const DBusRawFrame & frame);
    };
    
    struct DBusFrame {
        Control rc;
        Mouse mouse;
        Keyboard keyboard;
        DBusRawFrame encode(void);
        friend std::ostream& operator<<(std::ostream &os, const DBusFrame & frame);
        static DBusFrame decode(DBusRawFrame rawFrame)
        {
            unsigned char *pData = rawFrame.data;
            DBusFrame frame;
            frame.rc.ch0 = ((unsigned char)pData[0] | ((unsigned char)pData[1] << 8)) & 0x07FF;
            frame.rc.ch1 = (((unsigned char)pData[1] >> 3) | ((unsigned char)pData[2] << 5)) & 0x07FF;
            frame.rc.ch2 = (((unsigned char)pData[2] >> 6) | ((unsigned char)pData[3] << 2) |
                            ((unsigned char)pData[4] << 10)) & 0x07FF;
            frame.rc.ch3 = (((unsigned char)pData[4] >> 1) | ((unsigned char)pData[5]<<7)) & 0x07FF;
            
            frame.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
            frame.rc.s2 = ((pData[5] >> 4) & 0x0003);
            
            frame.mouse.x = ((unsigned char)pData[6]) | ((unsigned char)pData[7] << 8);
            frame.mouse.y = ((unsigned char)pData[8]) | ((unsigned char)pData[9] << 8);
            frame.mouse.z = ((unsigned char)pData[10]) | ((unsigned char)pData[11] << 8);
            
            frame.mouse.lbutton = pData[12];
            frame.mouse.rbutton = pData[13];
            
            frame.keyboard.value = pData[14];
            return frame;
        }
    };
    
    std::ostream& operator<<(std::ostream &os, const Control & frame);
    std::ostream& operator<<(std::ostream &os, const Mouse & frame);
    std::ostream& operator<<(std::ostream &os, const Keyboard & frame);
    std::ostream& operator<<(std::ostream &os, const DBusRawFrame & frame);
    std::ostream& operator<<(std::ostream &os, const DBusFrame & frame);

#endif /* DBUS_hpp */
