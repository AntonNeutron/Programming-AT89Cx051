//----------------------------------------------------------------------------
// ?????: ?????? ????????
//?????? ?????????? e

//?????? ??????: FEND, ADD, CMD, LNG, DAT0..DATN, CRC
//?????? ?????? ????? FEND ? FESC ?????????? ????????????????????
//TEND-FEND ? TFESC-FESC (????????). ????? ?????????? ? ?????????????
//??????? ?????. ??????? ?????? ????? ??????? ??????? ???. ??? ?????????
//?? ?????????? ????? ??? ?????? ?????-?????. ????? ???? ?????? ?
//?????? ????? ???? ?? 0 ?? 255 ????. CRC ??????????? ?? ????????
//X^8 + X^5 + X^4 + 1 ? ???????????????? ????????? 0xDE. CRC ???????????
//??? ????????? ???????? ?????? (????? ???????? ??????? ??? ?????
//????????). ???? ????? ?? ??????????, ?? ?? ????????? ? ??????? CRC.

//??? ????, ????? ?????? ????? ???? ???????????? ? ??????????????????,
//???????? ?????? ?????????? UART, ?????????? ???????? ???? ?????????
//? ????? UART. ????? ?????? ??? ????? ? ???????????? ? ???????
//????????? ????????????? UART.

//????? ?????? ???????????? ? ?????????? Rx_Int().
//???? ????????? ?????? ?????? ?????? FEND, ?????????? ?????.
//? ?????????? ???????????? ?????????? ??????.
//????? ?????? ?????? ?? ???????????? ? ???????? ??????? Addr,
//???? ????? ?? ??????, ????????? ?????? ???????????? ?? ??????
//?????????? ??????? ?????? ?????? FEND.
//???? ????? ??????, ????? ?????? ????????????. ???????? ?????
//??????????? ? ?????? ?????? RxData. ????? ????? ?????? ????????,
//????????? ?????? ?????? ??????????????? ? WST_DONE.
//? ????? ????????? ????????? ?? ????????? ?? ??????????
//??????????? ??????.

//??? ???????? ?????? ?????? ???????????? ?????? ?????????? ???????
//Wake_GetCmd(). ???? ? ?????? ?????? ????????? ???????? ?????,
//??? ??????? ???????????? ??????????? ?????. ???? ??? ?????????,
//??????? ?????????? ??? ??????? (????? ??????? ?????????? ???
//?????? ?????? ?????? CMD_ERR). ???? ??? ??????? ?? CMD_NOP,
//?????? ?????? ?????, ? ????????? ??????? ???? ??????????.
//??? ???? ??????? Wake_GetCmd() ????????????? ????????? ???????
//?????? ? ???????? ?? ?????? ??????.
//???????? ?????? ????? ????????? ?? ?????? ??? ?????? ???????
//Wake_GetByte(), Wake_GetWord(), Wake_GetDWord(), ???????
//?????????? 1, 2 ? 4 ????? ??????????????. ??? ????????
//Wake_GetData(), ??????? ???????? ?? ????????? ?????????
//???????? ?????????? ????? ?????? ?? ??????.
//????? ??????????? ?? ?????? ???????????????.
//?????????? ???????? ???? ????? ?????? ? ??????? ??????? Wake_GetRxCount().
//??? ????????? ??????? ? ????????????? ????? ?????? ??????? ???????
//????????? ????????? Wake_SetRxPtr().

//????? ?????????? ??????? ??????, ??????????????? ??? ????????,
//????? ????????? ? ????? ???????? ? ??????? ??????? Wake_AddByte(),
//Wake_AddWord(), Wake_AddDWord(). ??????? ????????? 1, 2 ? 4 ?????
//??????????????. ????? ??????????? ? ????? ???????????????. ??? ????????
//Wake_AddData(), ??????? ???????? ? ????? ????? ??????, ???????????
//?? ????????? ?????????. ??? ????????? ??????? ? ????????????? ?????
//?????? ??????? ??????? ????????? ????????? Wake_SetTxPtr().
//????? ?????? ? ????? ?????????, ????? ?????????? ?????, ??? ????
//????? ??????? ??????? Wake_StartTx(). ????? ?????? ????????????
//????????????? ?? ???????? ????????? ?? ?????? ?????? Wake_StartTx().
//??? ??????? ???????????? ??????????? ????? ??? ????? ?????? ? ????????
//????????. ???????????? ??? ????????? ????? ?????????? ??????.

//??????????? ??????? CMD_ERR, CMD_ECHO, CMD_INFO, CMD_SETADDR,
//CMD_GETADDR ????????????? ???? ???????. ????????? ???????
//?????? ???? ?????????? ??????? ???????.

//??? ?????? ? ?????? RS-485 ????? ?????????? ?????? RS485.
//??? ???? ????? ?????????????? ?????????? ????????????.
//??? ????? ?????? ???? ????????? ???? DIR ? ???????????????
//??????? Port_DIR_1 ? Port_DIR_0.

//???????? ???????????? ???????? ?????? ?? ???????, ??? ????? ????
//?????????? ? ?????? RS-485 ??? ????, ????? ?? ??????? ??????????
//???? ????? ????????????? ?? ?????. ???????? ???????? ? ???????
//????????? TX_DELAY ? ?????????????. ???????????? ???????? ??????????
//??????? Wake_Exe(), ??????? ? ?????? ????????????? ???????? ???
//??????? ?????? ?????????? ? ???????? ?????.

//??? ?????? ? ???? ????? ???? ???????? ????????? ?????????. ??? ?????
//?????? ???? ????????? ?????? USE_ADDR. ??? ???? ??????? Wake_SetAddr()
//????? ?????????? ????? ? EEPROM, ? ??? ????????????? ?????? ?? ?????
//?????? ???????????. ???? ????????? ?????????, ????? ?????? ?????
//????? ????.

//??? ????????? ?????? ????????? ????? ???????? ????? ??????????
//?????????? CRC. ??? ????? ?????? ???? ????????? ?????? TABLE_CRC.
//? ????? ?????? ? ?????? ???????? ????? ???????????? ???????, ???????
//?????? ????????????? 256 ????.

//----------------------------------------------------------------------------
#include "uart_pkg.h"
char PROGMEM DeviceName[] = { "Programming AT89Cx051" }; //??? ??????????
#define LO(x)    ((char)((x) & 0xFF))
#define HI(x)    ((char)(((x) >> 8) & 0xFF))
#define BYTE1(x) (LO(x))
#define BYTE2(x) (HI(x))
#define BYTE3(x) ((char)(((unsigned long)(x) >> 16) & 0xFF))
#define BYTE4(x) ((char)(((unsigned long)(x) >> 24) & 0xFF))
#define WORD(b1,b0)        (((unsigned int)(b1) << 8) | (b0))
#define DWORD(b3,b2,b1,b0) (((unsigned long)WORD(b3,b2) << 16) | WORD(b1,b0))

//--------------------------- ????? ??????????: ------------------------------

//#define RS485       //????????? ?????? ?????? ?? RS-485
//#define TX_DELAY 20 //???????? ?????? ?? RS-485, ??
//#define USE_ADDR    //????????? ????????????? ?????? ??????????  
#define TABLE_CRC   //????????? ?????????? ??????? ??????? CRC

//----------------------------- ?????????: -----------------------------------

#define FRAME 36      //???????????? ????? ?????? ??? ????? ????????? ??????

//???????? ???? ????????? ? ????? UART:

#define W_UCSRA         UCSRA
#define W_FE            FE
///
#define W_UCSRB         UCSRB
#define W_RXCIE         RXCIE
#define W_TXCIE         TXCIE
#define W_RXEN          RXEN
#define W_TXEN          TXEN
////
#define W_UCSRC         UCSRC
#define W_UCSZ1         UCSZ1
#define W_UCSZ0         UCSZ0
/////
#define W_UBRRL         UBRRL
#define W_UBRRH         UBRRH
#define W_UDR           UDR

enum
{
  PTR_CMD, //???????? ? ?????? ??? ???? ???????
  PTR_LNG, //???????? ? ?????? ??? ????? ??????
  PTR_DAT  //???????? ? ?????? ??? ??????
};

//???? ????-?????????:

#define FEND  0xC0    //Frame END
#define FESC  0xDB    //Frame ESCape
#define TFEND 0xDC    //Transposed Frame END
#define TFESC 0xDD    //Transposed Frame ESCape

#define CRC_INIT 0xDE //????????? ???????? ??????????? ?????
#define CRC_FEND 0x82 //????????? ???????? ??????????? ????? ? ?????? FEND

enum
{
  WST_IDLE, //????????? ????????
  //WST_ADD,  //????? ??????
  WST_CMD,  //????? ???????
  WST_LNG,  //????? ????? ??????
  WST_DATA, //?????/???????? ??????
  WST_CRC,  //?????/???????? CRC
  WST_DONE  //????????? ??????????
};

//------------------------------- ??????????: --------------------------------

//static uint8_t Addr;                 //????? ??????????
#ifdef USE_ADDR  
__no_init __eeprom int EAddr;     //????? ? EEPROM
#endif

static uint8_t RxState;              //????????? ???????? ??????
static uint8_t RxStuff;              //??????? ????????? ??? ??????
static uint8_t *RxPtr;               //????????? ?????? ??????
static uint8_t *RxEnd;               //???????? ????????? ????? ?????? ??????
static uint8_t RxCount;              //?????????? ???????? ????
static uint8_t RxData[FRAME + PTR_DAT + 1]; //????? ??????

static uint8_t TxState;              //????????? ???????? ????????
static uint8_t TxStuff;              //??????? ????????? ??? ????????
static uint8_t *TxPtr;               //????????? ?????? ????????
static uint8_t *TxEnd;               //???????? ????????? ????? ?????? ????????
static uint8_t TxCount;              //?????????? ???????????? ????
static uint8_t TxData[FRAME + PTR_DAT + 1]; //????? ????????
#ifdef RS485
static uint8_t TxTimer;              //?????? ???????? ?????? ?? RS-485
#endif

//-------------------------------- ???????: ----------------------------------

void Do_Crc8(uint8_t b, uint8_t *crc);  //?????????? ??????????? ?????

//------------------------- ????????????? UART: ------------------------------

void uart_pkg_init(void)
{ 
  //8-?????? ??????? atmega16:
  //W_UCSRC = (1 << W_UCSZ1) | (1 << W_UCSZ0);
  //??????? ???????? ??????:
  UBRR = 25; // ??? 8Mhz - 19200, Error 0.2%
  
  //?????????? ?????????? ?? ?????? ? ????????, ?????????? ?????? ? ????????:
  UCR = (1 << RXCIE) | (1 << TXCIE) | (1 << RXEN) | (1 << TXEN);

  TxState = WST_DONE;   //?????????? ????????
  RxState = WST_IDLE;   //?????????? ??????
  TxPtr = TxData + PTR_DAT;        //????????? ???????? ?? ??????
}

//----------------------- ????????? ?????? ??????????: -----------------------

/*uint8_t Wake_SetAddr(unsigned int sig, uint8_t addr)
{
#ifdef USE_ADDR         //???? ???????????? ?????????, 
  if(sig == ADDR_KEY && //???? ?????????? ?????????
     addr < 128)        //? ????? ? ?????????? ?????????
  {
    Addr = addr;        //??????? ??????
    EAddr = addr;       //?????????? ?????? ? EEPROM
    return(1);          //????????? ?????? ?????? ???????
  }
#endif    
  return(0);            //?????? ????????? ??????
}
*/
//------------------------ ?????? ?????? ??????????: -------------------------

//char Wake_GetAddr(void)
//{
//  return(Addr);
//}

//--------------------- ?????????? ??????????? ?????: ------------------------
void Do_Crc8(uint8_t b, uint8_t *crc)
{
#ifdef TABLE_CRC
  #define TABLE_READ(i) pgm_read_word(&CrcTable[i]) 
  static uint8_t CrcTable[256] PROGMEM =
  {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41, 
    0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 
    0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC, 
    0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 
    0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62, 
    0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 
    0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF, 
    0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 
    0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07, 
    0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58, 
    0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A, 
    0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6, 
    0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24, 
    0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 
    0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9, 
    0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F, 
    0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD, 
    0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 
    0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50, 
    0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 
    0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE, 
    0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1, 
    0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73, 
    0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 
    0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B, 
    0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 
    0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16, 
    0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A, 
    0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8, 
    0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 
    0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35 
  };
  *crc = TABLE_READ(*crc ^ b); //????????? ??????????
#else  
  uint16_t CRC_; 
  
  CRC_= *crc + (uint16_t)b*211;
  CRC_= CRC_ ^ (CRC_ >> 8); // "??????" ???????, ????? ??? ????????? ?????
  *crc = CRC_ & 0xFF; //??? ?????? ?????????? ????????? ??? 8, ? ?? 16 ???
  
 /* ?????? ??? ?????: ?????? ???????? 
  uint8_t i;
  
  for(i = 0; i < 8; b = b >> 1, i++) //?????????? ? ?????
    if((b ^ *crc) & 1) *crc = ((*crc ^ 0x18) >> 1) | 0x80;
     else *crc = (*crc >> 1) & ~0x80;*/
#endif     
}

//----------------------------------------------------------------------------
//----------------------------- ????? ??????: --------------------------------
//----------------------------------------------------------------------------
static uint8_t data;
//------------------- ?????????? UART ????? ?????? ?????: --------------------
ISR(UART_RX_vect)
{
  uint8_t error_flags = (USR & (1 << W_FE)); //?????? ?????? ??????     
  data = UDR;                 //?????? ??????
  if(RxState != WST_DONE){            //???? ????? ????????
    if(error_flags){                  //???? ??????,
      RxState = WST_IDLE; return;   //??????? ? ?????? FEND
    }   
    if(data == FEND){                 //????????? FEND (?? ?????? ?????????)
      RxState = WST_CMD;             //??????? ? ?????? ???? ???????
      RxPtr = RxData;                //????????? ?? ?????? ??????
      RxStuff = 0; return;           //??? ???????????
    }
    if(data == FESC){                 //?????? FESC,
      RxStuff = 1; return;          //?????? ??????????
    }   
    if(RxStuff){                      //???? ???? ?????????,
      if(data == TFESC)              //???? ?????? TFESC,
        data = FESC;                 //?????? ??? ?? FESC
      else if(data == TFEND)         //???? ?????? TFEND,
        data = FEND;                 //?????? ??? ?? FEND
        else { RxState = WST_IDLE; return; } //????? ?????? ?????????
      RxStuff = 0;                   //?????????? ????????
    }
    switch(RxState){
    /*case WST_ADD:                    //????? ??????
        RxState = WST_CMD;           //????? - ????? ???????
        if(data & 0x80){              //???? ?????? ?????,
          data &= ~0x80;             //?????????????? ???????? ??????
          if(data != Addr){           //????? ?? ??????,
            RxState = WST_IDLE; 
	    return;  //??????? ? ?????? FEND
	  }   //??????? ? ?????????? ??????
	  break; 
        } else *RxPtr++ = 0;           //?????????? ???????? ?????? ? ????? ????????? ? ?????????? ????????? ??? ??? ? data ? ??? "???????"
*/
    case WST_CMD:                    //????? ???? ???????
        RxState = WST_LNG;           //????? - ????? ????? ??????
        break;                       //??????? ? ?????????? ???????
    case WST_LNG:                    //???? ????? ????? ??????
        RxState = WST_DATA;          //????? - ????? ??????            
        if(data > FRAME) data = FRAME;   //??????????? ????? ??????
        RxEnd = RxData + PTR_DAT + data; //????????? ?? ????? ??????
        break;
    case WST_DATA:                   //???? ????? ??????
        if(RxPtr == RxEnd)           //???? ??? ?????? ? CRC ???????,
          RxState = WST_DONE;        //????? ???????
        break;
    default: return;
    }
    *RxPtr++ = data;                 //?????????? ?????? ? ??????
  }
}
  
//--------------------- ?????????? ??????? ??? ???????: ----------------------

uint8_t Wake_GetCmd(void)
{
  char data;
  char *str;
  uint8_t i;	
  uint8_t cmd = CMD_NOP;
  if(RxState == WST_DONE){            //???? ????? ?????? ????????
    RxCount = RxEnd - RxData - PTR_DAT; //?????????? ???????? ???? ??????
    uint8_t crc = CRC_FEND;             //????????????? CRC
    RxPtr = RxData;                  //????????? ?? ?????? ??????
   // if(!*RxPtr) RxPtr++;             //???? ????? ???????, ?????????? ???
    while(RxPtr <= RxEnd)            //??? ????? ??????
      Do_Crc8(*RxPtr++, &crc);       //??????? CRC 
    RxPtr = RxData + PTR_CMD;        //????????? ?? ??? ???????
    if(!crc) 
       cmd = *RxPtr;           //???? CRC ?????????, ??? ???????
    else  
       cmd = CMD_ERR;           //????? ??? ??????
  //  TxCount = 0;                     //????????? ?????????? ???? ??? ????????
    RxPtr = RxData + PTR_DAT;        //????????? ?????? ?? ??????
   // TxPtr = TxData + PTR_DAT;        //????????? ???????? ?? ??????
  }
  //????????? ??????????? ??????:
  if(cmd && (cmd < 4)){
    switch(cmd){
    case CMD_ERR:                    //????????? ??????
        Wake_AddByte(ERR_TX);        //???????? ???? ??????
        Wake_StartTx(cmd);           //?????? ????????
        break;
    case CMD_ECHO:                   //??????? "???"
        for(i = 0; i < RxCount; i++)
          *TxPtr++ = *RxPtr++;       //??????? ??????
        Wake_StartTx(cmd);           //?????? ????????
        break;
    case CMD_INFO:                   //??????? "????"
	str = DeviceName;    
        while ( (data = pgm_read_byte(str++)) ){
         Wake_AddByte(data);//??????????? ????? ? ?????
        }
        Wake_StartTx(cmd);            //?????? ????????
        break;
    }
    cmd = CMD_NOP;               //??????? ??????????
    Wake_EndCmd(); // ????????? ????????? ????????? ?????? 
  }  
  return(cmd);
}

//------------------- ?????????? ?????????? ???????? ????: -------------------

uint8_t Wake_GetRxCount(void)
{
  return(RxCount);
}

//----------------- ????????????? ????????? ?? ????? ??????: -----------------

void Wake_SetRxPtr(uint8_t p)
{
  if(p < FRAME)
    RxPtr = RxData + PTR_DAT + p;
}

//--------------------- ?????? ????????? ?????? ??????: ----------------------

uint8_t Wake_GetRxPtr(void)
{
  return(RxPtr - RxData - PTR_DAT);
}

//---------------------- ?????? ???? ?? ?????? ??????: -----------------------

uint8_t Wake_GetByte(void)
{
  return(*RxPtr++);
}

int8_t Wake_GetInt8_t(void)
{
  return(*RxPtr++);
}


//--------------------- ?????? ????? ?? ?????? ??????: -----------------------

int16_t Wake_GetInt(void)
{
  uint8_t l = *RxPtr++;
  uint8_t h = *RxPtr++;
  return(WORD(h, l));
}
uint16_t Wake_GetWord(void)     //?????? ????? ?? ?????? ??????
{
  uint8_t l = *RxPtr++;
  uint8_t h = *RxPtr++;
  return(WORD(h, l));
}


//----------------- ?????? ??????? ????? ?? ?????? ??????: -------------------

/*long Wake_GetDWord(void)
{
  char b1 = *RxPtr++;
  char b2 = *RxPtr++;
  char b3 = *RxPtr++;
  char b4 = *RxPtr++;
  return(DWORD(b4, b3, b2, b1));
}*/

//--------------------- ?????? ?????? ?? ?????? ??????: ----------------------

void Wake_GetData(uint8_t *d, uint8_t count)
{
  uint8_t i; 
  for(i = 0; i < count; i++)
    *d++ = *RxPtr++;
}

void Wake_EndCmd(void)
{
   RxState = WST_IDLE;                //?????????? ?????? ??????   
}

//----------------------------------------------------------------------------
//-------------------------- ???????? ??????: --------------------------------
//----------------------------------------------------------------------------

//---------------- ????????????? ????????? ?? ????? ????????: ----------------

void Wake_SetTxPtr(uint8_t p)
{
  if(p < FRAME)
    TxPtr = TxData + PTR_DAT + p;
}

//-------------------- ?????? ????????? ?????? ????????: ---------------------

uint8_t Wake_GetTxPtr(void)
{
  return(TxPtr - TxData - PTR_DAT);
}

//--------------------- ???????? ???? ? ????? ????????: ----------------------

void Wake_AddByte(uint8_t b)
{
  if(TxPtr < TxData + PTR_DAT + FRAME)
    *TxPtr++ = b;
}

//-------------------- ???????? ????? ? ????? ????????: ----------------------

void Wake_AddInt(int16_t w)    //???????? ????? ? ????? ????????
{
  if(TxPtr < TxData + PTR_DAT + FRAME - 1)
  {
    *TxPtr++ = LO(w);
    *TxPtr++ = HI(w);
  }
}

void Wake_AddWord(uint16_t w)    //???????? ????? ? ????? ????????
{
  if(TxPtr < TxData + PTR_DAT + FRAME - 1)
  {
    *TxPtr++ = LO(w);
    *TxPtr++ = HI(w);
  }
}


//---------------- ???????? ??????? ????? ? ????? ????????: ------------------

void Wake_AddDWord(long dw)
{
  if(TxPtr < TxData + PTR_DAT + FRAME - 3)
  {
    *TxPtr++ = BYTE1(dw);
    *TxPtr++ = BYTE2(dw);
    *TxPtr++ = BYTE3(dw);
    *TxPtr++ = BYTE4(dw);
  }
}

//-------------------- ???????? ?????? ? ????? ????????: ---------------------

void Wake_AddData(uint8_t *d, uint8_t count)
{
  uint8_t i;
  if(TxPtr <= (TxData + PTR_DAT + FRAME) - count)
    for(i = 0; i < count; i++)
      *TxPtr++ = *d++;
}

//-------------------------- ?????? ???????? ??????: -------------------------

void Wake_StartTx(uint8_t cmd)
{
  TxState = WST_DATA;                //????????? ???????? ??????
   
  TxEnd = TxPtr;                     //????????? ????? ??????
  TxCount = TxPtr - TxData - PTR_DAT; //?????????? ???? ??? ????????
  TxPtr = TxData;                    //????????? ?? ?????? ??????
//  *TxPtr++ = Addr | 0x80;            //?????????? ? ????? ??????
  *TxPtr++ = cmd;                    //?????????? ? ????? ???? ???????
  *TxPtr = TxCount;                  //?????????? ? ????? ??????? ??????
  uint8_t crc = CRC_FEND;               //????????????? CRC
  TxPtr = TxData;                    //????????? ?? ?????? ??????
 // if(!Addr) TxPtr++;                 //?????????? ??????? ?????
  while(TxPtr < TxEnd)
    Do_Crc8(*TxPtr++, &crc);         //?????? CRC ??? ????? ??????
  *TxPtr = crc;                      //?????????? ? ????? CRC
  TxPtr = TxData;                    //????????? ?? ?????? ??????
//  if(!Addr) TxPtr++;                 //?????????? ??????? ?????
  TxStuff = 0;                       //??? ?????????
  //RxState = WST_IDLE;                //?????????? ?????? ??????
#if TX_DELAY == 0
  #ifdef RS485
      Port_DIR_1;                    //???????????? RS-485 ?? ????????
  #endif
  
  W_UDR = FEND;                      //?????? ????????
#else  
  TxTimer = ms2sys(TX_DELAY);        //???? ????? RS-485, ?? ???????? ??????
  TxState = WST_IDLE;                //????????? ???????? ????????
#endif  
}

//------------------------- ???????? ???????? ??????: ------------------------

/*void Wake_Exe(bool t)
{
#if TX_DELAY != 0
  if(TxState == WST_IDLE)            //???? ???? ?????? ????????
  {
    if(TxTimer == 0)                 //? ???????? ?????? ???????
    {
  #ifdef RS485
      Port_DIR_1;                    //???????????? RS-485 ?? ????????
  #endif
      TxState = WST_DATA;            //????????? ???????? ??????
      W_UDR = FEND;                  //?????? ????????
    }
    else
      if(t) TxTimer--;               //????? ????????? ???????
  }
#endif
}*/

//------------------ ?????????? UART ????? ???????? ?????: -------------------
ISR(UART_TX_vect)
{
  if(TxState == WST_DATA){            //???? ???? ???????? ??????,
    uint8_t data = *TxPtr++;            //?? ?????? ????? ?? ??????
    if(data == FEND || data == FESC){ //??????? ???????? FEND ??? FESC,
      if(!TxStuff){                   //????? ????????
        data = FESC;                 //???????? FESC
        TxStuff = 1;                 //?????? ?????????
        TxPtr--;                     //??????? ? ???? ?? ?????
      } else {
        if(data == FEND) data = TFEND; //???????? TFEND
          else data = TFESC;         //??? TFESC
        TxStuff = 0;                 //????? ?????????
      }
    }  
    UDR = data;                    //???????? ?????
    if(TxPtr > TxEnd)                //???? ????? ?????? ?????????,
      TxState = WST_CRC;             //?????????? CRC
  } else {                              //???? ???????? ?????????,
      TxPtr = TxData + PTR_DAT;        //????????? ???????? ?? ??????
      TxState = WST_DONE;              //???????? ?????? ?????????
#ifdef RS485
    Port_DIR_0;                      //???????????? RS-485 ?? ?????
#endif     
  }
}

//------------------- ??????????? ????? ???????? ??????: ---------------------

uint8_t Wake_TxAndRx_End(void)
{
  return(TxState == WST_DONE) && (RxState == WST_DONE);
}

//----------------------------------------------------------------------------
