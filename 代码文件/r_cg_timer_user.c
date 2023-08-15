/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G13 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F100LG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2020/11/28
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_Tdetect.h"
#include "r_cg_digitalLED.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern uint8_t time_s, time_m;
extern char amp,amp1,amp2;
volatile uint8_t flag_digital_led;
uint8_t ledflag=0;
uint8_t delay_flag=1;
uint16_t times, temp;
uint8_t  count=0;
extern uint8_t  time[4];
void display1(void);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */

	  display1();  //100 us刷新一次数码管显示
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */

    if(times){
	      temp++;
	      if(temp==times){
		      delay_flag = 0;
		      times = 0;
		      temp = 0;}
      }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    if(ledflag==1){
	    if(amp2==9)
	    {
		    amp2=0;
	    }
	    else
	    {
	    amp2++;}
	    ledflag=0;}
    if(time_m!=0)
    time_m=(time_m-1)%60;
    else
    {
	    ledflag=1;
	    time_m=59;
    }
    amp=time_m%10;
    amp1=time_m/10;

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void display1(void)
{
        time[3]=time_s%10;
	time[2]=time_s/10;
	time[1]=time_m%10;
	time[0]=time_m/10;
	P5 = 0x0;
	switch(count)
	{
		case 0:
		        DIGTAL_PORT = Getcode(time[count]);
			DIGTAL_LED_D1 = 1;break;
		case 1:
			DIGTAL_PORT = Getcode(time[count]);
		        DIGTAL_LED_D2 = 1;break;
		case 2:
			DIGTAL_PORT = Getcode(time[count]);
		        DIGTAL_LED_D3 = 1;break;
		case 3:
			DIGTAL_PORT = Getcode(time[count]);
		        DIGTAL_LED_D4 = 1;break;
		default:
			P5 = 0x0;
	}
	count++;
	if (count==4) count=0;
}
/* End user code. Do not edit comment generated here */
