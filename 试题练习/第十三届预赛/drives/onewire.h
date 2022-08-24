#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "reg52.h"
#include "type.h"

sbit DQ = P1^4;  
 
uint rd_temp(void); 

#endif

