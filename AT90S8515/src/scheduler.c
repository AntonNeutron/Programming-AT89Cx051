#include "scheduler.h"

/******************************************************************************************
 * ���������� ������
 */
volatile static task TaskArray[MAX_TASKS];      // ������� �����
volatile static uint8_t arrayTail;                  // "�����" �������
volatile static uint8_t FlagTimer =0;                  //
volatile static uint8_t delay_beep =0;  // ����� ���� (��� �����)

/******************************************************************************************
 * ������������� ����, ����� ���� - 1 ��
 */
inline void DS_Init()
{
   arrayTail = 0;                               // "�����" � 0
   TCCR0  = 0;					// ������������� ������
   TCNT0  = 131;					// ���������� ������� �������
  // OCR0   = 125;					// ������������� ������� ��������� (�� ���� ����� ������ ������, ����� ����� [����� CTC])
   // ����� ������ ���������� � �������� TIFR ����������� ������� 1 � ��������������� ������. 
   //��� �������� ����� ��������� ������ ����������� ��������, � �� � ������� ���������� ��� !!!!
   TIFR   = (1<< TOV0 );                   // ������� ���� ���������� ������� �0 �� ��������� 
   TIMSK  |= (1<<TOIE0);                  // ��������� ���������� �� ���������
   TCCR0  = (0<<CS02)|(1<<CS01)|(1<<CS00);   // ������ ������� ��������� - 64  ����� - ���
    //7		6	5	4	3	2	1	0
   //FOC0	WGM00	COM01	COM00	WGM01	CS02	CS01	CS00 
   //7 - �������������� ��������� ��������� ������ ��0 
   //6,3 - ����� ������ ������� �������� (00-Normal, 10-Phase correct PWM, 01-CTC(����� ��� ����������), 11-Fast PWM) 
   //5,4 - ����� ������ ����� ��������� 
   //2-0 - ���������� �������� �������, � ����������� 5 = 00000101 (101 - ������������ ����� 1024, �������� ������� ������� �� 1024)
   
 //  DDR_BEEP  |= (1<<Pin_BEEP);
 //  PORT_BEEP &= ~(1<<Pin_BEEP);   
}

/******************************************************************************************
 * ���������� ������ � ������
 */
void DS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod)
{
   uint8_t i;
   
   if(!taskFunc) return;
   for(i = 0; i < arrayTail; i++)                     // ����� ������ � ������� ������
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� �����, �� ��������� ����������
      {
         TaskArray[i].delay  = taskDelay;
         TaskArray[i].period = taskPeriod;
         TaskArray[i].run    = 0;   
         return;                                      // �������, �������
      }
   }

   if (arrayTail < MAX_TASKS)                         // ���� ����� ������ � ������ ��� 
   {                                                  // � ���� �����,�� ���������
      TaskArray[arrayTail].pFunc  = taskFunc;
      TaskArray[arrayTail].delay  = taskDelay;
      TaskArray[arrayTail].period = taskPeriod;
      TaskArray[arrayTail].run    = 0;   
      arrayTail++;                                    // ����������� "�����"
   }
}

/******************************************************************************************
 * �������� ������ �� ������
 */
void DS_DeleteTask (void (*taskFunc)(void))
{
   uint8_t i;
   
   for (i=0; i<arrayTail; i++)                        // �������� �� ������ �����
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� ������ � ������ �������
      {
         if(i != (arrayTail - 1))                     // ��������� ��������� ������
         {                                            // �� ����� ���������
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
         arrayTail--;                                 // ��������� ��������� "������"
         return;
      }
   }
}

/******************************************************************************************
 * ��������� ����, ���������� � main
 */

void DS_DispatchTask(){
   uint8_t i;
   void (*function) (void);
   
   // �������� �� ������ ����� ���� ���������� ���� ���������� �������
   if (FlagTimer) {
    FlagTimer = 0;  
    for (i=0; i<arrayTail; i++) {                       // �������� �� ������ �����
      if  (TaskArray[i].delay == 0)                   // ���� ����� �� ���������� �������
       TaskArray[i].run = 1;                      // ������� ���� �������,
      else TaskArray[i].delay--;                      // ����� ��������� �����
    }  
   } 
   
   for (i=0; i<arrayTail; i++)                        // �������� �� ������ �����
   {
      if (TaskArray[i].run == 1)                      // ���� ���� �� ���������� �������,
      {                                               // ���������� ������, �.�. ��
         function = TaskArray[i].pFunc;               // ����� ���������� ����� 
                                                      // ���������� ������
         if(TaskArray[i].period == 0)                 
         {                                            // ���� ������ ����� 0
            DS_DeleteTask(TaskArray[i].pFunc);      // ������� ������ �� ������,
            
         }
         else
         {
            TaskArray[i].run = 0;                     // ����� ������� ���� �������
            if(!TaskArray[i].delay)                   // ���� ������ �� �������� ��������
            {                                         // ������ ��
               TaskArray[i].delay = TaskArray[i].period-1; 
            }                                         // ������ ��� ���� ����� ������� �����  
         }
         (*function)();                               // ��������� ������
      }
   }
}

/******************************************************************************************
 * ��������� ������ ���� (���������� ����������� �������)
 */

/*void beep (uint8_t delay) {
 delay_beep = delay;  
}  */

ISR(TIMER0_OVF_vect){
  FlagTimer = 1; 
  TCNT0  = 131;
  //if (delay_beep) {
  // delay_beep --;  
  // PORT_BEEP  = PORT_BEEP ^ (1<<Pin_BEEP);  
   //BEEP_OUT  = BEEP_OUT ^ (1<<PD6);  
//  }   

}





