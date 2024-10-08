/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "ring_buffer.h"
#include "keypad.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

//variables necesarias  para el buffer
#define USART2_BUFFER_SIZE 4
uint8_t usart2_buffer[USART2_BUFFER_SIZE];
ring_buffer_t usart2_rb;
uint8_t usart2_rx;

//variables necesarias  para el buffer
#define USART3_BUFFER_SIZE 20
uint8_t usart3_buffer[USART3_BUFFER_SIZE];
ring_buffer_t usart3_rb;
uint8_t usart3_rx;

//variables de control de los leds
uint8_t enter = 0;
flag_lights place = NOTHING;
uint8_t key_control = 0;

uint32_t one_last_press_tick = 0;
uint32_t A_last_press_tick = 0;
uint32_t B_last_press_tick = 0;
uint32_t C_last_press_tick = 0;
uint32_t D_last_press_tick = 0;
uint8_t party_mode = 0;
uint8_t off_everything = 0;
uint8_t turn_off_light = 0;
uint8_t off_control = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 10);
  return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Data received in USART2 */
  if (huart->Instance == USART2) {
	  usart2_rx = USART2->RDR; // leyendo el byte recibido de USART2
	  ring_buffer_write(&usart2_rb, usart2_rx); // put the data received in buffer
	  //HAL_UART_Receive_IT(&huart2, &usart2_rx, 1); // enable interrupt to continue receiving
	  ATOMIC_SET_BIT(USART2->CR1, USART_CR1_RXNEIE); // usando un funcion mas liviana para reducir memoria
  }

  /* Data received in USART3 */
    if (huart->Instance == USART3) {
  	  usart3_rx = USART3->RDR;
  	  ring_buffer_write(&usart3_rb, usart3_rx);
  	  ATOMIC_SET_BIT(USART3->CR1, USART_CR1_RXNEIE);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	uint8_t key_pressed = keypad_scan(GPIO_Pin);

	if (key_pressed != 0xFF) {
			//la tecla # funciona commo enter
			if(key_pressed == '#'){
				printf("ENTER: %c\r\n", key_pressed);
				enter = 1;



				uint8_t size = ring_buffer_size(&usart3_rb);
							uint8_t room[size]; //se almacenan los datos
							uint8_t lee;

							uint8_t len_room = 0;

							uint8_t status[size-len_room];


							for (uint8_t i = 0; i <= size; i++){
								ring_buffer_read(&usart3_rb, &lee);

								if(lee == ':'){
									room[i] = 0;
									len_room = i;
									break;
								}

								room[i] = lee;
							};

							for (uint8_t i = 0; i <= size-len_room; i++){
								ring_buffer_read(&usart3_rb, &lee);

								status[i] = lee;
							}

							status[size-len_room-1] = 0;

							printf("DATA: %s\r\n", room);
							printf("STATUS: %s\r\n", status);



			return;
				//la tecla * funciona como reset
			}else if(key_pressed == '*'){
				ring_buffer_reset(&usart2_rb);
				printf("RESET: enter again %c\r\n", key_pressed);

				// Reseteamos el mensaje igualmente:
				ssd1306_FillRectangle(37, 50, 97, 20, Black);
				ssd1306_SetCursor(45, 30);
				ssd1306_WriteString("Hello!", Font_7x10, White);
				ssd1306_UpdateScreen();
				off_everything = 1;
				return;
			}

			//lógica para encendido y apagado de los leds
			if(key_pressed == 'A'){
				if (HAL_GetTick() < (A_last_press_tick + 500)) { // si la última pulsación fue antes de los 500ms
					turn_off_light = LUZ_A;
					off_control = 1;

				}else{
					place = LUZ_A;
					key_control = 1;
				}
				A_last_press_tick = HAL_GetTick();
			}

			if(key_pressed == 'B'){
				if (HAL_GetTick() < (B_last_press_tick + 500)) {
					turn_off_light = LUZ_B;
					off_control = 1;
				}else{
					place = LUZ_B;
					key_control = 1;
				}
				B_last_press_tick = HAL_GetTick();
			}

			if(key_pressed == 'C'){
				if (HAL_GetTick() < (C_last_press_tick + 500)) {
					turn_off_light = LUZ_C;
					off_control = 1;
				}else{
					place = LUZ_C;
					key_control = 1;
				}
				C_last_press_tick = HAL_GetTick();
			}

			if(key_pressed == 'D'){
				if (HAL_GetTick() < (D_last_press_tick + 500)) {
					turn_off_light = LUZ_D;
					off_control = 1;
				}else{
					place = LUZ_D;
					key_control = 1;
				}
				D_last_press_tick = HAL_GetTick();
			}

			printf("Pressed: %c\r\n", key_pressed);
			ring_buffer_write(&usart2_rb, key_pressed);

			return;
	}

}

//funcion para el ahorro de energía
void low_power_mode()
{
#define AWAKE_TIME (10 * 1000) // 10 segundos
	static uint32_t sleep_tick = AWAKE_TIME;

	if (sleep_tick > HAL_GetTick()) {
		return;
	}
	printf("Sleeping\r\n");
	sleep_tick = HAL_GetTick() + AWAKE_TIME;

	RCC->AHB1SMENR  = 0x0;
	RCC->AHB2SMENR  = 0x0;
	RCC->AHB3SMENR  = 0x0;

	RCC->APB1SMENR1 = 0x0;
	RCC->APB1SMENR2 = 0x0;
	RCC->APB2SMENR  = 0x0;

	/*Suspend Tick increment to prevent wakeup by Systick interrupt.
	Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
	HAL_SuspendTick();

	/* Enter Sleep Mode , wake up is done once User push-button is pressed */
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	/* Resume Tick interrupt if disabled prior to SLEEP mode entry */
	HAL_ResumeTick();

	printf("Awake\r\n");
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  ssd1306_Init();
  ssd1306_FillRectangle(37, 50, 97, 20, Black);
  ssd1306_SetCursor(40, 30);
  ssd1306_WriteString("Welcome!", Font_7x10, White);
  ssd1306_UpdateScreen();

ring_buffer_init(&usart2_rb, usart2_buffer, USART2_BUFFER_SIZE);
ring_buffer_init(&usart3_rb, usart3_buffer, USART3_BUFFER_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("Starting...\r\n");
  //HAL_UART_Receive_IT(&huart2, &usart2_rx, 1); // enable interrupt for USART2 Rx
  ATOMIC_SET_BIT(USART2->CR1, USART_CR1_RXNEIE); // usando un funcion mas liviana para reducir memoria
  ATOMIC_SET_BIT(USART3->CR1, USART_CR1_RXNEIE);

  //variables de control
  uint8_t cont = 0; //contador para el parpadeo de los leds
  uint8_t toggle = 0; //toggles necesarios para el parpadeo de los leds
  uint8_t lights_correct_password = 0;//bandera que indica que la contraseña es correcta y se pueden encender las luces

  while (1) {

	 if (enter == 1) { //revisa si se hizo enter para verificar la clave ingresada
		  printf("Received:\r\n");
		  while (ring_buffer_is_empty(&usart2_rb) == 0) {
			  uint8_t data[4]; //se almacenan los datos de la clave recibida
			  uint8_t read;

			  for (uint8_t i = 0; i <= 4; i++){
				  ring_buffer_read(&usart2_rb, &read);
				  data[i] = read; //se leen los datos
			  };

			  if(right_password(data)){ //se verifica si se ingresó la clave correcta
				  cont = 0;
				  toggle = 0;
				  lights_correct_password = 1;
				  //menú de instrucciones
				  messages();

				  // Si hay algo escrito, se borra:
				  ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(40, 30);
				  ssd1306_UpdateScreen();

				  // Se escribe el mensaje
				  ssd1306_WriteString("Welcome!", Font_7x10, White);
				  ssd1306_UpdateScreen();

			  }else{ //si la clave es incorrecta
				  printf("Alerta!\r\n");

				  ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(45, 30);
				  ssd1306_UpdateScreen();

				  ssd1306_WriteString("Alerta!", Font_7x10, White);
				  ssd1306_UpdateScreen();

				  // Se enciende el led 3 veces si la contraseña es incorrecta:
				  cont = 1;
				  toggle = 6;
			  }
		  }
	  		  printf("\r\n");
	  		  enter = 0;
	  		  place = 0;
	  	  }

	  if(cont == 1){
		  cont = blinking_led_ret(&toggle);
		  }

	  //si la clave es correcta y la bandera de control de las teclas está activada, se enciende la luz correspondiente
	  if(lights_correct_password == 1 && key_control == 1 && off_control==0){

	  		uint8_t message_on = light_on(place, &huart3);

	  		key_control = 0;

	  		switch(message_on){
				case 1:
				  //HAL_UART_Transmit(&huart3, "bedroom:1", 11, 10);

				  ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(45, 30);
				  ssd1306_UpdateScreen();

				  ssd1306_WriteString("Room on", Font_7x10, White);
				  ssd1306_UpdateScreen();


				  //HAL_UART_Transmit(&huart3, "bedroom:1", 9, 10);
				 break;

				case 2:
					//HAL_UART_Transmit(&huart3, "living:1", 11, 10);

				 ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(45, 30);
				  ssd1306_UpdateScreen();

				  ssd1306_WriteString("Living on", Font_7x10, White);
				  ssd1306_UpdateScreen();
				break;

				case 3:
					//HAL_UART_Transmit(&huart3, "bathroom:1", 11, 10);

				 ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(45, 30);
				  ssd1306_UpdateScreen();

				  ssd1306_WriteString("Bathroom on", Font_7x10, White);
				  ssd1306_UpdateScreen();
				break;

				case 4:
					//HAL_UART_Transmit(&huart3, "kitchen:1", 11, 10);

				 ssd1306_FillRectangle(37, 50, 97, 20, Black);
				  ssd1306_SetCursor(45, 30);
				  ssd1306_UpdateScreen();

				  ssd1306_WriteString("Kitchen on", Font_7x10, White);
				  ssd1306_UpdateScreen();
				  break;
	  			}
	  }
	  //se apaga la luz activada después de 5 segundos
		  turn_off_with_time();


	  //apagar la luz correspndiente
	  if(off_control == 1){
		  turn_off(turn_off_light, &huart3);
		  off_control = 0;
	  }

	  //todas las luces se apagan al dar reset * y el sistema se suspende
	  if(off_everything == 1 && lights_correct_password == 1){
		  turn_off_completely();
		  off_everything = 0;
		  low_power_mode();
	  }

	  if(lights_correct_password == 1){

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Room_Pin|Living_Pin|Bathroom_Pin|ROW_1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ROW_2_Pin|ROW_4_Pin|ROW_3_Pin|Kitchen_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : BUTTON_LEFT_Pin BUTTON_RIGHT_Pin */
  GPIO_InitStruct.Pin = BUTTON_LEFT_Pin|BUTTON_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Room_Pin Living_Pin Bathroom_Pin */
  GPIO_InitStruct.Pin = Room_Pin|Living_Pin|Bathroom_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : COLUMN_1_Pin */
  GPIO_InitStruct.Pin = COLUMN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(COLUMN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : COLUMN_4_Pin */
  GPIO_InitStruct.Pin = COLUMN_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(COLUMN_4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : COLUMN_2_Pin COLUMN_3_Pin */
  GPIO_InitStruct.Pin = COLUMN_2_Pin|COLUMN_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ROW_1_Pin */
  GPIO_InitStruct.Pin = ROW_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ROW_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ROW_2_Pin ROW_4_Pin ROW_3_Pin */
  GPIO_InitStruct.Pin = ROW_2_Pin|ROW_4_Pin|ROW_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Kitchen_Pin */
  GPIO_InitStruct.Pin = Kitchen_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Kitchen_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
