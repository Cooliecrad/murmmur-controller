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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Communication.h"
#include "ili9341.h"
#include "vision.h"
#include "Timer.h"
#include "HWT101.h"
#include "StepmotorGPIO.h"
#include "maintask.h"
#include "Arm.h"
#include "servor_ctl.h"
#include "easy_font.h"
#include "PID.h"
#include "Chassis.h"
#include "using_PID.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define locationtime 200
#define shorttime 4000
#define angletime 1000

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/**
 * @brief 用户在DEBUG中可以修改并用于测试的位姿变量
 */
Pose2f USER_POSE;

volatile uint8_t controlflag;
volatile uint8_t anglecontrolflag;
volatile uint8_t anglecontrolflag2;
volatile uint8_t workflag;
volatile uint8_t adjustflag;
volatile uint8_t duojikongzhiflag;
volatile uint8_t debugflag;

volatile uint8_t flag;
uint8_t duankou;
uint16_t sudu;
uint16_t jiaodu;

float targetx;
float targety;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern PIDTypeDef AnglePID;

StepMotorStruct Motor_X = {
    //??????
    .ID = 3,
    .huart = &huart8,
    .Ratio = 1,
    //?????
    .Motor_Direction = MOTOR_FORWARD,

    .Steps_Cur = 0,
    .Steps_Tar = 0,
    .Steps_Sum = 0,
    .Steps_ToDir = 0,
    .Angle = 0,

    .Speed_Tar = 2000, //??????
    .Speed_Max = 5000,
    .Speed_Min = 0,
    .acceleration = 220.0f,
    .deceleration = 200.0f,
    //?????
    .Slow_Flag = 1, //??????????
    .Slow_Finish_Flag = 0,
    .Step_Finish_Flag = 0,
    .Step_Running_Flag = 0,
    .ToStep_Flag = 1};

StepMotorStruct Motor_R = {
    //????????
    .htim = &htim2,
    .GPIO_Port_EN = GPIOA,
    .GPIO_PIN_EN = GPIO_PIN_0,
    .GPIO_Port_DIR = GPIOC,
    .GPIO_PIN_DIR = GPIO_PIN_13,
    .GPIO_Port_OUT = GPIOB,
    .GPIO_PIN_OUT = GPIO_PIN_10,
    //????
    .Steps_Cur = 0,
    .Steps_Tar = 0,
    .Steps_Sum = 0,
    //???
    .Speed_Max = 28800,
    .Speed_Min = 3000,
    //?????
    .Slow_Flag = 0,
    .Slow_Finish_Flag = 0,
    .Step_Finish_Flag = 0,
    .Step_Running_Flag = 0,
};

/*
????????22400????
*/
StepMotorStruct Motor_Z = {
    //????????
    .htim = &htim5,
    .GPIO_Port_EN = GPIOA,
    .GPIO_PIN_EN = GPIO_PIN_1,
    .GPIO_Port_DIR = GPIOC,
    .GPIO_PIN_DIR = GPIO_PIN_14,
    .GPIO_Port_OUT = GPIOB,
    .GPIO_PIN_OUT = GPIO_PIN_11,
    //????
    .Steps_Cur = 0,
    .Steps_Tar = 0,
    .Steps_Sum = 0,
    //???
    .Speed_Max = 28800,
    .Speed_Min = 3000,
    //?????
    .Slow_Flag = 0,
    .Slow_Finish_Flag = 0,
    .Step_Finish_Flag = 0,
    .Step_Running_Flag = 0,
};

volatile uint8_t arm_test_flag = 0;
volatile ArmAction action_test;
volatile ArmTarget color_test;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
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
    uint8_t list[10] = {0x55, 0x55, 0x08, 0x03, 0x01, 0x00, 0x00, 0x00, 0x7D, 0x00};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
#define LINE_SPEED 500
#define LINE_ACC 150
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
    //  HAL_UART_Transmit_DMA(&huart2,list,13);
    Communication_PacketInit();

    //__HAL_DMA_DISABLE_IT(&hdma_usart2_tx,DMA_IT_HT);
    Communication_Start();

    servor_ctl_init(&huart5); // 初始化舵机控制

    vision_init(&huart6); // 初始化视觉通信

    // 初始化显示屏
    ili9341_init(&hspi1);
    ili9341_set_window(0, 0, 320, 240);
    EasyRender label{ili9341_set_window, ili9341_write_GRAM};
    label.fill(0, 0, 320, 240, 0x0);
    label.label(0, 0, "123+321", 0xffff);

    HWT101_init(&huart3); // 初始化陀螺仪

    Timer_Start();

    PID_Init(&ANGLE_PID, PID_mode_PID, 7, -10, 0, -1, 800, 10, 2); //-0.02
    PID_Init(&ANGLE_PID2, PID_mode_PID, 7, -0.5, 0, -0.1, 100, 5, 2);
    PID_Init(&PID_LOCATION_JUST_X, PID_mode_PID, 1, 0.2, 0.0005, 0.1, 100, 0, 2);
    PID_Init(&PID_LOCATION_JUST_Y, PID_mode_PID, 1, 0.2, 0.0005, 0.1, 100, 0, 2);

    chassis_init(&huart2);
    // Screwrod_Blockage_protection_Disable(&Chassis);

    // StepMotor_ToStepData(&Motor_R,0,9600,1);
    // StepMotor_ToStepData(&Motor_Z,0,9600,1);
    // StepMotor_ToStepControl(&Motor_R);
    // StepMotor_ToStepControl(&Motor_Z);

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
    // uint8_t BTCommandArry[5] = {0XFF, 0XAA, 0X04, 0X02, 0X00};
    // uint8_t ULockCommandArry[5] = {0XFF, 0XAA, 0X69, 0X88, 0XB5};
    // uint8_t LockCommandArry[5] = {0XFF, 0XAA, 0X00, 0X00, 0X00};

    // HAL_UART_Transmit(&huart3, ULockCommandArry, 5, 1000);
    // HAL_Delay(1000);
    // HAL_UART_Transmit(&huart3, BTCommandArry, 5, 1000);
    // HAL_Delay(1000);
    // HAL_UART_Transmit(&huart3, LockCommandArry, 5, 1000);
    // HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {

        if (arm_test_flag == 1)
        {
            arm_test_flag = 0;
            Arm_Action(action_test, color_test);
        } else if (arm_test_flag == 2)
        {
            arm_test_flag = 0;
            servor_ctl(servor_object_CLAW, 1);
            HAL_Delay(1000);
            servor_ctl(servor_object_CLAW, 2);
            HAL_Delay(1000);
        }

        if (controlflag == 1)
        {
            while (1)
            {
                USER_POSE.xy.y = 0.8;
                USER_POSE.xy.x = 0;
                chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);

                USER_POSE.xy.y = -0.8;
                chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);
            }
            controlflag = 0;
        }
        else if (controlflag == 2)
        {
            USER_POSE.xy.x = 0.5;
            USER_POSE.xy.y = 0;
            USER_POSE.angle = 0;
            chassis_move_speed(LINE_ACC, &USER_POSE);
            controlflag = 0;
        }
        if (anglecontrolflag == 1)
        {
            while (1)
            {
                USER_POSE.angle = 0;
                chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                USER_POSE.angle = 150;
                chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                USER_POSE.angle = -50;
                chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                USER_POSE.angle = 50;
                chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
            }
            anglecontrolflag = 0;
        }

        if (adjustflag == 1)
        {
            // Chassis_Movedistance(&Chassis,10,10,targetx,targety);
            chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);
            adjustflag = 0;
        }

        if (workflag == 1)
        {

            Taskpoint();
            workflag = 0;
        }

        if (duojikongzhiflag == 1)
        {
            list[5] = (uint8_t)sudu;
            list[6] = (uint8_t)(sudu >> 8);
            list[7] = duankou;
            list[8] = (uint8_t)jiaodu;
            list[9] = (uint8_t)(jiaodu >> 8);
            HAL_UART_Transmit_IT(&huart5, list, 10);
            duojikongzhiflag = 0;
        }

        if (debugflag == 1)
        {
            //		Arm_Action(Dtct,0);
            //		HAL_Delay(4000);
            my_debug(flag);
            debugflag = 0;
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 480;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
