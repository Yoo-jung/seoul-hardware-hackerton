Skip to content
Search or jump to…
Pull requests
Issues
Marketplace
Explore
 
@gcuutakeout 
HwangGoeun
/
2022_1_Seoul-Hardware-Hackathon
Public
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
2022_1_Seoul-Hardware-Hackathon/RFID CARD/Core/Src/main.c
@HwangGoeun
HwangGoeun RFID CARD Final CODE
Latest commit 8f453cb 8 hours ago
 History
 1 contributor
292 lines (245 sloc)  8.16 KB
   
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rc522.h"
#include "stdio.h"
#include "pir_motion_sensor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* printf --> huart1 for debugging */
struct __FILE{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

FILE __stdout;

int fputc(int ch, FILE *f){
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

int ferror(FILE *f){
  /* Your implementation of ferror(). */
  return 0;
}
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
uint8_t blockAddr;
uint8_t RC_size;

uint8_t sectorKeyA[16][16] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                              {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                              {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                              {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};

uint8_t status;
uint8_t	str[MFRC522_MAX_LEN];
uint8_t sn[4];
char	buff[64];
uint8_t i;
uint8_t size;
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	MFRC522_Init();
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
  {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		
		//MFRC522_Init();
		size = sprintf(buff, "\033[H\033[2J\r\n");
		printf("%s", buff);
		
		HAL_Delay(20);
		
		status = MI_ERR;
		
		// Look for the card, return type
		status = MFRC522_Request(PICC_REQIDL, str);
		if (status == MI_OK) {
          size = sprintf(buff, "SAK: 0x%02X, 0x%02X\n\r", str[1], str[0]);
          printf("%s", buff);
          HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
    }
		else {
    }
		
		// Anti-collision, return the card's 4-byte serial number
    status = MFRC522_Anticoll(sn);
    if (status == MI_OK) {
        size = sprintf(buff, "CN: %x%x%x%x\r\n", sn[0],sn[1],sn[2],sn[3]);
        printf("%s", buff);
    }
    else {
    }
		
		// Election card, return capacity
    RC_size = MFRC522_SelectTag(sn);
    if (RC_size != 0) {
        size = sprintf(buff, "CS: %d\r\n", RC_size);
        printf("%s", buff);
    }
    else {
    }
     
		// Card reader
    status = MFRC522_Auth(PICC_AUTHENT1A, 11, sectorKeyA[2], sn);
    if (status == MI_OK) {
        // Read data
        status = MFRC522_Read(11, str);
        if (status == MI_OK) {
            size = sprintf(buff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", str[0],str[1],str[2],str[3],str[4],str[5],str[6],str[7]);
            printf("%s", buff);
            size = sprintf(buff, "%02X %02X %02X %02X %02X %02X %02X %02X\r\n", str[8],str[9],str[10],str[11],str[12],str[13],str[14],str[15]);
            printf("%s", buff);
						printf("-------------------------------------------------\r\n");
					
					  // PIR Motion Sensor Check and Control Servo Motors
						PIRMotionSensor pir = get_PIR_status();
						__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 200);
						__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 200);
						do {
							__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 200);
							__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 200);
							pir = get_PIR_status();
						} while (pir.status != 1);
						HAL_Delay(8000);
						__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
						__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
				}
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
    }
		else {
		}

    MFRC522_Halt();
    HAL_Delay(200);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
© 2022 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
