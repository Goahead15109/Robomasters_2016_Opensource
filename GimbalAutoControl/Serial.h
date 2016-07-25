#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>


#define BUFFER_SIZE 256
#define TIME_OUT 20 // 20 ms

enum parity
{
	NO,
	EVEN,
	ODD
};

class Serial
{
private:
    int	m_fd;
    struct termios m_oldtio;
    struct termios m_newtio;

	
public:
		
    char Open(char* port, int baud, char bits, parity parity, char stopbit);
    void Close(void);
    char Write(char* buffer, int length);
    int Read(char* buffer);
};

#endif
