/*
 * Project1.c
 *
 *  Created on: Jan 30, 2021
 *      Author: Yasmin Abdelaziz Hamza
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char flag = 1;
unsigned char resume = 1;
unsigned char sec = 0;
unsigned char min = 0;
unsigned char hr = 0;

/*INT0 code */
void INT0_Init()
{
	DDRD &=~ (1<<PD2);
	PORTD |= (1<<PD2);
	SREG &=~ (1<<7);
	SREG |= (1<<7);
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
}

ISR(INT0_vect)
{
	sec = 0;
	min = 0;
	hr = 0;
}

/*end of INT0 code */

/*INT1 code */
void INT1_Init()
{
	SREG &=~ (1<<7);
	DDRD &=~ (1<<PD3);
	MCUCR |= (1<<ISC11) | (1<<ISC10) ;
	GICR |= (1<<INT1);
	SREG |= (1<<7);
}

ISR(INT1_vect)
{
	resume = 0;
}

/*end of INT1 code */

/*INT2 code */
void INT2_Init()
{
	SREG &=~ (1<<7);
	DDRB &=~ (1<<PB2);
	PORTB |= (1<<PB2);
	GICR |= (1<<INT2);
	MCUCSR &=~ (1<<ISC2);
	SREG |= (1<<7);
}

ISR(INT2_vect)
{
	resume = 1;
}
/*end of INT2 code */

/*Timer 1 code */
void TIMER1_COMP()
{
	SREG &=~ (1<<7);
	/* Set channel A for compare mode, with prescaler 64 */
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);
	/* Initialized value of the counter*/
	TCNT1 = 0;
	/* Timer/counter1, output compare A match Interrupt enable */
	TIMSK |= (1<<OCIE1A);
	/* Compared value */
	OCR1A = 976;
	SREG |= (1<<7);
}

ISR(TIMER1_COMPA_vect)
{
	if(resume == 1)
	{
		flag = 1;
			sec++;
			if (sec > 59 )
			{
				min++;
				sec = 0;
			}
			if (min > 59)
			{
				min = 0;
				hr++;
			}
			if(hr > 24)
			{
				hr= 0;
			}
	}
}
/* end of timer1 code */

int main()
{
	DDRC |= 0x0F;
	PORTC &= 0xF0;
	DDRA |= 0x3F;
	TIMER1_COMP();
	INT0_Init(); // Reset button
	INT1_Init(); // Pause button
	INT2_Init(); // Resume Button
	while(1)

	{
		while(flag == 1 && resume == 1)
		{
			while (resume == 1)
			{
				PORTA = (1<<0);
				PORTC = (0xF0 & PORTC) | (0x0F & sec%10);
				_delay_ms(5);
				PORTA = (1<<1);
				PORTC = (0xF0 & PORTC) | (0x0F & sec/10);
				_delay_ms(5);
				PORTA = (1<<2);
				PORTC = (0xF0 & PORTC) | (0x0F & min%10);
				_delay_ms(5);
				PORTA = (1<<3);
				PORTC = (0xF0 & PORTC) | (0x0F & min/10);
				_delay_ms(5);
				PORTA = (1<<4);
				PORTC = (0xF0 & PORTC) | (0x0F & hr%10);
				_delay_ms(5);
				PORTA = (1<<5);
				PORTC = (0xF0 & PORTC) | (0x0F & hr/10);
				_delay_ms(5);
			}
			while (resume == 0)
			{
				PORTA = (1<<0);
				PORTC = (0xF0 & PORTC) | (0x0F & sec%10);
				_delay_ms(5);
				PORTA = (1<<1);
				PORTC = (0xF0 & PORTC) | (0x0F & sec/10);
				_delay_ms(5);
				PORTA = (1<<2);
				PORTC = (0xF0 & PORTC) | (0x0F & min%10);
				_delay_ms(5);
				PORTA = (1<<3);
				PORTC = (0xF0 & PORTC) | (0x0F & min/10);
				_delay_ms(5);
				PORTA = (1<<4);
				PORTC = (0xF0 & PORTC) | (0x0F & hr%10);
				_delay_ms(5);
				PORTA = (1<<5);
				PORTC = (0xF0 & PORTC) | (0x0F & hr/10);
				_delay_ms(5);
			}
			flag = 0;

		}
	}
}
