// �� ������ ceptimus.  January 2016
#include <util/delay.h>
#include "programmer.h"
#include "scheduler.h"
#include "uart_pkg.h"

static uint16_t ChipAddress = 0x0000;
static uint8_t P1output = 0;
volatile static uint8_t Led_Blink = 0;
//volatile static uint8_t Chip

void PWM_start(void) {
   OCR1A = 100;
  _delay_ms(20);
}

void PWM_stop(void) {
  OCR1A = 0;
  _delay_ms(20);
}


void SetVoltageRST(uint8_t v) { // set RST/VPP pin to 12V, 5V or (default) 0V
  switch (v) {
    case 12:
      PWM_start();
      CONTROL_0V_RST = 0; // ������ �������
      break;
    case 5:
      CONTROL_0V_RST = 1; // ��������� �������
      PWM_stop();
      CONTROL_0V_RST = 0; // ������ �������
      break;
    default: // 0V
      OCR1A = 0;
      CONTROL_0V_RST = 1; // ��������� �������
  }  
}

void Chip_P1_SetMode(uint8_t output) { // ������������ ����� ����� ��� ����
  if (P1output == output) return;
  if (output) {
    PORTA = PORTA_MASK_OUTPUT; // ������ ������ ������� ��� ���� ���� ��� ������ �� � Hi-Z
    DDRA  = DDRA_MASK_OUTPUT; // ����������� ��� �����
  } else {
    DDRA  = DDRA_MASK_INPUT; //������ ���������� ��� ���� ����
    PORTA = PORTA_MASK_INPUT; 
  }
  P1output = output;
}

void Chip_on(void) {
  CONTROL_PWR = 0; // �������� ������� 
  Chip_P1_SetMode(0); // ���������� ���� ��� ����
  SetVoltageRST(0); // Reset on
  XTAL1 = 0;
  P3_3 = 0;
  P3_4 = 0;
  P3_5 = 0;
  P3_7 = 0;
  DDRC = DDRC_MASK_ON;
 
  _delay_ms(10);
  ChipAddress = 0x00; // ���������� ������� ������� PEROM ������������ �� 000H �� ������ RST
  SetVoltageRST(5); // ��������� ���
  P3_2 = 1;
  _delay_ms(1);
}

void Chip_off(void) {
  SetVoltageRST(0); // Reset on
  Chip_P1_SetMode(0); // ���������� ���� ��� ����
  XTAL1 = 0;
  CONTROL_PWR =1; // ��������� �������
  DDRC = DDRC_MASK_OFF;
  P3_2 = 0; 
  P3_3 = 0;
  P3_4 = 0;
  P3_5 = 0;
  P3_7 = 0;
  
}



void LED_GreenToggle(void){
   PORTB ^= (1 << LED_GREEN_PIN);
}

void LED_RedToggle(void){
   PORTB ^= (1 << LED_RED_PIN);
}


void LED_Off(void){
   LED_GREEN = 0;
   LED_RED = 0;
}


void Chip_writeP1(uint8_t b) {
  //Chip_P1setMode(1); ��� ������� ��������� ����� �� ���� ��� ����� ����� ������ ��������������� (����� �������� �������� ������� � ��� �� ������)
  // #define INVERT_BYTE(a)   ((a&1)<<7) | ((a&2)<<5) | ((a&4)<<3) | ((a&8)<<1) | ((a&16)>>1) | ((a&32)>>3) | ((a&64)>>5) | ((a&128)>>7) 
  PORT_P1_WRITE = INVERT_BYTE(b);
}

uint8_t Chip_readP1(void) {
  //Chip_P1setMode(0); ��� ������� ��������� ����� �� ���� ��� ����� ����� ������ ��������������� (����� �������� �������� ������� � ��� �� ������)
  return INVERT_BYTE(PORT_P1_READ);
}

void Chip_pulseClock(void) {
  // Increment Clock High 200 ns 
  XTAL1 = 1;
  _delay_us(1); // 1 microseconds
  XTAL1 = 0; 
  ChipAddress++;
}

void Peripheral_Ports_InitFirst(void) {
   DDRA  = DDRA_MASK_INPUT;
   PORTA = PORTA_MASK_INPUT;
   
   DDRB  = DDRB_MASK;
   PORTB = PORTB_MASK;
 
   DDRD  = DDRD_MASK;
   PORTD = PORTD_MASK;
 
   DDRC  = DDRC_MASK_OFF;
   PORTC = PORTC_MASK;
   
   uart_pkg_init(); //������������� UART, ���������� ����������  19200 8N1 (������ ��� 8MZh 0.2%)
   
   TCCR1A = (1 << COM1A1) | (1 << PWM10); // Set the OC1A non-inverted PWM		Timer/Counter1 is an 8-bit PWM
   TCCR1B = (1 << CS10); // Clock 1 Prescale = CK
   OCR1A = 0;
}

void DispatchUART(void)
{
   uint8_t cmd, i;//, ch;
   
   if (!Wake_TxAndRx_End()) return;
   cmd = Wake_GetCmd();
   if (cmd){
    switch(cmd){
       case CMD_CHIPSIG: // �������� ���������� � ����
	  Chip_on();
	  P3_3 = 0;
	  P3_4 = 0;
	  P3_5 = 0;
	  P3_7 = 0;
	  _delay_us(1);
	  i = 0;
	  cmd = Chip_readP1();
	  if ( cmd == 30 ) i++;
	  Wake_AddByte(cmd);
	  Chip_pulseClock();
	  cmd = Chip_readP1();
	  if ( (cmd == 33)|| (cmd == 65) ) i++;
          Wake_AddByte(cmd);
	  Chip_pulseClock();
	  Wake_AddByte(Chip_readP1());
	  Wake_StartTx(CMD_CHIPSIG);
	  Chip_off();
	  if ( i == 2 ) {
	    LED_RED = 0;
	    LED_GREEN = 1;
	  }
	  break;
       case CMD_CHIPERASE: // ������� ���
	  LED_GREEN= 0;
	  LED_RED = 1;  
	  Chip_on();
	  P3_3 = 1;
	  P3_4 = 0;
	  P3_5 = 0;
	  P3_7 = 0;
	  SetVoltageRST(12);
	  _delay_us(100);
	  P3_2 = 0;
          _delay_ms(10);
	  P3_2 = 1;
          _delay_ms(10);
          SetVoltageRST(5);
          Chip_off();
          Wake_StartTx(CMD_CHIPERASE);
	  LED_GREEN= 1;
	  LED_RED = 0;  
	  break;
       case CMD_CHIPON: // �������� ������� 
	  Chip_on();
          Wake_StartTx(CMD_CHIPON);
	  LED_Off();
	  break;
       case CMD_CHIPOFF: // ��������� ������ 
	  Chip_off();
          Wake_StartTx(CMD_CHIPOFF);
	  DS_DeleteTask(LED_GreenToggle);
	  DS_DeleteTask(LED_RedToggle);
	  Led_Blink = 0;
	  LED_Off();
	  LED_GREEN = 1;
	  break;
       case CMD_CHIPREAD: // ������ 16 ����
         if ( !Led_Blink ) { // ����������� ������ 1 ��� � ������ ������
	    DS_SetTask(LED_GreenToggle, 0, 100);
	    Led_Blink = 1;
	 }
	 P3_3 = 0;
	 P3_4 = 0;
	 P3_5 = 1;
	 P3_7 = 1;
	 _delay_us(1);
         Wake_AddWord(ChipAddress);
     	 for (i = 0; i < 16; i++) {
	    Wake_AddByte(Chip_readP1());
	    Chip_pulseClock();
	 }
	 Wake_StartTx(CMD_CHIPREAD);
	 break;
       case CMD_CHIPWRITE: //������ � ��� 16 ����
         if ( !Led_Blink ) {
	    DS_SetTask(LED_RedToggle, 0, 100);
	    Led_Blink = 1;
	 }
	 P3_3 = 0;
         P3_4 = 1;
         P3_5 = 1;
	 P3_7 = 1;
        SetVoltageRST(12);
        _delay_us(100);
        Chip_P1_SetMode(1); // ���������� ���� ��� �����
        for (i = 0; i < 16; i++) {
	  Chip_writeP1(Wake_GetByte());
          _delay_us(2);
          P3_2 = 0;
          _delay_us(2);
          P3_2 = 1;
          _delay_ms(2);
	  Chip_pulseClock();
        }
	Wake_AddWord(ChipAddress);
	Wake_StartTx(CMD_CHIPWRITE);
        break;
      case CMD_CHIPSETBIT:
      	LED_GREEN= 0;
	LED_RED = 1;  
	i = Wake_GetByte(); 
        Chip_on();
        SetVoltageRST(12);
       _delay_us(100);
	if (i & 1) {
          P3_3 = 1;
          P3_4 = 1;
	  P3_5 = 1;
          P3_7 = 1;
          P3_2 = 0;
          _delay_us(2);
          P3_2 = 1;
          _delay_ms(2);
	}
	if (i & 2) {
          P3_3 = 1;
          P3_4 = 1;
	  P3_5 = 0;
          P3_7 = 0;
          P3_2 = 0;
          _delay_us(2);
          P3_2 = 1;
          _delay_ms(2);
	}
        Chip_off();
        Wake_StartTx(CMD_CHIPSETBIT);
	LED_GREEN= 1;
	LED_RED = 0;  
	break; 
      case CMD_LED_OFF:
	 Chip_off();
	  DS_DeleteTask(LED_GreenToggle);
	  DS_DeleteTask(LED_RedToggle);
	  Led_Blink = 0;
	  LED_Off();
	  LED_RED = 1;
	 break;
    } //switch 
    Wake_EndCmd(); // ��������� ��������� ��������� ������  
   }
}

