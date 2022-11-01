#include "scheduler.h"

/******************************************************************************************
 * Переменные модуля
 */
volatile static task TaskArray[MAX_TASKS];      // очередь задач
volatile static uint8_t arrayTail;                  // "хвост" очереди
volatile static uint8_t FlagTimer =0;                  //
volatile static uint8_t delay_beep =0;  // длина бипа (это опция)

/******************************************************************************************
 * Инициализация РТОС, время тика - 1 мс
 */
inline void DS_Init()
{
   arrayTail = 0;                               // "хвост" в 0
   TCCR0  = 0;					// останавливаем таймер
   TCNT0  = 131;					// сбрасываем счетный регистр
  // OCR0   = 125;					// устанавливаем регистр сравнения (до него будет тикать таймер, потом сброс [режим CTC])
   // Сброс флагов прерываний в регистре TIFR выполняется записью 1 в соответствующий разряд. 
   //Эту операцию нужно выполнять именно перезаписью регистра, а не с помощью побитового ИЛИ !!!!
   TIFR   = (1<< TOV0 );                   // очищаем флаг прерывания таймера Т0 по сравнению 
   TIMSK  |= (1<<TOIE0);                  // разрешаем прерывание по сравнению
   TCCR0  = (0<<CS02)|(1<<CS01)|(1<<CS00);   // запуск таймера прескалер - 64  режим - СТС
    //7		6	5	4	3	2	1	0
   //FOC0	WGM00	COM01	COM00	WGM01	CS02	CS01	CS00 
   //7 - принудительное изменение состояния вывода ОС0 
   //6,3 - режим работы таймера счетчика (00-Normal, 10-Phase correct PWM, 01-CTC(Сброс при совпадении), 11-Fast PWM) 
   //5,4 - режим работы блока сравнения 
   //2-0 - определяют источник сигнала, и пределитель 5 = 00000101 (101 - предделитель равен 1024, тактовая частота делится на 1024)
   
 //  DDR_BEEP  |= (1<<Pin_BEEP);
 //  PORT_BEEP &= ~(1<<Pin_BEEP);   
}

/******************************************************************************************
 * Добавление задачи в список
 */
void DS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod)
{
   uint8_t i;
   
   if(!taskFunc) return;
   for(i = 0; i < arrayTail; i++)                     // поиск задачи в текущем списке
   {
      if(TaskArray[i].pFunc == taskFunc)              // если нашли, то обновляем переменные
      {
         TaskArray[i].delay  = taskDelay;
         TaskArray[i].period = taskPeriod;
         TaskArray[i].run    = 0;   
         return;                                      // обновив, выходим
      }
   }

   if (arrayTail < MAX_TASKS)                         // если такой задачи в списке нет 
   {                                                  // и есть место,то добавляем
      TaskArray[arrayTail].pFunc  = taskFunc;
      TaskArray[arrayTail].delay  = taskDelay;
      TaskArray[arrayTail].period = taskPeriod;
      TaskArray[arrayTail].run    = 0;   
      arrayTail++;                                    // увеличиваем "хвост"
   }
}

/******************************************************************************************
 * Удаление задачи из списка
 */
void DS_DeleteTask (void (*taskFunc)(void))
{
   uint8_t i;
   
   for (i=0; i<arrayTail; i++)                        // проходим по списку задач
   {
      if(TaskArray[i].pFunc == taskFunc)              // если задача в списке найдена
      {
         if(i != (arrayTail - 1))                     // переносим последнюю задачу
         {                                            // на место удаляемой
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
         arrayTail--;                                 // уменьшаем указатель "хвоста"
         return;
      }
   }
}

/******************************************************************************************
 * Диспетчер РТОС, вызывается в main
 */

void DS_DispatchTask(){
   uint8_t i;
   void (*function) (void);
   
   // проходим по списку задач если установлен флаг прерывания таймера
   if (FlagTimer) {
    FlagTimer = 0;  
    for (i=0; i<arrayTail; i++) {                       // проходим по списку задач
      if  (TaskArray[i].delay == 0)                   // если время до выполнения истекло
       TaskArray[i].run = 1;                      // взводим флаг запуска,
      else TaskArray[i].delay--;                      // иначе уменьшаем время
    }  
   } 
   
   for (i=0; i<arrayTail; i++)                        // проходим по списку задач
   {
      if (TaskArray[i].run == 1)                      // если флаг на выполнение взведен,
      {                                               // запоминаем задачу, т.к. во
         function = TaskArray[i].pFunc;               // время выполнения может 
                                                      // измениться индекс
         if(TaskArray[i].period == 0)                 
         {                                            // если период равен 0
            DS_DeleteTask(TaskArray[i].pFunc);      // удаляем задачу из списка,
            
         }
         else
         {
            TaskArray[i].run = 0;                     // иначе снимаем флаг запуска
            if(!TaskArray[i].delay)                   // если задача не изменила задержку
            {                                         // задаем ее
               TaskArray[i].delay = TaskArray[i].period-1; 
            }                                         // задача для себя может сделать паузу  
         }
         (*function)();                               // выполняем задачу
      }
   }
}

/******************************************************************************************
 * Таймерная служба РТОС (прерывание аппаратного таймера)
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





