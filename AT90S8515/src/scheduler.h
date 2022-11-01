#ifndef _scheduler_h_
#define _scheduler_h_
#include <avr/io.h>
#include <avr/interrupt.h>

/******************************************************************************************
 * За основу взят планировщик задач с сайта ChipEnable.ru                                 *
 * http://chipenable.ru/index.php/programming-avr/item/110-planirovschik.html             *
 *                                                                                        *
 * Доработал Шибанов Владимир aka KontAr                                                  *
 * Дата: 26.03.2014                                                                       *
 *                                                                                        *
 * Изменения:                                                                             *
 * - добавлен однократный вызов задачи                                                    *
 * - добавлено удаление задачи по имени                                                   *
 * - при повторном добавлении задачи обновляются ее переменные                            *
 * - добавлен указатель на "хвост" списка                                                 *
 * - функции РТОС скорректированы с учетом "хвоста"                                       *
 ******************************************************************************************/

#define MAX_TASKS      5               // Количество задач

//#define PORT_BEEP 	PORTD 	
//#define DDR_BEEP 	DDRD
//#define Pin_BEEP 	5

/******************************************************************************************
 * Структура задачи
 */
typedef struct {   
   void (*pFunc) (void);                    // указатель на функцию
   uint16_t delay;                          // задержка перед первым запуском задачи
   uint16_t period;                         // период запуска задачи
   uint8_t  run;                            // флаг готовности задачи к запуску
}task;

/******************************************************************************************
 * Прототипы фукнций
 */
void DS_Init (void);
void DS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod);
void DS_DeleteTask (void (*taskFunc)(void));
void DS_DispatchTask (void);
//void beep (uint8_t delay);

#endif
