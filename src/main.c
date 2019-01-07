/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/main.c
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    27-May-2016
  * @brief   This example describes how to configure and use GPIOs through
  *          the STM32F0xx HAL API.
  ******************************************************************************
  */
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
//@todo 	field descriptions, header
//@src 		Sec 26.7.5 Timing register (I2C_TIMINGR)
typedef struct tgr_value {
	uint8_t PRESC;
	uint8_t SCLDEL;
	uint8_t SDADEL;
	uint8_t SCLH;
	uint8_t SCLL;
} TGR_Value;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStruct;
I2C_HandleTypeDef hi2c2;
UART_HandleTypeDef huart2;

uint8_t tx_data[5] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_I2C2_Init(void);
void Error_Handler(void);
uint32_t I2C_GetTIMINGR(TGR_Value config);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {
  /* This sample code shows how to use GPIO HAL API to toggle LED2 IOs
    in an infinite loop. */

  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();
  
  /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
  LED2_GPIO_CLK_ENABLE();

  /* -2- Configure IOs in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = LED2_PIN;
  HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

  MX_I2C2_Init();

  /* -3- Toggle IOs in an infinite loop */
  for(;;) {
   //Locals
	HAL_StatusTypeDef result;

	//Notice
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);

    //Transmit
    result = HAL_I2C_Master_Transmit(&hi2c2, 0xEE, tx_data, 1, 1000);

    //Safety
    if(result != HAL_OK) {
    	asm(" nop");									/* Debug breakpoint			*/
    }

    //Loop Delay
    HAL_Delay(100);										/* Insert delay 100 ms 		*/
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Select HSI48 Oscillator as PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;

  if(HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK) {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK) {
    Error_Handler();
  }

  return;
}


void MX_I2C2_Init(void) {

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20303E5D;
  hi2c2.Init.OwnAddress1 = 200;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if(HAL_I2C_Init(&hi2c2) != HAL_OK) {
    Error_Handler();
  }

  /**Configure Analogue filter
  */
  if(HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    Error_Handler();
  }

  /**Configure Digital filter
  */
  if(HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
    Error_Handler();
  }

  return;
}


//@todo 	header
//@todo 	validate and use
//@ref		Sec. 26.7.5 Timing register (I2C_TIMINGR)
uint32_t I2C_GetTIMINGR(TGR_Value config) {

	//Locals
	uint32_t reg_val;

	//Init
	reg_val = 0;

	//Safety
	config.PRESC  &= (0x0F);						/* b3:b0						*/
	config.SCLDEL &= (0x0F);						/* b3:b0						*/
	config.SDADEL &= (0x0F);						/* b3:b0						*/
	config.PRESC  &= (0xFF);						/* all bits						*/
	config.PRESC  &= (0xFF);						/* all bits						*/

	//PRESC
	reg_val |= (config.PRESC<<28);

	//SCLDEL
	reg_val |= (config.SCLDEL<<20);

	//SDADEL
	reg_val |= (config.SDADEL<<16);

	//SCLH
	reg_val |= (config.SCLH<<8);

	//SCLL
	reg_val |= (config.SCLL<<0);


	return reg_val;
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {

  /* User may add here some code to deal with this error */
  for(;;) {
	  asm(" nop");
  }
}

