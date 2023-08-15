#include "r_cg_macrodriver.h"
#include "r_cg_Tdetect.h"
#include "r_cg_timer.h"

extern uint8_t time_s, time_m, delay_flag;
extern uint16_t times;

struct Temperature T;

//???????
void init(void)
{
   reset();
   delayTdetect(300);
}


void write_bit(uint8_t num)
{
   PIN_OUT();
   LOW();
   delayTdetect(1);
   if(num==1) PIN_IN();
   delayTdetect(65);
   PIN_IN();
}

uint8_t read_bit(void)
{
   uint8_t num;
   PIN_OUT();
   LOW();
   delayTdetect(3);
   PIN_IN();
   delayTdetect(3);
   num = P0.0;
   delayTdetect(65);
   return num;
}

uint16_t read_word(void)
{
   uint8_t i=0;
   uint8_t byte1=0,byte2=0, temp=0;
   uint16_t word=0;
   while(i<16)
   {
       if(i<8)   
           byte1 |= read_bit()<<i;
       else
	   byte2 |= read_bit()<<(i-8);
       i++;
   }
   word = byte2;
   word = word<<8;
   word |= (uint16_t)byte1;
   return word;
}

void write_byte(uint8_t byte)
{  
   uint8_t num;
   uint8_t i=0;
   while(i<8)
   {
     num = byte&0x01;
     write_bit(num);
     byte = byte>>1;
     i++;
   }
	
}

void test(void)
{
   uint16_t t_flag = 0;
   send_cmd(0xcc,0x44);  //????
   while(!P0.0);  //????????
   send_cmd(0xcc,0xbe);  //????
   t_flag = read_word();
   reset();
   transf(t_flag, &T);  //??????????????
   time_s = T.tem;
}

void reset(void)
{
   PIN_OUT();
   LOW();
   delayTdetect(500);
   PIN_IN();
}

void delayTdetect(uint16_t t)
{
   times = t;
   while(delay_flag);
   delay_flag = 1;
}

void transf(uint16_t temp, struct Temperature *p)
{
   p->sign = '+';
   if(temp&0x8000==0x8000) p->sign = '-';
   p->tem = (uint8_t)((temp&0x07f0)>>4);  //??4?????
}

void send_cmd(uint8_t rom_cmd, uint8_t func_cmd)
{
   init();
   write_byte(rom_cmd);
   write_byte(func_cmd);
}