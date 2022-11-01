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


// � ����� �������� ����������������� � ��������� ���������� ������������� ��� ���������������� ���� �������� � ����� PullUp
//_________________________________________
//           ����         76543210
//���� P1 ������ ����������  P1.0,P1.1,P1.2,P1.3,P1.4,P1.5,P1.6,P1.7
#define DDRA_MASK_OUTPUT 	0b11111111	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTA_MASK_OUTPUT 	0b00000000	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

#define DDRA_MASK_INPUT 	0b00000000	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTA_MASK_INPUT 	0b11000000	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

// LED R G, and PWR CHIP
#define DDRB_MASK 	0b00010011	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTB_MASK 	0b11111110	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

// UART
#define DDRD_MASK 	0b00100010	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTD_MASK 	0b11011100	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1

// //���� P3+XTAL		XTAL,P3.7,P3.5,P3.4,P3.3,P3.2,P3.1,P3.0, 0V
#define DDRC_MASK_OFF 	0b00000001	//��������� �������� ����� x �� ���� ��� �����. 0 � ����� �������� ��� ����. 1 - ����� �������� �� �����.
#define PORTC_MASK 	0b00000011	//���������� ���������� ������ ����� 0 - Hi-Z, 1 - PullUp, ���� ����� 0- ���0, 1- ���1
#define DDRC_MASK_ON	0b11111101	// ����� ����� P3

#define	CONTROL_PWR	SBIT( PORTB, 4 )
#define	CONTROL_0V_RST	SBIT( PORTC, 0 )

#define	LED_GREEN	SBIT( PORTB, 0 )
#define	LED_RED		SBIT( PORTB, 1 )
#define	LED_GREEN_PIN	0
#define	LED_RED_PIN	1	
//#define	CONTROL_5V	SBIT( PORTC, 6 )


#define PORT_P1_WRITE PORTA	// ���������� ��� ������ � ����
#define PORT_P1_READ  PINA	// ���������� ��� ������ �� �����
//����� ������ ���� �� ������� ��������� � ������ AT89 ����� ������ ����� ���� ����, � �� �� �����. ������ ��������� ��������� ���� � �����
//#define P1_0 	SBIT( PORTA, 7 )
//#define P1_1 	SBIT( PORTA, 6 )
//#define P1_2 	SBIT( PORTA, 5 )
//#define P1_3 	SBIT( PORTA, 4 )
//#define P1_4 	SBIT( PORTA, 3 )
//#define P1_5 	SBIT( PORTA, 2 )
//#define P1_6 	SBIT( PORTA, 1 )
//#define P1_7 	SBIT( PORTA, 0 )

//#define P3_0 	SBIT( PORTB, 0 )
//#define P3_1 	SBIT( PINB, 1 ) // ���� ��������� AT89 (�� ������������)

// ����� ���������� �����������������
#define P3_2 	SBIT( PORTC, 5 )
#define P3_3 	SBIT( PORTC, 4 )
#define P3_4 	SBIT( PORTC, 3 )
#define P3_5 	SBIT( PORTC, 2 )
#define P3_7 	SBIT( PORTC, 6 )

#define XTAL1 	SBIT( PORTC, 7 )

void Peripheral_Ports_InitFirst(void);
void DispatchUART(void);
#endif

