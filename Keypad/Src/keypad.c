#include "main.h"
#include "keypad.h"
#include "stdio.h"
#include "lcd.h"
#include "lcd_init.h"
#include "spi.h"
#include "usart.h"

extern uint16_t datatemp[6];

const unsigned char gImage_123[128] = { /* 0X10,0X10,0X00,0X08,0X00,0X08,0X01,0X1B, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};



static KEY_GPIO_PIN OUT_KEY_PIN[] = //static表示只在本文件内有效，即让别的文件可以继续定义这个
{
  OUT_KEY_PORT_PIN
};

static KEY_GPIO_PIN IN_KEY_PIN[] = //static表示只在本文件内有效，即让别的文件可以继续定义这个
{
  IN_KEY_PORT_PIN
};



int Scan44Key(void)
{
   int KeyValue = -1;
   int i=0,j=0,k=0;

   for (i = 0; i < 5; i++) 
	{
		for (j = 0; j < OUT_KEY_COUNT; j++) 
		{
			if(i==j) HAL_GPIO_WritePin(OUT_KEY_PIN[j].port, OUT_KEY_PIN[j].pin, GPIO_PIN_SET);
			else     HAL_GPIO_WritePin(OUT_KEY_PIN[j].port, OUT_KEY_PIN[j].pin, GPIO_PIN_RESET);
			
		    for (k = 0; k < IN_KEY_COUNT; k++) 
	        {
			    if(HAL_GPIO_ReadPin(IN_KEY_PIN[k].port, IN_KEY_PIN[k].pin)>0)  
				{
					HAL_Delay(200);
				    if(HAL_GPIO_ReadPin(IN_KEY_PIN[k].port, IN_KEY_PIN[k].pin)>0)
                    KeyValue= i+(k*4);
				}
			
	        }
		}
	 } 
     return  KeyValue;
}

 void  InputCode(void)
 {
	 int  flag=0,ikey=0,i=0,j=0,k=1,iNum[10];
	 
	 while(k)  
	 {
		   ikey = Scan44Key();
	       switch(ikey)
	       {
				case 0:ikey=0;break;
				case 1:ikey=1;break;
				case 2:ikey=2;break;
				case 3:ikey=3;break;
				case 4:ikey=4;break;
				case 5:ikey=5;break;
				case 6:ikey=6;break;
				case 7:ikey=7;break;
				case 8:ikey=8;break;
				case 9:ikey=9;break;
				case 11:ikey=11;break;
				case 12:ikey=12;break;
				default:ikey=NULL;break;
		  } 
	 
		  if(ikey!=NULL&&i<6)
		  {
			   if(ikey>=0&&ikey<=9)
				{
					iNum[i]=ikey;
					printf("*");
					i++;
				 }
		  }
		 
		 if(ikey==11)       
	     {
			 i--;
			 printf("\b");
	     }
		 
		  if(ikey==12)       			  
	      {
			  flag=0;
			  i++;
			  for(j=0;j<6;j++)
			  {
				 if(iNum[j]!=datatemp[j])
				 flag=1;
			  }
		      if(flag==0)    //密码正确
		      {  
				  printf("\r\n Congratulations! Input Correct\r\n");
		         break;
		      }
		      else          //密码错误
		      {
			     printf("\r\n Input Error\r\n");
				 printf("\r\n Please Input Code Again:\r\n");
			     i=0;
		      }
	     }
		  
		 if(i<7||ikey==NULL)  k=1;
		 else                 k=0;
	 }	  
 }

 
 
 void ResetCode(void)
 {
	 
	 USART_RX_STA1=0;
	 
	 HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer1, RXBUFFERSIZE1);
	 
 }
 
 
 void TextChoose(void)
 {
	 int k,ikey;
	 
	  while(k)  
	 {
		   ikey = Scan44Key();
	       switch(ikey)
	       {
				case 1:ikey=3;break;
				case 2:ikey=1;break;
				case 3:ikey=2;break;
				case 15:ikey=15;break;
				default:ikey=NULL;break;
		  }
		   
         if(ikey==3)
	     { 
			LCD_Fill(10,50,18,58,YELLOW);
			LCD_Fill(10,70,18,78,YELLOW);
			LCD_ShowPicture(10,30,8,8,gImage_123);
	     }else if(ikey==1)
	     {
			LCD_Fill(10,70,18,78,YELLOW);
			LCD_Fill(10,30,18,38,YELLOW);
			LCD_ShowPicture(10,50,8,8,gImage_123);	
	     }else if(ikey==2)
	     {
			LCD_Fill(10,50,18,58,YELLOW);
			LCD_Fill(10,30,18,38,YELLOW);
			LCD_ShowPicture(10,70,8,8,gImage_123);
	     }else if(ikey==15)  
		 {
			k=0;
		 	LCD_Fill(10,50,18,58,YELLOW);
			LCD_Fill(10,30,18,38,YELLOW);
			LCD_Fill(10,70,18,78,YELLOW);
		 }
		 
		   
	  }
 
 
 
 }

void KeyHandle(int iKey)
{	
	if(iKey>=0)
	{
		if(iKey>=0&&iKey<=9) printf("key %d be pressed \r\n",iKey);
		 else if(iKey==10)    
		 {
			 printf("Code Input Key\r\n");
			 printf("Please Input Your Six Bits Code\r\n");
			 InputCode();
		 }
		 else if(iKey==11)    printf("Confirm Key\r\n");
		 else if(iKey==12)    printf("Delete Key\r\n");
		 else if(iKey==13)
		 {
			 printf("Reset Code Key\r\n");   
             printf("Please Reset Your Six Bits Code\r\n");
			 ResetCode();
		 }	
		 else if(iKey==14)
		 {
			 printf("Text Choosing Begin Key\r\n"); 
			 TextChoose();
		 }
	     else if(iKey==15)
		 {
			 printf("Text Choosing exti Key\r\n");
		 }

 }
} 


