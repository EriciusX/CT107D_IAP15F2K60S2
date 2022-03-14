#ifndef __SONIC_H
#define __SONIC_H

#include "type.h"
sbit TX = P1^0;
sbit RX = P1^1;
uchar wave_recv(void);

#endif