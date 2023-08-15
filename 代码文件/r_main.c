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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F100LG
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 2020/11/28
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_lcd.h"
#include "r_cg_KeyScan.h"
#include "r_cg_digitalLED.h"
#include "r_cg_Tdetect.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned int i,timerflag=0;
uint16_t n;
uint8_t  time[4];
char amp=0,amp1=0,amp2=0;
static char mix0[17]={""};
static char mix1[17]={""};
static char mix2[17]={""};
static char mix3[17]={""};
extern uint8_t tempkey;
extern uint8_t num_keyboard;
volatile char inputBuffer = 0;
static char bufferindex=0;
uint8_t time_s=0, time_m=0,inflag=0,pause=0;
void display(void);
void lcd_default(void);
void lcd_clear(void);
void lcd_timer(void);
void change_timerflag(void);
uint8_t line=0;
unsigned int a,b;
char str[17];
	
char map[16]={
	'=','0','.','/',
	'1','2','3','*',
	'4','5','6','-',
	'7','8','9','+'};

extern unsigned char count;
uint8_t array_key[2];
static order=0;

void delay(int t)
{
  int x = 0;
  int y = 0;
  for(x = 0; x < t;x++)
   for(y = 0; y < 1000;y++);
}
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    hdwinit();    
    R_MAIN_UserInit();
    R_TAU0_Channel0_Start();
    R_TAU0_Channel1_Start();
   // R_TAU0_Channel2_Start();	

		
	lcd_init( );	

        LcdFill_REC();
	delay(1000);

	LcdClear();     //Clear Graph;
	lcd_write(0x01,0);  

	WDTE = 0xAC;

	

	lcd_display((line+1)*16,"select moudel   ");
	delay(1000);
	while(1U)
	{

	 tempkey=Keyboard_scan();
	 
	
	 if(timerflag==2)
	 {
		 P4.1=1;
		  P4.2=0;
		 P4.3=1;
		         
		         lcd_display(line*16,"calc_input:     ");
		         lcd_display((line+1)*16,mix1);
		        lcd_display((line+2)*16,mix2);
	                lcd_display((line+3)*16,mix3);
			
                   if(tempkey!=0){    
			 inputBuffer = tempkey;
			
			 mix1[bufferindex++]=(inputBuffer==0) ? 0 : map[inputBuffer-1];
			 mix1[bufferindex]='\0';
			 if(bufferindex>4)
			 {
				 lcd_clear();
				 bufferindex=0;

			 }
			 if(bufferindex==4&&mix1[3]=='=')
			 {
				a=mix1[0]-'0';
				b=mix1[2]-'0';
				switch(mix1[1])
				{
					case '+':
					if(a+b<10)
					{
						mix2[0]='0';
						mix2[1]=a+b+'0';
					}
					else
					{
						mix2[0]='1';
						mix2[1]=a+b-10+'0';
					}
					break;
					case '-':
					if(a<b)
					{
						mix2[0]='-';
						mix2[1]=b-a+'0';
					}
					else
					{
						mix2[0]='0';
						mix2[1]=a-b+'0';
					}
					break;
					case '*':
					mix2[0]=a*b/10+'0';
					mix2[1]=a*b%10+'0';
					break;
					case '/':
					if(a<b)
					{
						mix2[0]='0';
						mix2[1]='.';
						mix2[2]=a*10/b+'0';
						
						if (((a*10%b)*10%b)*10/b>=5)
						{
							mix2[3]=(a*10%b)*10/b+1+'0';
						}
						else
						{
							mix2[3]=(a*10%b)*10/b+'0';
						}
					}
					else
					{
						mix2[0]='1';
						mix2[1]='.';
						mix2[2]=a*10/b+'0';
						mix2[3]=(a*10%b)*10/b+'0';
					}
					break;
					default:
					lcd_clear();
					break;
				}
				
			 }
			 if(mix1[0]=='.'&&mix1[1]=='=')
			 {
				 bufferindex=0;
				 lcd_clear();
				 change_timerflag();
			 }
			 
		   } 
		   
	 }
	  if(timerflag==1)
	 {
		 
                 P4.1=0;
		 P4.2=1;
		 P4.3=1;
			     
			     change_timerflag();
	 }
	 
	 if(timerflag==0)
	 {
                
		 if(time[2]*10+time[3]>20)
		 {
		 P4.1=0;
		 P4.2=0;
		 P4.3=0;
		 lcd_display((line+2)*16,"tempture high   ");
		 }
		 else
		 {
		  P4.1=1;
		 P4.2=1;
		 P4.3=1;
		 lcd_display((line+2)*16,"tempture normal ");
		 }
			 
		 change_timerflag();
	 }
	 

			
	
		n++;      // Temperature detection
		if(n%100==0) 
		{
			test();
			n=0;
		}

		WDTE = 0xAC;
	}
	
	
		
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void display(void)
{
	array_key[0]=count%10;    //the second char
	array_key[1]=count/10;    //the front char

	for(order=0;order<2;order++)
	{
	  DIGTAL_LED_D2=0;//turn off them before display, the number will be clearer.
	  DIGTAL_LED_D1=0;		
	  switch(order)
	  {
	    case 0:
		DIGTAL_PORT=Getcode(array_key[0]);// units
		DIGTAL_LED_D2=1;
		break;
		
	    case 1:
	    	DIGTAL_PORT=Getcode(array_key[1]);// tens
	        DIGTAL_LED_D1=1;
		break;	
	  }
	}
}

void lcd_default(void)
{
	  for(i=1;i<16;i++){
				mix0[i]=' ';
				mix1[i]=' ';
				mix2[i]=' ';
				mix3[i]=' ';}
	  mix0[0]='0';
	  mix1[0]='1';
	  mix2[0]='2';
	  mix3[0]='3';
}

void lcd_clear(void)
{
	for(i=0;i<16;i++){
				mix0[i]=' ';
				mix1[i]=' ';
				mix2[i]=' ';
				mix3[i]=' ';}
}

void lcd_timer(void)
{
	                     
		        lcd_display(line*16,mix0);
	                lcd_display((line+1)*16,"                ");
	                lcd_display((line+2)*16,"                ");
	                lcd_display((line+3)*16,"                ");
}

void change_timerflag(void)
{
	switch(tempkey)
		{
			case 1:
			timerflag=0;
			lcd_clear();
			lcd_display((line+1)*16,"select moudel   ");
			delay(1000);


				break;
			case 2:
			timerflag=1;
			lcd_clear();
			

				break;
			case 3:
			timerflag=1;
			lcd_clear();
	
				break;
			/*case 4:
			timerflag=1;
			  R_TAU0_Channel2_Start();
			  lcd_clear();
		

				break;*/
			case 5:
			timerflag=1;
			lcd_clear();
			time_m=0;
			amp=0;
			amp1=0;
			amp2=0;
			R_TAU0_Channel2_Stop();
			

				break;
			case 6:
			    timerflag=0;
			    lcd_clear();
			    lcd_default();


				break;
			case 8:
			lcd_clear();
			timerflag=2;
			
			

				break;
			case 4:
			timerflag=1;
				
			 
			  if(pause==1)
			  {
			  R_TAU0_Channel2_Start();
			  pause=0;
			  lcd_clear();
			  mix3[5]='t';
			  mix3[6]='i';
			  mix3[7]='m';
			  mix3[8]='i';
			  mix3[9]='n';
			  mix3[10]='g';
			  }
			  else
			  {
			   R_TAU0_Channel2_Stop();
			   pause=1;
			    lcd_clear();
			  mix3[5]='s';
			  mix3[6]='t';
			  mix3[7]='o';
			  mix3[8]='p';
		
			  }
			  
			 
		
				break;
			default:
			
	 lcd_display(line*16,mix0);
	 lcd_display((line+1)*16,mix1);
         lcd_display((line+2)*16,mix2);
	 lcd_display((line+3)*16,mix3);

				break;
		}
}
/* End user code. Do not edit comment generated here */
