/******************************EXERCISE 7***************************/
/*VANSHAJ TAXALI
 * Matriculation Nr. 4558621
 *
 */

#include<msp430.h>
#include<templateEMP.h>

// _____________________________________________________________________________________
// ++++++++++++++++++++++
// ++ GLOBAL VARIABLES ++
// ++++++++++++++++++++++


int count;
int temperature;
static int buttonPressed;


// _____________________________________________________________________________________
// +++++++++++++++++++++++++++++++++++++++
// ++ FUNCTION DECLARATION & DEFINATION ++
// +++++++++++++++++++++++++++++++++++++++

//Initialization function
void initialize()
{   // Set P1.0 as output
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    // Set P1.3 as input
    P1DIR &= ~BIT3;
    // Enable P1.3 pull-up resistor
    P1REN |= BIT3;
    // Set to pull-up
    P1OUT |= BIT3;
    // Enable Interrupts
    P1IE |= BIT3;
    // High/Low-Edge
    P1IES |= BIT3;
    // Clear Interrupt Flags
    P1IFG &= ~BIT3;
    // Clock
    //BCSCTL1 |= DIVA_3;
    //BCSCTL3 |= LFXT1S_2;
    // Set P3.5 as output
    P3DIR |= BIT5; // REL_DYN X4
    // Set P3.4 as output
    P3DIR |= BIT4; // REL_STAT X5
    // Set P3.2 as output
    P3DIR |= BIT2; // LED rt
    P3OUT &= ~BIT2;
    // Set P1.5 as input
    P1DIR &= ~BIT5; // U_NTC Thermistor

    P3OUT &= ~BIT4;
}


void _watchdog_enable(void)
{
    WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;// + WDTIS0;
}
void clock(int x) //function to generate clock with x as an argument and it decides number of clocks
{
    while(x>0)
    {
        P2OUT |=BIT4;
        P2OUT &= ~BIT4;
        x--;
    }
}
void initADC()
{
    // Turn ADC on; use 16 clocks as sample & hold time ( see p. 559 in the user guide )
    ADC10CTL0 = ADC10ON + ADC10SHT_2 ;
    // Enable P1 .5 as AD input
    ADC10AE0 |= BIT5;
    // Select channel 5 as input for the following conversion (s)
    ADC10CTL1 = INCH_5;
}
void ADCread()
{
    // Start conversion
       ADC10CTL0 |= ENC + ADC10SC;
       // Wait until result is ready
       while (ADC10CTL1 & ADC10BUSY);
       // If result is ready , copy it to temperature
       temperature = ADC10MEM;
}
void ADCprint()
{
    serialFlush();
                 serialPrintInt (temperature);
                 serialPrintln ("");

}
void task1()
{
    buttonPressed = 0;  // Variable for checking wheather interrupt was caused (PB5 pressed).



      // While button is not pressed, toggle LED

        P1OUT ^= BIT0;  // Toggle green LED.
        __delay_cycles(125000);  // delay 125 ms.

      P1OUT &= ~BIT0;  // Switch off green LED.
      // Do nothing.

    }
void resetShift()
{
    P2SEL &= ~BIT6;
    P2SEL2 &= ~BIT6;
    P2SEL &= ~BIT7;
    P2SEL2 &= ~BIT7;

    P2DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6;
    P2DIR &= ~BIT7;
    // Clear / reset the shift register , i.e. turn all LEDs off.
    P2OUT &= ~BIT5;
    // Set the shift register 2 mode right shift mode (S0 =1, S1 =0) ,
    // stop the reset mode (/ CLR = 1) and
    // apply the data to be shifted at SR (SR =1 to insert a 1).
    P2OUT |= BIT0 + BIT5 + BIT6 ;
    P2OUT &= ~BIT4;
    P2OUT &= ~BIT1;
}
void Range(int m)
{
    if(m==1)
    {
        P3OUT |= BIT4;

                           P3OUT &= ~BIT2;
                               resetShift();
                               __delay_cycles(100000);
     }
    else if(m==2)
    {
        P3OUT &= ~BIT2;
                         resetShift();
                         count = 0;
                         while(count != 1)
                         {
                             P2OUT |= BIT0;
                             P2OUT &= ~BIT1;
                             clock(1);
                             count++;
                             // set SR to 0
                             P2OUT &= ~BIT6;
                         }
                         __delay_cycles(100000);
    }
    else if(m==3)
    {
        P3OUT &= ~BIT2;
                              resetShift();
                              count = 0;
                              while(count != 2)
                              {
                                  P2OUT |= BIT0;
                                  P2OUT &= ~BIT1;
                                  clock(1);
                                  count++;
                              }
                              // set SR to 0
                              P2OUT &= ~BIT6;
                              __delay_cycles(100000);
    }
    else if(m==4)
    {
        P3OUT &= ~BIT2;
    resetShift();
    count = 0;
    while(count != 3)
    {
        P2OUT |= BIT0;
        P2OUT &= ~BIT1;
        clock(1);
        count++;
    }
    // set SR to 0
    P2OUT &= ~BIT6;
    __delay_cycles(100000);

    }
    else if(m==5)
    {
       // __delay_cycles(100000);

                            P3OUT &= ~BIT2;
                              resetShift();
                              count = 0;
                              while(count != 4)
                              {
                                  P2OUT |= BIT0;
                                  P2OUT &= ~BIT1;
                                  clock(1);
                                  count ++;
                              }
                              // set SR to 0
                              P2OUT &= ~BIT6;
                              __delay_cycles(100000);


    }
    else if (m==6)
    {
        //P3OUT &= ~BIT4; //turn off heater
        P3OUT |= BIT4;
                          P3OUT |= BIT2;
                             resetShift();
                             count = 0;
                             while(count != 4)
                             {
                                 P2OUT |= BIT0;
                                 P2OUT &= ~BIT1;
                                 clock(1);
                                 count ++;
                             }
                             // set SR to 0
                             P2OUT &= ~BIT6;
                             __delay_cycles(100000);


    }
    else
        exit();

}
void Refresh()
{ TA0CTL = TASSEL_1 + MC_1 + TAIE;
TA0CCR0 = 500;

//CCR0 interrupt enabled
TA0CCTL0 = CCIE;
}

// _____________________________________________________________________________________
// ++++++++++++++++
// ++ INTERRUPTS ++
// ++++++++++++++++


// Port 1 interrupt service routine
#pragma vector = PORT1_VECTOR;
__interrupt void Port_1(void) {
    __delay_cycles(250000);
    buttonPressed = 1;

    // Watchdog - Password, clk source: ACLK, count clear.
          WDTCTL = (WDTPW | WDTSSEL | WDTCNTCL);

          // Set the divider for the ACLK to 1/2.

         // For 4 sec for task 1
          //BCSCTL1 |=  (DIVA1);


          BCSCTL1 |= DIVA_3;  // for 20 sec
         // LF clk set to VLOCLK.

          BCSCTL3 |= LFXT1S_2;

    P1IFG &= ~BIT3;  // Enable interrupt.
}
#pragma vector = TIMER0_A0_VECTOR //CCR0 interrupt vector
__interrupt void Timer_A (void)
{
    resetShift();

  TACTL |=TAIFG;
}


// _____________________________________________________________________________________
// +++++++++++++++++++
// ++ MAIN FUNCTION ++
// +++++++++++++++++++

int main(void)
{
	//WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	initMSP();
	    initialize();
	    initADC();

	    P3OUT &= ~BIT4; //turn off heater
//	Refresh(); // Refresh LED D1-D4


	    while(buttonPressed==0)
	    {

	        ADCread();
	      //  ADCprint();
	      // __delay_cycles(125000);
	        buttonPressed=0;
	     if (temperature <= 450)
	            {
	               Range(1);
	            }
	            else if (temperature >450 && temperature <= 520)
	            {
	                P3OUT |= BIT4;

	                Range(2);
	            }
	            else if (temperature > 520 && temperature <= 570)
	            {
	               Range(3);
	                // control the heater uncomment for task 2e)
	              if (temperature > 570)
	                  P3OUT |= BIT4;
	            }
	            else if (temperature > 570 && temperature <= 620)
	            {
	                Range(4);
	              // control the heater uncomment for task 2e)
	              if (temperature > 620)
	                  P3OUT &= ~BIT4; //turn off heater

	            }
	            else if (temperature >620 && temperature <= 660)
	            {
	             Range(5);
	             P3OUT &= ~BIT4; //turn off heater uncomment for task 2e)
	            }

	            else if (temperature > 660)
	            {
	             Range(6);
	           // uncomment for task 2e
	              __delay_cycles(3000000);
	              P3OUT &= ~BIT4;
	            }
	            else
	                count = 0;

	      task1();
	      while (buttonPressed==1)
	      {
	          P3OUT |= BIT4; //uncomment for task 2e) continues heating up the resistor
	          //so after the watch dog the ntc goes into over heating
	      }


	      __enable_interrupt(); // enable all interrupts
	          _BIS_SR(GIE); // Sleep in LPM0 with interrupts enabled



	    }

	
	return 0;
}
