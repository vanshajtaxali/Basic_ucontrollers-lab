// Copyright Nico Buehler
// Author: Nico Buehler < buehleni@gmail.com >

// Include the given template
#include <templateEMP.h>

#define PB5 BIT3
#define PB6 BIT4
#define BUZZER BIT6

// ++++++++++++++
// ++ MELODIES ++
// ++++++++++++++
int melody[2][5] = {
		{100, 600, 300, 400, 500},
		{900, 0, 1000, 100, 400},
};

// ++++++++++++++++++++++
// ++ GLOBAL VARIABLES ++
// ++++++++++++++++++++++

int timesPressed = 0;
int timePassed = 0;

// +++++++++++++++++++++++++
// ++ FUNCTION DEFINITION ++
//++++++++++++++++++++++++++
// Sets I/O-Directions.
void initDirection();

// Set P3.5 to PWM mode.
void initPWM();

// Initialize the interrupt.
void initInterrupt();

// Plays a given tone a certain time.
void play(int tone);

// Stops the tone.
void stop();

// Exercise a)
void exerciseA();

// Plays Melody 1
void playMelodyOne();

// Plays Melody 2
void playMelodyTwo();

void detectKnock();

// +++++++++++++++++++
// ++ MAIN FUNCTION ++
// +++++++++++++++++++
int main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	initMSP();
	initDirection();
	initPWM();
	initInterrupt();

	// exerciseA();

	__enable_interrupt(); // enable all interrupts
	while (1) {
		if (timesPressed > 0) {
			timePassed++;
			if (timePassed > 1000) {
				__disable_interrupt();
				if (timesPressed == 1) {
					playMelodyOne();
				}
				else {
					playMelodyTwo();
				}
				timePassed = 0;
				timesPressed = 0;
				__enable_interrupt();
			}
			__delay_cycles(1000);
		}

		// detectKnock();
	}
}

// ++++++++++++++++++++++++++
// ++ FUNCTION DECLARATION ++
// ++++++++++++++++++++++++++
void initDirection() {
	P1SEL  &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);
	P1SEL2 &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);

	P3SEL  &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);
	P3SEL2 &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);

	P1DIR &= ~(PB5 | PB6);					// Set PB5 and PB6 in input mode.
	P3DIR |=  (BUZZER);						// Set BUZZER in output mode.
}

void initPWM() {
	P3SEL |=  (BUZZER);						// P3.6 TA0.2 option.
	TA0CCTL2 = OUTMOD_3 ;					// CCR2 set/reset
	TA0CCR0 = 1000;							// PWM Period : 1000 us
	stop();
	TA0CTL = TASSEL_2 + MC_1;				// SMCLK ; MC_1 -> up mode;
}

void initInterrupt() {
	P1IE   |=  (PB5);						// Interrupt enabled
	P1IES  |=  (PB5);						// Interrupt edge is set to high-to-low transition
	P1REN  |=  (PB5);						// Resistor enable
	P1OUT  |=  (PB5);
	P1IFG  &= ~(PB5);						// Clears interrupt flag
}

void play(int tone) {
	TA0CCR2 = tone;
	__delay_cycles(200000);
}

void stop() {
	TA0CCR2 = 1000;
}

void exerciseA() {
	int i, j;
	for (i = 1; i >= 0; i--) {
		for (j = 4; j >= 0; j--) {
			play(melody[1 - i][4 - j]);
			stop();
		}
		__delay_cycles(200000);
	}
}

void playMelodyOne() {
	int i;
	for (i = 4; i >= 0; i--) {
		play(melody[0][4 - i]);
		stop();
	}
}

void playMelodyTwo() {
	int i;
	for (i = 4; i >= 0; i--) {
		play(melody[1][4 - i]);
		stop();
	}
}

void detectKnock() {
	P3DIR &= ~BUZZER;  // Set the buzzer in input mode.
	P3REN |=  BUZZER;
	P3OUT |=  BUZZER;
	if ((P3IN & BUZZER) != 0) {  // If knock is detected...
		timesPressed++;
	}
	P3DIR |= BUZZER;  // Reset the buzzer in output mode.
}

// +++++++++++++++++++++++++++++++
// ++ INTERRUPT SERVICE ROUTINE ++
// +++++++++++++++++++++++++++++++

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
	timesPressed++;
	P1IFG &= ~PB5;
}

