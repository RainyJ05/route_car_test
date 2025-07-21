/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "motor.h"
#include "key.h"
#include "stdio.h"
#include "route.h"
#include "pid.h"
#include "control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char oled_message[20] = {0};
char uart_message[2] = {0,0};//启停标志位（012），转向选择
uint8_t Move_Flag = 0;//0：待机，1：开启，2：停止
extern int TurnChoose[5];//路口行动选择，由串口数据修
uint8_t i = 0;
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim9);
	OLED_Init();
	Motor_Init();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t KeyNum = 0;//存建值，模拟放取药

  Key_Open();//按键打开
	Get_Encoder_Open();
	
	HAL_UART_Receive_IT(&huart1,(uint8_t *)uart_message,2);

  while (1)
  {
		
		OLED_NewFrame();
		sprintf(oled_message,"M:%d",Move_Flag);
		OLED_PrintASCIIString(0,0,oled_message,&afont12x6,OLED_COLOR_NORMAL);
		sprintf(oled_message,"T1:%d",TurnChoose[0]);
		OLED_PrintASCIIString(0,17,oled_message,&afont12x6,OLED_COLOR_NORMAL);
		sprintf(oled_message,"T2:%d",TurnChoose[1]);
		OLED_PrintASCIIString(60,17,oled_message,&afont12x6,OLED_COLOR_NORMAL);
		sprintf(oled_message,"T3:%d",TurnChoose[2]);
		OLED_PrintASCIIString(0,34,oled_message,&afont12x6,OLED_COLOR_NORMAL);
		sprintf(oled_message,"T4:%d",TurnChoose[3]);
		OLED_PrintASCIIString(60,34,oled_message,&afont12x6,OLED_COLOR_NORMAL);
		OLED_ShowFrame();
		
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Move_Flag = 1;//启动标志位，可以被串口数据改变
		}
		if(Move_Flag == 1)
		{
			Route_Open();
			Pid_Open(200);
			Move_Flag = 0;//清空标志位
		}
		if(Move_Flag == 2)
		{
			Pid_Close();
			Route_Close();
			Move_Flag = 0;//清空标志位
		}
		if(Turn_Flag == 1)
		{
			turn_process();//调用处理函数
			Turn_Flag = 0;//清空标志位
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器回调函数
{
	if (htim->Instance == TIM9)
	{
    Key_Tick();
		Get_Route_Error_Tick();
		Get_Encoder_Tick();
		Pid_Controller_Tick();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//串口中断回调
{
	if(huart == &huart1)
	{
		if(uart_message[0] == '0')
		{
			Move_Flag = 0;
		}
		if(uart_message[0] == '1')
		{
			Move_Flag = 1;
		}
		if(uart_message[0] == '2')
		{
			Move_Flag = 2;
		}
		if(uart_message[1] == '0')
		{
			TurnChoose[i] = 0;
		}
		if(uart_message[1] == '1')
		{
			TurnChoose[i] = 1;
		}
		if(uart_message[1] == '2')
		{
			TurnChoose[i] = 2;
		}
		i++;
//		HAL_UART_Transmit(&huart1,(uint8_t*)uart_message,2,HAL_MAX_DELAY);
		HAL_UART_Receive_IT(&huart1,(uint8_t *)uart_message,2);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
