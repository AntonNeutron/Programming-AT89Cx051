//----------------------------------------------------------------------------

//Константы протокола WAKE:

//----------------------------------------------------------------------------

#ifndef uart_pkg_h
#define uart_pkg_h
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr\pgmspace.h>


//----------------------------------------------------------------------------

//Коды стандартных команд:

#define CMD_NOP     0 //нет операции
#define CMD_ERR     1 //ошибка приема пакета
#define CMD_ECHO    2 //передать эхо
#define CMD_INFO    3 //передать информацию об устройстве

//Коды ошибок:

#define ERR_NO      0 //no error
#define ERR_TX      1 //Rx/Tx error
#define ERR_BU      2 //device busy error
#define ERR_RE      3 //device not ready error
#define ERR_PA      4 //parameters value error
#define ERR_NR      5 //no replay
#define ERR_NC      6 //no carrier

#define ADDR_KEY 0xBEDA  //ключ для установки адреса

//-------------------------- Макроопределения: -------------------------------

#define BAUD2CODE(b) ((unsigned int)((F_CLK * 1E6)/(16.0 * b) - 0.5))

//-------------------------- Прототипы функций: ------------------------------

void uart_pkg_init(void); //инициализация модуля
//void Wake_Exe(bool t);       //задержка передачи ответа по RS-485
//uint8_t Wake_SetAddr(unsigned int sig, uint8_t addr); //установка адреса устройства
//char Wake_GetAddr(void);     //чтение адреса устройства

uint8_t Wake_GetCmd(void);      //возвращает текущий код команды
void Wake_EndCmd(void);		//Конец обработки входящего пакета
uint8_t Wake_GetRxCount(void);  //возвращает количество принятых байт
void Wake_SetRxPtr(uint8_t p);  //устанавливает указатель буфера приема
uint8_t Wake_GetRxPtr(void);    //читает указатель буфера приема
uint8_t Wake_GetByte(void);     //читает байт из буфера приема
int8_t Wake_GetInt8_t(void); // знаковое
int16_t Wake_GetInt(void);     //читает слово из буфера приема
uint16_t Wake_GetWord(void);     //читает слово из буфера приема
//long Wake_GetDWord(void);    //читает двойное слово из буфера приема
void Wake_GetData(uint8_t *d, uint8_t count); //читает данные из буфера приема

void Wake_SetTxPtr(uint8_t p);  //устанавливает указатель буфера передачи
uint8_t Wake_GetTxPtr(void);    //читает указатель буфера передачи
void Wake_AddByte(uint8_t b);   //помещает байт в буфер передачи
void Wake_AddInt(int16_t w);    //помещает слово в буфер передачи
void Wake_AddWord(uint16_t w);    //помещает слово в буфер передачи
void Wake_AddDWord(long dw); //помещает двойное слово в буфер передачи
void Wake_AddData(uint8_t *d, uint8_t count); //помещает данные в буфер передачи
void Wake_StartTx(uint8_t cmd); //начало передачи пакета
uint8_t Wake_TxAndRx_End(void);    //определение конца передачи пакета

//----------------------------------------------------------------------------

#endif
