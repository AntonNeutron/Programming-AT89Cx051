//----------------------------------------------------------------------------

//��������� ��������� WAKE:

//----------------------------------------------------------------------------

#ifndef uart_pkg_h
#define uart_pkg_h
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr\pgmspace.h>


//----------------------------------------------------------------------------

//���� ����������� ������:

#define CMD_NOP     0 //��� ��������
#define CMD_ERR     1 //������ ������ ������
#define CMD_ECHO    2 //�������� ���
#define CMD_INFO    3 //�������� ���������� �� ����������

//���� ������:

#define ERR_NO      0 //no error
#define ERR_TX      1 //Rx/Tx error
#define ERR_BU      2 //device busy error
#define ERR_RE      3 //device not ready error
#define ERR_PA      4 //parameters value error
#define ERR_NR      5 //no replay
#define ERR_NC      6 //no carrier

#define ADDR_KEY 0xBEDA  //���� ��� ��������� ������

//-------------------------- ����������������: -------------------------------

#define BAUD2CODE(b) ((unsigned int)((F_CLK * 1E6)/(16.0 * b) - 0.5))

//-------------------------- ��������� �������: ------------------------------

void uart_pkg_init(void); //������������� ������
//void Wake_Exe(bool t);       //�������� �������� ������ �� RS-485
//uint8_t Wake_SetAddr(unsigned int sig, uint8_t addr); //��������� ������ ����������
//char Wake_GetAddr(void);     //������ ������ ����������

uint8_t Wake_GetCmd(void);      //���������� ������� ��� �������
void Wake_EndCmd(void);		//����� ��������� ��������� ������
uint8_t Wake_GetRxCount(void);  //���������� ���������� �������� ����
void Wake_SetRxPtr(uint8_t p);  //������������� ��������� ������ ������
uint8_t Wake_GetRxPtr(void);    //������ ��������� ������ ������
uint8_t Wake_GetByte(void);     //������ ���� �� ������ ������
int8_t Wake_GetInt8_t(void); // ��������
int16_t Wake_GetInt(void);     //������ ����� �� ������ ������
uint16_t Wake_GetWord(void);     //������ ����� �� ������ ������
//long Wake_GetDWord(void);    //������ ������� ����� �� ������ ������
void Wake_GetData(uint8_t *d, uint8_t count); //������ ������ �� ������ ������

void Wake_SetTxPtr(uint8_t p);  //������������� ��������� ������ ��������
uint8_t Wake_GetTxPtr(void);    //������ ��������� ������ ��������
void Wake_AddByte(uint8_t b);   //�������� ���� � ����� ��������
void Wake_AddInt(int16_t w);    //�������� ����� � ����� ��������
void Wake_AddWord(uint16_t w);    //�������� ����� � ����� ��������
void Wake_AddDWord(long dw); //�������� ������� ����� � ����� ��������
void Wake_AddData(uint8_t *d, uint8_t count); //�������� ������ � ����� ��������
void Wake_StartTx(uint8_t cmd); //������ �������� ������
uint8_t Wake_TxAndRx_End(void);    //����������� ����� �������� ������

//----------------------------------------------------------------------------

#endif
