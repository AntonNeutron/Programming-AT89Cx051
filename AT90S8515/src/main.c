#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/sleep.h>

#include "programmer.h"
#include "scheduler.h"

int main() { 
    //JTAG interface then just disable for Atmega16 
   // MCUCSR = (1<<JTD); 
  // MCUCSR = (1<<JTD);
    //������������� ������
    Peripheral_Ports_InitFirst();
    DS_Init();
    sei();  // ��������� ���������� ����������
   while (1) {
      DispatchUART();
      DS_DispatchTask();
   }
 
 }