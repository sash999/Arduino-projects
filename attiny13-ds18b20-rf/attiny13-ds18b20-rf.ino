/* Name: main.c
ds18b20
 */
#define F_CPU 1200000UL
#define timer 2 // время в секундах (умноженное на 5) опрос и отправка данных с датчика.
#define periodusec 400 // mcs
#define DS_BIT         4 // pin 3
#define RC_BIT         3 // pin 2
#define keyT  11000

#include <avr/io.h>
#include <util/delay.h>     /* for _delay_us() */

void sendRC(unsigned long data) { // Отправка данных по радиоканалу RCswitch. Двоичный протокол

    DDRB |= _BV(RC_BIT);
    data |= 3L << 20; // ?
    unsigned short repeats = 1 << (((unsigned long)data >> 20) & 7);

    data = data & 0xfffff;

    unsigned long dataBase4 = 0; uint8_t i;

    for (i=0; i<20; i++) {
        dataBase4<<=1;
        dataBase4|=(data%2);
        data/=2;
    }

    unsigned short int j;
    for (j=0;j<repeats;j++) {

        data=dataBase4; uint8_t i;

        for (i=0; i<20; i++) {
            switch (data & 1) {

            case 0:
              
    PORTB |= _BV(RC_BIT);

    _delay_us(periodusec);

    PORTB &= ~_BV(RC_BIT);

    _delay_us(periodusec*3);
    
            break;

            case 1:
              
    PORTB |= _BV(RC_BIT);

    _delay_us(periodusec*3);

    PORTB &= ~_BV(RC_BIT);

    _delay_us(periodusec);
    
            break;

        }

        data>>=1;

    }

    PORTB |= _BV(RC_BIT);

    _delay_us(periodusec);

    PORTB &= ~_BV(RC_BIT);

    _delay_us(periodusec*31);
}
}

// OneWire функции:

void OneWireReset() 
{

     PORTB &= ~_BV(DS_BIT);
     DDRB |= _BV(DS_BIT);
     _delay_us(500);
     DDRB &= ~_BV(DS_BIT);
     _delay_us(500);
}

void OneWireOutByte(uint8_t d) 
{
   uint8_t n;

   for(n=8; n!=0; n--)
   {
      if ((d & 0x01) == 1) 
      {

     PORTB &= ~_BV(DS_BIT);
     DDRB |= _BV(DS_BIT);
         _delay_us(5);
     DDRB &= ~_BV(DS_BIT);
         _delay_us(60);
      }
      else
      {
     PORTB &= ~_BV(DS_BIT);
     DDRB |= _BV(DS_BIT);
         _delay_us(60);
     DDRB &= ~_BV(DS_BIT);
      }
      d=d>>1; 
   }
}


uint8_t OneWireInByte() 
{
    uint8_t d, n,b;

    for (n=0; n<8; n++)
    {
    PORTB &= ~_BV(DS_BIT);
    DDRB |= _BV(DS_BIT);
        _delay_us(5);
    DDRB &= ~_BV(DS_BIT);
        _delay_us(5);
    b = ((PINB & _BV(DS_BIT)) != 0);
        _delay_us(50);
        d = (d >> 1) | (b<<7);
    }
    return(d);
}

//------------------------------------------------
//-----------------Main программа-----------------
//------------------------------------------------
int __attribute__((noreturn)) main(void)
{
  uint8_t delay_counter=0;


    // установка режима для ds
    DDRB |= _BV(DS_BIT);
    PORTB &= ~_BV(DS_BIT);
    DDRB &= ~_BV(DS_BIT);


    for(;;){                /* main event loop */


    _delay_ms(5000); 

    
    if (delay_counter==0) {

      delay_counter=timer;

   // ds              
       
  uint8_t SignBit;
  uint8_t DSdata[2];
  
  
     OneWireReset();
     OneWireOutByte(0xcc);
     OneWireOutByte(0x44);
     
       PORTB |= _BV(DS_BIT);
       DDRB |= _BV(DS_BIT);
       delay(3000); // если хотим ждать когда датчик посчитает температуру.
       DDRB &= ~_BV(DS_BIT);
       PORTB &= ~_BV(DS_BIT);
     
     
     OneWireReset();
     OneWireOutByte(0xcc);
     OneWireOutByte(0xbe);

     DSdata[0] = OneWireInByte(); 
     DSdata[1] = OneWireInByte();
      

  int TReading = (int)(DSdata[1] << 8) + DSdata[0];
  
     SignBit = TReading & 0x8000; 
     if (SignBit) TReading = (TReading ^ 0xffff) + 1;
     
     sendRC(((6 * TReading) + TReading / 4)/10+500+keyT); // отправляем данные



    }
        delay_counter--;
        
    }
    
}
