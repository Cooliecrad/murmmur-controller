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
#include "ili9341.h"
#include "vision.h"
#include "remote_control.h"
#include "Timer.h"
#include "HWT101.h"
#include "maintask.h"
#include "servor_ctl.h"
#include "easy_font.h"
#include "Chassis.h"
#include "Arm.h"
#include "arm_seqs.h"
#include "cool_rgb.h"

#include "vision_adjust.h"
#include "maintask.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define locationtime 200
#define shorttime 4000
#define angletime 1000
int INIT = 0;
bool INITALIZED = false;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

uint8_t ctl_flag;
uint8_t work_flag;
uint8_t RC_flag;
uint8_t servor_flag;
float armangle;

ArmAction actiontest;
ArmTarget colortest;

/**
 * @brief 用户在DEBUG中可以修改并用于测试的位姿变量
 */
Pose2f USER_POSE;
float USER_SERVO_ADDR;
float USER_SERVO_ANGLE;

uint16_t LINE_SPEED=100;//500
uint16_t LINE_ACC=50;//150

Point2f visionpositiontest;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// extern PIDTypeDef AnglePID;

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
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
    servor_init(&huart5); // 初始化舵机控制
    arm_ctl_init(&huart8); // 初始化机械臂控制
    vision_init(&huart6); // 初始化视觉通信
    chassis_init(&huart2); // 初始化底盘控制
    HWT101_init(&huart3); // 初始化陀螺仪
    cool_rgb_init(&htim16, TIM_CHANNEL_1, 40);

    // 初始化显示屏
    ili9341_init(&hspi1);
    ili9341_set_window(0, 0, 320, 240);
    EasyRender label{ili9341_set_window, ili9341_write_GRAM};
    label.fill(0, 0, 320, 240, 0x0);
    label.label(0, 0, "123+321", 0xffff);

    Timer_Start();

    // PID_Init(&ANGLE_PID, PID_mode_PID, 7, -10, 0, -1, 800, 10, 2); //-0.02
    // PID_Init(&ANGLE_PID2, PID_mode_PID, 7, -0.5, 0, -0.1, 100, 5, 2);
    // PID_Init(&PID_LOCATION_JUST_X, PID_mode_PID, 1, 0.2, 0.0005, 0.1, 100, 0, 2);
    // PID_Init(&PID_LOCATION_JUST_Y, PID_mode_PID, 1, 0.2, 0.0005, 0.1, 100, 0, 2);


    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    // 初始化完毕 ---------------------------------------------------------------
    INITALIZED = true; // 标志着初始化完毕

    // 等待允许启动
    arm_set_state(RESET);
    GPIO_PinState state = GPIO_PIN_SET;
    while (INIT == 0)
    {
      state = HAL_GPIO_ReadPin(BOOT0_GPIO_Port, BOOT0_Pin);
      if (state == GPIO_PIN_RESET) INIT = 1;
    }
    
    // 使能机械臂，检查机械臂
    arm_zero_reset();
    work_flag = 1;
    while (1)
    {
        if (arm_test_flag == 1) // 设置角度
        {
            motor_r->move_to(armangle);
        }
        else if (arm_test_flag == 2) // Z轴移动测试
        {
            motor_z->move_to(armangle);
        }
        else if (arm_test_flag == 3) // X轴移动测试
        {
            motor_x->move_to(armangle);
        }
        else if (arm_test_flag == 5) // R旋转测试
        {
            for (int x = 0; x < 20; x++)
            {
                motor_r->move_to(270);
            }
            for (int x = 0; x < 20; x++)
            {
                motor_r->move_to(-270);
            }
            motor_r->move_to(0);
        }
        else if (arm_test_flag == 6)
        {
            Arm_Action(actiontest, colortest);
        } else if (arm_test_flag == 7)
        {
            Point3f target = {.x = visionpositiontest.x, .y = visionpositiontest.y,
                              .z = STEPMOTOR_Z_POSITIONS[2]};
            arm_ground_place(&target);
        } else if (arm_test_flag == 8) // 失能机械臂，供用户校准
        {
            arm_set_state(false);
            wing_ctl(false, true);
            claw_ctl(false, true);
        } else if (arm_test_flag == 9) // 校准机械臂
        {
//            arm_calibrate();
            arm_set_state(true);
        } else if (arm_test_flag == 10)
        {
            arm_position_update();
        } else if (arm_test_flag == 11) // 机械臂直线测试
        {
            Point3f p3f = {-5, -200, -190};
            motor_r->move_to(-180);
            motor_r->arrived();
            motor_z->move_to(-190);
            motor_z->arrived();
            for (int y=-200; y<=200; y+=10)
            {
                p3f.y = y;
                arm_ground_place(&p3f);
                arm_move_sync();
                HAL_Delay(20);
            }
        }

        if (arm_test_flag) arm_test_flag = 0;

        if (servor_flag == 1)
        {
            wing_ctl(false, false);
        } else if (servor_flag == 2)
        {
            wing_ctl(true, false);
        } else if (servor_flag == 3) // 机械爪开
        {
            claw_ctl(false, false);
        } else if (servor_flag == 4)
        {
            claw_ctl(true, false);
        } else if (servor_flag == 5)
        {
            servor_rotate(1000, true, USER_SERVO_ADDR, USER_SERVO_ANGLE);
        }

        if (servor_flag) servor_flag = 0;

        if (ctl_flag == 1)
        {
            while (1)
            {
                USER_POSE.xy.x = 2;
                USER_POSE.xy.y = 0;
                chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);

                USER_POSE.xy.x = -2;
                chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);
            }
			
			chassis_to(LINE_ACC, LINE_SPEED, USER_POSE.xy);
        }
        else if (ctl_flag == 2)
        {
            chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
        } else if (ctl_flag == 3)
        {
            chassis_to(LINE_ACC, LINE_SPEED, USER_POSE.xy);
        }
        if (ctl_flag == 4)
        {
            while (1)
            {
                for (float x=0; x<360; x++)
                {
                    chassis_rotate_abs(LINE_ACC, LINE_SPEED, x);
                }
                // chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                // USER_POSE.angle = 150;
                // chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                // USER_POSE.angle = -50;
                // chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
                // USER_POSE.angle = 50;
                // chassis_rotate_abs(LINE_ACC, LINE_SPEED, USER_POSE.angle);
            }
        }

        if (ctl_flag == 5)
        {
            chassis_move(LINE_ACC, LINE_SPEED, USER_POSE.xy);
        }
        if (ctl_flag != 0) ctl_flag = 0;

        if (work_flag == 1)
        {

            task();
        } else if (work_flag == 2) // 扫码测试，测试通信用
        {
            vision_subscribe_order();
            vision_sync();
        }
        if (work_flag == 3)
            vision_adjust_chassis(0, &points::TEST_STORAGE);
        if (work_flag == 4)
            process_place_task(0);
        if (work_flag == 5)
            materials_task(1);
        if (work_flag == 6)
            storage_place_task(0);
        else if (work_flag == 7)
        {
          cool_rgb_dynamic(1, 1000);
        }

        if (RC_flag == 1)
        {
            vision_remote_control_mode();
            remote_control_nowait();
        }
        servor_flag = 0;
        work_flag = 0;
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
