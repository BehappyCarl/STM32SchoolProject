/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_qspi.h"
#include "echo_server.h"
#include "tcp.h"
#include <stdio.h>
#include <string.h>
#include "Netflix.h"
#include "Google.h"
#include "Outlook.h"
#include "Canvas.h"
#include "Youtube.h"


#define SCREENSAVER_DELAY 30000
#include "tcp.h"
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

DMA2D_HandleTypeDef hdma2d;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

UART_HandleTypeDef huart1;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */
const char NETFLIX_site[] = " HTTP/1.0\r\nHost: Netflix.com\r\n\r\n";


#define GoogleXStartPosition 93
#define GoogleXStopPosition 193
#define OutlookXStartPosition 286
#define OutlookXStopPosition 386
#define CanvasXStartPosition 45
#define CanvasXStopPosition 145
#define YoutubeXStartPosition 190
#define YoutubeXStopPosition 290
#define NetflixXStartPosition 335
#define NetflixXStopPosition 435

#define GoogleYStartPosition 24
#define GoogleYStopPosition 124
#define OutlookYStartPosition 27
#define OutlookYStopPosition 120
#define CanvasYStartPosition 148
#define CanvasYStopPosition 248
#define YoutubeYStartPosition 148
#define YoutubeYStopPosition 248
#define NetflixYStartPosition 148
#define NetflixYStopPosition 248
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LTDC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_QUADSPI_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//This function will send the characters from printf via UART1.
//Modified so you don't have to type \r\n anymore, just type \n.
int _write(int file, char *ptr, int len) {
	for(int i = 0; i < len; i++){
		if(ptr[i]=='\n'){
			HAL_UART_Transmit(&huart1, (uint8_t*)"\r", 1, HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(&huart1, (uint8_t*)&ptr[i], 1, HAL_MAX_DELAY);
	}
    return len;
}
err_t connectedToServer(void *arg, struct tcp_pcb *connection, err_t err)
{
	uint8_t button = *((uint8_t *) arg);
	printf("Geconecteerd\r\n");

	//write to the server

	const char cHttpHeaderGoogle1[] = "GET /My_PHP_files/Stm32_Server.php?q=Google";
	const char cHttpHeaderGoogle2[] = " HTTP/1.0\r\nHost: localhost\r\n\r\n";
	const char cHttpHeaderOutlook1[] = "GET /My_PHP_files/Stm32_Server.php?q=Outlook";
	const char cHttpHeaderOutlook2[] = " HTTP/1.0\r\nHost: localhost\r\n\r\n";
	const char cHttpHeaderCanvas1[] = "GET /My_PHP_files/Stm32_Server.php?q=Canvas";
	const char cHttpHeaderCanvas2[] = " HTTP/1.0\r\nHost: localhost\r\n\r\n";
	const char cHttpHeaderYoutube1[] = "GET /My_PHP_files/Stm32_Server.php?q=Youtube";
	const char cHttpHeaderYoutube2[] = " HTTP/1.0\r\nHost: localhost\r\n\r\n";
	const char cHttpHeaderNetflix1[] = "GET /My_PHP_files/Stm32_Server.php?q=Netflix";
	const char cHttpHeaderNetflix2[] = " HTTP/1.0\r\nHost: localhost\r\n\r\n";
	switch(button)
	{
	case 1:
		tcp_write(connection, cHttpHeaderGoogle1, strlen(cHttpHeaderGoogle1), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE); // eerste deel van de header sturen
		tcp_write(connection, cHttpHeaderGoogle2, strlen(cHttpHeaderGoogle2), TCP_WRITE_FLAG_COPY ); // De rest van de header sturen
		HAL_Delay(500);
		break;
	case 2:
		tcp_write(connection, cHttpHeaderOutlook1, strlen(cHttpHeaderOutlook1), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE); // eerste deel van de header sturen
		tcp_write(connection, cHttpHeaderOutlook2, strlen(cHttpHeaderOutlook2), TCP_WRITE_FLAG_COPY ); // De rest van de header sturen
		HAL_Delay(500);
		break;
	case 3:
		tcp_write(connection, cHttpHeaderCanvas1, strlen(cHttpHeaderCanvas1), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE); // eerste deel van de header sturen
		tcp_write(connection, cHttpHeaderCanvas2, strlen(cHttpHeaderCanvas2), TCP_WRITE_FLAG_COPY ); // De rest van de header sturen
		HAL_Delay(500);
		break;
	case 4:
		tcp_write(connection, cHttpHeaderYoutube1, strlen(cHttpHeaderYoutube1), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE); // eerste deel van de header sturen
		tcp_write(connection, cHttpHeaderYoutube2, strlen(cHttpHeaderYoutube2), TCP_WRITE_FLAG_COPY ); // De rest van de header sturen
		HAL_Delay(500);
		break;
	case 5:
		tcp_write(connection, cHttpHeaderNetflix1, strlen(cHttpHeaderNetflix1), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE); // eerste deel van de header sturen
		tcp_write(connection, cHttpHeaderNetflix2, strlen(cHttpHeaderNetflix2), TCP_WRITE_FLAG_COPY ); // De rest van de header sturen
		HAL_Delay(500);
		break;
		}
	tcp_output(connection); //Alles nu naar buiten sturen


	//configure the receive callback
	return ERR_OK;
}

void ClearTouchedMarks(uint8_t button)
      {
   	   BSP_LCD_SelectLayer(0);
   	   BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
   	   switch(button)
   	   {
   	   case 1:
   		BSP_LCD_FillRect(GoogleXStartPosition, GoogleYStartPosition, 100, 100);
   		   break;
   	   case 2:
   		BSP_LCD_FillRect(OutlookXStartPosition, OutlookYStartPosition, 100, 93);
   		   break;
   	   case 3:
   		BSP_LCD_FillRect(CanvasXStartPosition, CanvasYStartPosition, 100, 100);
   	   	   break;
   	   case 4:
   		BSP_LCD_FillRect(YoutubeXStartPosition, YoutubeYStartPosition, 100, 100);
   		   break;
   	   case 5:
   		BSP_LCD_FillRect(NetflixXStartPosition, NetflixYStartPosition, 100, 100);
   		   break;
   	   default:
   		   break;
   	   }
      }
	void SetNewTouchedMarks(uint8_t button)
	{
		switch(button)
		{
		case 1:
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(GoogleXStartPosition, GoogleYStartPosition, 100, 100);
			break;

		case 2:
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(OutlookXStartPosition, OutlookYStartPosition, 100, 93);
			break;
		case 3:
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(CanvasXStartPosition, CanvasYStartPosition, 100, 100);
			break;
		case 4:
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(YoutubeXStartPosition, YoutubeYStartPosition, 100, 100);

			break;
		case 5:
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(NetflixXStartPosition, NetflixYStartPosition, 100, 100);
			break;
		}
	}
   void SetANewConnection(uint8_t button)
   {
	   static uint8_t sbutton;
	   sbutton = button;
	   struct tcp_pcb *connection;
	   connection = tcp_new();
	   ip4_addr_t serverIp;
	   tcp_arg(connection, &sbutton);
	   IP_ADDR4(&serverIp, 192,168,69,11); /*server Ip = 192.168.69.11*/
	   tcp_connect(connection, &serverIp, 80, connectedToServer);
   }
   uint8_t ReadTouch(uint8_t button)
   {
	   uint32_t SysTick_Old_Time;
	   uint32_t SysTick_New_Time;
	   TS_StateTypeDef TS_State;
	   BSP_TS_GetState(&TS_State);
	   //printf("%d",SysTick);

	   if(TS_State.touchDetected == 1 ) //1 vinger op het scherm
	   {

	   if((TS_State.touchX[0]<=GoogleXStopPosition)&&(TS_State.touchX[0]>=GoogleXStartPosition)&&(TS_State.touchY[0]<=GoogleYStopPosition)&&(TS_State.touchY[0]>=GoogleYStartPosition))
	   {
		   SysTick_New_Time = HAL_GetTick();
		   if(SysTick_Old_Time != 0)
		   {

		   }
		   if(Systick_old_time == 0)
		   {
			   systick_old_time = HAL_GetTick();
		   }
		   if(SysTick_New_Time > SysTick_Old_Time+2000)
		   {
			   SysTick_Old_Time = HAL_GetTick();
			   printf("Google is Pushed\r\n");
			   ClearTouchedMarks(button);
			   button = 1;	//debouncing
			   SetNewTouchedMarks(button);
			   SetANewConnection(button);
		   }
		   if(button != 1)
		   {
		   SysTick_Old_Time = HAL_GetTick();
		   printf("%d", SysTick_Old_Time);

		   }



	   }
	   if((button != 2)&&(TS_State.touchX[0]<=OutlookXStopPosition)&&(TS_State.touchX[0]>=OutlookXStartPosition)&&(TS_State.touchY[0]<=OutlookYStopPosition)&&(TS_State.touchY[0]>=OutlookYStartPosition))
	   {
		   printf("Outlook is Pushed\r\n");
		   ClearTouchedMarks(button);
		   button = 2;	//debouncing
		   SetNewTouchedMarks(button);
		   SetANewConnection(button);

	   }
	   if((button != 3)&&(TS_State.touchX[0]<=CanvasXStopPosition)&&(TS_State.touchX[0]>=CanvasXStartPosition)&&(TS_State.touchY[0]<=CanvasYStopPosition)&&(TS_State.touchY[0]>=CanvasYStartPosition))
	   {
		   printf("Canvas is Pushed\r\n");
		   ClearTouchedMarks(button);
		   button = 3;	//debouncing
		   SetNewTouchedMarks(button);
		   SetANewConnection(button);

	   }
	   if((button != 4)&&(TS_State.touchX[0]<=YoutubeXStopPosition)&&(TS_State.touchX[0]>=YoutubeXStartPosition)&&(TS_State.touchY[0]<=YoutubeYStopPosition)&&(TS_State.touchY[0]>=YoutubeYStartPosition))
	   {
		   printf("Youtube is Pushed\r\n");
		   ClearTouchedMarks(button);
		   button = 4;	//debouncing
		   SetNewTouchedMarks(button);
		   SetANewConnection(button);

	   }
	   if((button != 5)&&(TS_State.touchX[0]<=NetflixXStopPosition)&&(TS_State.touchX[0]>=NetflixXStartPosition)&&(TS_State.touchY[0]<=NetflixYStopPosition)&&(TS_State.touchY[0]>=NetflixYStartPosition))
	   {
		   printf("Netflix is Pushed\r\n");
		   ClearTouchedMarks(button);
		   button = 5;	//debouncing
		   SetNewTouchedMarks(button);
		   SetANewConnection(button);

	   }
	   }
	   return button;
   }
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
  MX_LTDC_Init();
  MX_USART1_UART_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LWIP_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  /*QSPI INIT*/
  BSP_QSPI_Init();
  BSP_QSPI_MemoryMappedMode();
  WRITE_REG(QUADSPI->LPTR, 0xFFF);
  printf("Running LwIP & LCD start project...\n");
  echo_init();
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS+(480*272*4));
  /* Enable the LCD */
  BSP_LCD_DisplayOn();
  /* Select the LCD Background Layer  */
  BSP_LCD_SelectLayer(0);
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SelectLayer(1);
  /* Clear the foreground Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  /* Some sign */
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(0);/*transparant*/
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*) "Click on what to open on computer!", CENTER_MODE);
  BSP_TS_Init(480,272);
  BSP_LCD_SelectLayer(1);
  WDA_LCD_DrawBitmap(GOOGLE_DATA, GoogleXStartPosition, GoogleYStartPosition, GOOGLE_DATA_X_PIXEL, GOOGLE_DATA_Y_PIXEL, GOOGLE_DATA_FORMAT);
  WDA_LCD_DrawBitmap(OUTLOOK_DATA, OutlookXStartPosition, OutlookYStartPosition, OUTLOOK_DATA_X_PIXEL, OUTLOOK_DATA_Y_PIXEL, OUTLOOK_DATA_FORMAT);
  WDA_LCD_DrawBitmap(CANVAS_DATA, CanvasXStartPosition, CanvasYStartPosition, CANVAS_DATA_X_PIXEL, CANVAS_DATA_Y_PIXEL, CANVAS_DATA_FORMAT);
  WDA_LCD_DrawBitmap(YOUTUBE_DATA, YoutubeXStartPosition, YoutubeYStartPosition, YOUTUBE_DATA_X_PIXEL, YOUTUBE_DATA_Y_PIXEL, YOUTUBE_DATA_FORMAT);
  WDA_LCD_DrawBitmap(NETFLIX_DATA, NetflixXStartPosition, NetflixYStartPosition, NETFLIX_DATA_X_PIXEL, NETFLIX_DATA_Y_PIXEL, NETFLIX_DATA_FORMAT);

  uint8_t button = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	MX_LWIP_Process();
	button = ReadTouch(button);
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 53;
  hltdc.Init.AccumulatedVBP = 11;
  hltdc.Init.AccumulatedActiveW = 533;
  hltdc.Init.AccumulatedActiveH = 283;
  hltdc.Init.TotalWidth = 565;
  hltdc.Init.TotalHeigh = 285;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 255;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB1555;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 480;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 272;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB1555;
  pLayerCfg1.Alpha = 255;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 480;
  pLayerCfg1.ImageHeight = 272;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 16;
  SdramTiming.ExitSelfRefreshDelay = 16;
  SdramTiming.SelfRefreshTime = 16;
  SdramTiming.RowCycleDelay = 16;
  SdramTiming.WriteRecoveryTime = 16;
  SdramTiming.RPDelay = 16;
  SdramTiming.RCDDelay = 16;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, LED_Pin|LCD_DISP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LCD_BL_CTRL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin LCD_DISP_Pin */
  GPIO_InitStruct.Pin = LED_Pin|LCD_DISP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

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
