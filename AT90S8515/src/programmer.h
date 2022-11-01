// AT89C51 connecting pin definitions and routines
// ceptimus.  January 2016

#ifndef programmer_h
#define programmer_h
#include "mydefs.h"

#define CMD_ERRORCRC  4
#define CMD_CHIPSIG   5
#define CMD_CHIPERASE 6
#define CMD_CHIPON    7
#define CMD_CHIPOFF   8
#define CMD_CHIPREAD  9
#define CMD_CHIPWRITE  10
#define CMD_CHIPSETBIT 11
#define CMD_LED_OFF	12


// С целью снижения энергопотребления и повышения надежности рекомендуется все неиспользованные пины включить в режим PullUp
//_________________________________________
//           биты         76543210
//Порт P1 только перевернут  P1.0,P1.1,P1.2,P1.3,P1.4,P1.5,P1.6,P1.7
#define DDRA_MASK_OUTPUT 	0b11111111	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTA_MASK_OUTPUT 	0b00000000	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

#define DDRA_MASK_INPUT 	0b00000000	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTA_MASK_INPUT 	0b11000000	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

// LED R G, and PWR CHIP
#define DDRB_MASK 	0b00010011	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTB_MASK 	0b11111110	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

// UART
#define DDRD_MASK 	0b00100010	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTD_MASK 	0b11011100	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1

// //Порт P3+XTAL		XTAL,P3.7,P3.5,P3.4,P3.3,P3.2,P3.1,P3.0, 0V
#define DDRC_MASK_OFF 	0b00000001	//Настройка разрядов порта x на вход или выход. 0 — вывод работает как ВХОД. 1 - вывод работает на ВЫХОД.
#define PORTC_MASK 	0b00000011	//Управление состоянием входов порта 0 - Hi-Z, 1 - PullUp, если выход 0- лог0, 1- лог1
#define DDRC_MASK_ON	0b11111101	// Маска порта P3

#define	CONTROL_PWR	SBIT( PORTB, 4 )
#define	CONTROL_0V_RST	SBIT( PORTC, 0 )

#define	LED_GREEN	SBIT( PORTB, 0 )
#define	LED_RED		SBIT( PORTB, 1 )
#define	LED_GREEN_PIN	0
#define	LED_RED_PIN	1	
//#define	CONTROL_5V	SBIT( PORTC, 6 )


#define PORT_P1_WRITE PORTA	// переменная для записи в порт
#define PORT_P1_READ  PINA	// переменная для чтения из порта
//Почти удачно порт на мастере совпадает с портом AT89 будем читать сразу весь порт, а не по битам. Только прийдется развенуть биты в байте
//#define P1_0 	SBIT( PORTA, 7 )
//#define P1_1 	SBIT( PORTA, 6 )
//#define P1_2 	SBIT( PORTA, 5 )
//#define P1_3 	SBIT( PORTA, 4 )
//#define P1_4 	SBIT( PORTA, 3 )
//#define P1_5 	SBIT( PORTA, 2 )
//#define P1_6 	SBIT( PORTA, 1 )
//#define P1_7 	SBIT( PORTA, 0 )

//#define P3_0 	SBIT( PORTB, 0 )
//#define P3_1 	SBIT( PINB, 1 ) // Вход занятости AT89 (Не используется)

// порты управления программированием
#define P3_2 	SBIT( PORTC, 5 )
#define P3_3 	SBIT( PORTC, 4 )
#define P3_4 	SBIT( PORTC, 3 )
#define P3_5 	SBIT( PORTC, 2 )
#define P3_7 	SBIT( PORTC, 6 )

#define XTAL1 	SBIT( PORTC, 7 )

void Peripheral_Ports_InitFirst(void);
void DispatchUART(void);
#endif

