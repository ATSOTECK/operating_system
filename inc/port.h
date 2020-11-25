#include "types.h"

#ifndef __PORT_H__
#define __PORT_H__

uint8_t portByteIn(ushort port);
void portByteOut(ushort port, uint8_t data);
ushort portWordIn(ushort port);
void portWordOut(ushort port, ushort data);

#endif //__PORT_H__
