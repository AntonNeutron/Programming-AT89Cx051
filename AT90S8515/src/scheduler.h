#ifndef _scheduler_h_
#define _scheduler_h_
#include <avr/io.h>
#include <avr/interrupt.h>

/******************************************************************************************
 * �� ������ ���� ����������� ����� � ����� ChipEnable.ru                                 *
 * http://chipenable.ru/index.php/programming-avr/item/110-planirovschik.html             *
 *                                                                                        *
 * ��������� ������� �������� aka KontAr                                                  *
 * ����: 26.03.2014                                                                       *
 *                                                                                        *
 * ���������:                                                                             *
 * - �������� ����������� ����� ������                                                    *
 * - ��������� �������� ������ �� �����                                                   *
 * - ��� ��������� ���������� ������ ����������� �� ����������                            *
 * - �������� ��������� �� "�����" ������                                                 *
 * - ������� ���� ��������������� � ������ "������"                                       *
 ******************************************************************************************/

#define MAX_TASKS      5               // ���������� �����

//#define PORT_BEEP 	PORTD 	
//#define DDR_BEEP 	DDRD
//#define Pin_BEEP 	5

/******************************************************************************************
 * ��������� ������
 */
typedef struct {   
   void (*pFunc) (void);                    // ��������� �� �������
   uint16_t delay;                          // �������� ����� ������ �������� ������
   uint16_t period;                         // ������ ������� ������
   uint8_t  run;                            // ���� ���������� ������ � �������
}task;

/******************************************************************************************
 * ��������� �������
 */
void DS_Init (void);
void DS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod);
void DS_DeleteTask (void (*taskFunc)(void));
void DS_DispatchTask (void);
//void beep (uint8_t delay);

#endif
