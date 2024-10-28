#ifndef _L298N_H
#define _L298N_H
#include "main.h"
#include "tim.h"

extern uint32_t PWM;

void L298N_Init(void);
void L298N_Go(void);
void L298N_Back(void);
void L298N_Left(void);
void L298N_Right(void);
void L298N_Stop(void);

#endif
