/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "keypad.h"
#include "stdio.h"
#include "lcd.h"
#include "lcd_init.h"
#include "spi.h"
#include "adc.h"
#include "pic.h"




/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#endif
  
  
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	
	return ch;
	
}

int fgetc(FILE *f)

{
    uint8_t  ch;
    HAL_UART_Receive(&huart1,(uint8_t *)&ch, 1, 0xFFFF);
    return  ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint16_t datatemp[6]={1,1,5,4,4,1};
int iflag=0;
uint16_t  ADC1_Value;
uint16_t  ADC1_Volt;
float     ADC1_Volt1;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
		
			float percent_1;
			int   percent_2;
			
			ADC1_Value = HAL_ADC_GetValue(&hadc1);    //读取ADC转换结果
			ADC1_Volt = ADC1_Value * 330/ 4096;      //将采样值换算成电压值
			ADC1_Volt1= (ADC1_Volt/100) + (0.1*((ADC1_Volt%100)/10)) + (0.01*(ADC1_Volt%10));
				

				
			 percent_1=ADC1_Volt1/3.3;
			 percent_2=percent_1*100;
				
			 LCD_ShowIntNum(80,90,percent_2,3,BLACK,YELLOW,16);
			 LCD_ShowChar(110,90,'%',BLACK,YELLOW,16,0);	
			 
			
			
			if(75<percent_2&&percent_2<=100)
			{
				LCD_ShowPicture(50,90,16,8,power1);
			}else if(50<percent_2&&percent_2<=75)
			{
				LCD_ShowPicture(50,90,16,8,power2);
			}else if (25<percent_2&&percent_2<=50)
			{
				LCD_ShowPicture(50,90,16,8,power3);	
			}else if (0<percent_2&&percent_2<=25)
			{
				LCD_ShowPicture(50,90,16,8,power4);
			}
			
                                     
    }
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int Key=0;
//  uint8_t buffer[10];

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  SOFT_SPI_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  LCD_Init();
  /* USER CODE BEGIN 2 */
  USART_RX_STA1=0;
  HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer1, RXBUFFERSIZE1);

  printf("/********************************\r\n");
  printf("电子系统设计与分析\r\n");
  printf("实验四：通信接口应用设计\r\n");
  printf("班级：信息工程1901\r\n");
  printf("作者：刘尚宇1941900120\r\n");
  printf("日期：2021年6月9日\r\n");
  printf("/********************************\r\n");
 
  LCD_Fill(0,0,128,160,YELLOW);
  
  LCD_ShowChinese(25,30,"信息工程",BLACK,YELLOW,16,0);
  LCD_ShowString(90,30,"1901",BLACK,YELLOW,16,0);
	  
  LCD_ShowChinese(25,50,"信息工程",BLACK,YELLOW,16,0);
  LCD_ShowString(90,50,"1902",BLACK,YELLOW,16,0);
	  
  LCD_ShowChinese(25,70,"信息工程",BLACK,YELLOW,16,0);
  LCD_ShowString(90,70,"1903",BLACK,YELLOW,16,0);
  


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	Key=Scan44Key();
	KeyHandle(Key);
//    HAL_ADC_Start_IT(&hadc1);
	 
	 if(USART_RX_STA1&0x8000)
	 {
         int m;
		 datatemp[0]= USART_RX_BUF1[0]-0x30;
		 datatemp[1]= USART_RX_BUF1[1]-0x30;
		 datatemp[2]= USART_RX_BUF1[2]-0x30;
		 datatemp[3]= USART_RX_BUF1[3]-0x30;
		 datatemp[4]= USART_RX_BUF1[4]-0x30;
		 datatemp[5]= USART_RX_BUF1[5]-0x30;
		
		 printf("密码已成功修改为：\r\n");
		 
		 for(m=0;m<6;m++)
		 {
			  printf("%d",datatemp[m]);
		 }
		 USART_RX_STA1=0;
	 }
	 	
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
