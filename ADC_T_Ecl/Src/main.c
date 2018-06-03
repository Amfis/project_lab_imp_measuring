
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "opamp.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#define U0 3
#define ARM_MATH_CM4
#include "arm_math.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#define DACSAMPLE 32
#define ADCSAMPLE 100
void Set_DAC_Freq(uint32_t );
void Set_ADC_Freq(uint32_t );
void Sin_Gen(void);
uint16_t NUM_SAMPLES_DAC =DACSAMPLE;
uint16_t NUM_SAMPLES_ADC =ADCSAMPLE;

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint32_t ADC1ConvertedValues[ADCSAMPLE];
uint16_t Sine_Lut[DACSAMPLE], Sine_Lut_180[DACSAMPLE];
uint32_t  Voltage_val[ADCSAMPLE], Current_val[ADCSAMPLE];
uint32_t  FFT_Data[ADCSAMPLE];
float X_Real_V[ADCSAMPLE] , X_Imag_V[ADCSAMPLE],X_Real_C[ADCSAMPLE] , X_Imag_C[ADCSAMPLE],Phase_V[ADCSAMPLE],Abs_C[ADCSAMPLE],Abs_V[ADCSAMPLE];
float Imp[ADCSAMPLE]={0},real[ADCSAMPLE]={0},imag[ADCSAMPLE]={0};
arm_rfft_instance_q31 * S;
void calc();
void fft();
 uint8_t busy=0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Sin_Gen();//Generates Sine wave

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  Set_DAC_Freq(100000);
  Set_ADC_Freq(200000);
  //HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC1ConvertedValues, 2048);


  //HAL_ADCEx_MultiModeStart_DMA(&hadc1, ADC1ConvertedValues, sizeof(ADC1ConvertedValues));
  HAL_OPAMP_Start(&hopamp1);
  HAL_OPAMP_Start(&hopamp2);
    HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)Sine_Lut, NUM_SAMPLES_DAC, DAC_ALIGN_12B_R);
    //HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Sine_Lut_180, NUM_SAMPLES_DAC, DAC_ALIGN_12B_R);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    //HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADCEx_MultiModeStart_DMA(&hadc1, ADC1ConvertedValues, (sizeof(ADC1ConvertedValues)));
   //HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC1ConvertedValues, sizeof(ADC1ConvertedValues));
    HAL_TIM_Base_Start(&htim6);
    HAL_TIM_Base_Start(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void Sin_Gen()
{

  	for (int i=0; i< NUM_SAMPLES_DAC; i++)
  	{

  		Sine_Lut[i] = (0.8*cosf(((2*PI)/NUM_SAMPLES_DAC)*i)+1)*682;

  		Sine_Lut_180[i] = (0.8*cosf(i*2*PI/NUM_SAMPLES_DAC+PI)+1)*682;

  	}

}
void Set_DAC_Freq(uint32_t freq)
{
uint32_t	time ;
time=(RCC_FREQ/(NUM_SAMPLES_DAC*freq))-1;
htim6.Init.Period=time;
if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
 {
   _Error_Handler(__FILE__, __LINE__);
 }

}
void Set_ADC_Freq(uint32_t freq)
{
	 uint32_t	time ;
time=(RCC_FREQ/(freq))-1;
htim1.Init.Period=time;
if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
 {
   _Error_Handler(__FILE__, __LINE__);
 }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(hadc);
  if(hadc->Instance==ADC1)
   {
	 // HAL_ADCEx_MultiModeStop_DMA(&hadc1);
//	  HAL_TIM_Base_Stop(&htim6);
	 calc();
   }

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */

}


void calc(){
if(busy==1)
{
	return;
}
busy=1;
for(int i=0;i<ADCSAMPLE;i++)
{
	Voltage_val[i]=ADC1ConvertedValues[i]&0xffff;
    Current_val[i]=(ADC1ConvertedValues[i]&0xffff0000)>>16;

}
fft();

}

uint32_t N=ADCSAMPLE;
void fft()
{
	for(int k=0;k<N;k++)
	 {
	  X_Real_V[k] = X_Imag_V[k]=0.0;
	  X_Real_C[k] = X_Imag_C[k]=0.0;
	  Imp[k]=0;
	    for(int n=0;n<N;n++)
	     {
	       X_Real_V[k]+=Voltage_val[n]*cosf((2*PI*k*(n))/N);
	       X_Imag_V[k]+=Voltage_val[n]*sinf((2*PI*k*(n))/N);
	       X_Real_C[k]+=Current_val[n]*cosf((2*PI*k*(n))/N);
	       X_Imag_C[k]+=Current_val[n]*sinf((2*PI*k*(n))/N);

	     }
	    X_Imag_V[k]=(2*X_Imag_V[k]/N)*U0/4096;
	    X_Real_C[k]=(2*X_Real_C[k]/N)*U0/4096;
	    X_Real_V[k]=(2*X_Real_V[k]/N)*U0/4096;
	    X_Imag_C[k]=(2*X_Imag_C[k]/N)*U0/4096;
	    X_Imag_V[k]=X_Imag_V[k]*(-1.0);
	     X_Imag_C[k]=X_Imag_C[k]*(-1.0);
	     Abs_V[k]=sqrt((X_Imag_V[k]*X_Imag_V[k])+(X_Real_V[k]*X_Real_V[k]));
	     Abs_V[k]=Abs_V[k]>0.2?Abs_V[k]:0;
	   	Abs_C[k]=sqrt((X_Imag_C[k]*X_Imag_C[k])+(X_Real_C[k]*X_Real_C[k]));
	   	Abs_C[k]=Abs_C[k]>0.2?Abs_C[k]:0.01;
	   	Imp[k]=(Abs_V[k]/Abs_C[k])*900;
	   	real[k]=   (X_Real_V[k]/ X_Real_C[k])*900;
	   	imag[k]=(X_Imag_V[k]/ X_Imag_C[k])*900;

	 }


}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
