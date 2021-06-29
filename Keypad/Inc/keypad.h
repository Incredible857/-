#ifndef  _KEYPAD_H
#define _KEYPAD_H
#include "main.h"


#define  OUT_KEY_PORT_PIN  		{ GPIOB, GPIO_PIN_9 },\
								{ GPIOB, GPIO_PIN_8 },\
								{ GPIOB, GPIO_PIN_7 },\
								{ GPIOB, GPIO_PIN_6 }
								
#define  IN_KEY_PORT_PIN  		{ GPIOE, GPIO_PIN_6 },\
								{ GPIOE, GPIO_PIN_3},\
							    { GPIOE, GPIO_PIN_4 },\
								{ GPIOE, GPIO_PIN_5 }

#define OUT_KEY_COUNT  (sizeof(OUT_KEY_PIN)/sizeof(KEY_GPIO_PIN))//定义的KEY个数
	
#define IN_KEY_COUNT   (sizeof(IN_KEY_PIN)/sizeof(KEY_GPIO_PIN))//定义的KEY个数


typedef struct _KEY_GPIO_PIN	
{
    GPIO_TypeDef *port;
     uint16_t      pin;
} KEY_GPIO_PIN;



int Scan44Key(void);

void KeyHandle(int iKey);

void InputCode(void);

void ResetCode(void);

void TextChoose(void);

#endif

