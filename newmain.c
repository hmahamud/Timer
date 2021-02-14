/* 
 * File:   newmain.c
 * Author: husseinmahamud
 *
 * Created on November 23, 2020, 10:16 PM
 */

#define F_CPU 8000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include "defines.h"
#include "hd44780.h"
#include "lcd.h"

static uint8_t hh = 00; 
static uint8_t mm = 00; 
static uint8_t ss = 00;



static FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar , NULL , _FDEV_SETUP_WRITE);

int main() {
    
    TCCR1A = 0;
    TCCR1B = (1<<WGM12)|(1<<CS12);
    OCR1A = (32149);
    TIMSK1 |= (1 << OCIE1A);
    sei();

    PORTB |= (1<<PORTB0);
    DDRB &= ~(1<<DDB0);
    PORTB |= (1<<PORTB2);
    DDRB &= ~(1<<DDB2);
    PORTB |= (1<<PORTB1);
    DDRB &= ~(1<<DDB1);
    lcd_init();

    while(1){
        
        if ((PINB & (1<<PINB0))==0){
            _delay_ms(100);
            if((PINB & (1<<PINB0))==0){
                cli();
                ss++;
                if (ss==60){
                    ss=0;
                    mm++;
                }
                sei();
            }
        }
        
        
        if ((PINB & (1<<PINB1))==0){
            _delay_ms(100);
            if((PINB & (1<<PINB1))==0){
                cli();
                mm++;
                if (mm==60){
                    mm=0;
                    hh++;
                }
                sei();
            }
        }
        
        
        if ((PINB & (1<<PINB2))==0){
            _delay_ms(100);
            if((PINB & (1<<PINB2))==0){
                cli();
                hh++;
                if (hh==24){
                    hh=0;
                }
                sei();
            }
        }   
    } 
}

ISR(TIMER1_COMPA_vect) {
    fprintf(&lcd_str,"\x1b\x01    %02d:%02d:%02d    \x1b\xc0 B1hh B2mm B3ss",hh,mm,ss);
    ss++;
    if(ss>=60){
        mm++;
        ss=0;
        if(mm>=60){
            hh++;
            mm=0;
            if(hh>=24){
                hh=0;
            }
        }
    }
}