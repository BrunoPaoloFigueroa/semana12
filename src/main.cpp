#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char V_A0,V_A1;
void gate(char gate){//cambio de puerto A0 a A1

  ADMUX&=0xF0;
  ADMUX|=gate;

}
void adc_config(){

    ADMUX|=(1<<REFS0);
    ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2);
    gate(0);
    ADCSRA|=(1<<ADSC);
}
ISR(ADC_vect){

      
      V_A0=ADC*(100.0/1023.0);
  
     
   

    ADCSRA|=(1<<ADSC);
}
void config_timer(void){

    TCCR0A|=(1<<WGM01);
    TCCR0B|=(1<<CS01);
   // OCR0A=250;
}


void delay_10us(void){
    OCR0A=20;//93.75
    TCNT0=0;
    while(!(TIFR0&(1<<OCF0A)));
    TIFR0|=(1<<OCF0A);

}
void config_pwm(void){
DDRB|=0x01;
PORTB&=~0x01;
config_timer();


}
void pwm(unsigned char a){
PORTB|=0x01;
for(int i=0;i<100+a;i++){
  delay_10us();
}
PORTB&=~0x01;
for(int i=0;i<1900-a;i++){
  delay_10us();
}
}



int main(void){
    config_pwm();
  adc_config();
sei();
    while(1){

pwm(V_A0);

    }
  return 0;
  }