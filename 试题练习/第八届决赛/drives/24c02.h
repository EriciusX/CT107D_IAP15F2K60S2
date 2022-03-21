#ifndef __24C02_H
#define __24C02_H

#include "type.h"

void eeprom_read(uchar *pucbuf, uchar addr, uchar num);
void eeprom_write(uchar *pucbuf, uchar addr, uchar num);

#endif