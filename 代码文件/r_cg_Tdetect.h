#ifndef R_CG_TDETECT_H_
#define R_CG_TDETECT_H_

#define PIN_OUT() (PM0 = 0x00)
#define PIN_IN() (PM0 = 0x01)
#define LOW()  (P0.0=0)  
#define HIGH() (P0.0=1)

struct Temperature{
    char sign;  //????
    uint8_t tem;  //????
};
void delayTdetect(uint16_t t);
uint16_t read_word(void);
void init(void);
void write_bit(uint8_t num);
uint8_t read_bit(void);
void write_byte(uint8_t byte);
void test(void);
void reset(void);
void transf(uint16_t temp, struct Temperature *p);
void send_cmd(uint8_t rom_cmd, uint8_t func_cmd);

#endif