#ifndef __CALENDAR_H
#define __CALENDAR_H	
#include "main.h"

extern uint8_t calendar_aclock_flag;
extern uint8_t calendar_alarm_flag;
extern uint16_t calendar_alarm_data[500];

void Calendar_Init(void);
void Calendar_Clock_Cnt(void);
void Calendar_Run(void);

#endif
