
/*********EXERCISE 2****************/
/*VANSHAJ TAXALI
 * Mtr Nr. 4558621
 */

#include <msp430.h> 
#include<templateEMP.h>
 int x;
 int y;
void Red_led(void) //creating a function to glow red led the way it is asked
{

   while(((BIT3 & P1IN)== 0) && (P1IN & BIT4) !=0) // active low switch
        {

            if(x==0)
            {


                P1OUT &= ~BIT7; //turning yellow led conncted to P1.7 off
            P1OUT ^=BIT5;  //turning on red led p1.5 on
            __delay_cycles(250000); //delay for blinking
            P1OUT &= ~BIT5;
            P1OUT ^=BIT7; // turning yellow led on again

                x=x+1; // raising the integer to exit the loop it is re initialised to 0 in main while loop
            }
            else
                {
                    P1OUT &= ~BIT5; //turning bit 5 off


                }

        }
}
// comment to see the output of ques 1a) and ques 1g
# pragma vector = PORT1_VECTOR
__interrupt void Port_1 ( void ) // interrupt function
{
     if((P1IN & BIT4)!=0) /*condition that green led should be turned off by polling pb6 so that interrupt
     also works normally as the red led function and also in sync with other given task such as glowing of blue and green led */

               {
                    P1OUT &= ~BIT7;
               P1OUT ^=BIT5;
               __delay_cycles(250000);
              P1OUT &= ~BIT5;
               P1OUT ^=BIT7;
                }
    P1IFG &= ~ BIT3 ; // clearing the interrupt flag
    }
/*****************************************************************/

    int main(void)
{

    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    P1REN |= BIT3; // enebling pull resistors
    P1IE |= BIT3 ; //enabling interrupt at press of switch 3 i.e pb5
    P1IES |=BIT3 ; // enabling interrupt at high to low transition
    P1DIR |= BIT7; //p1.7 as output
    P1DIR |= BIT5; //p1.5 as output
    P1DIR |= BIT0; //p1.0 as output
    P1DIR |= BIT6; //p1.6 as output
    P1REN |= BIT4; //enebling pull resistors

_BIS_SR(GIE);

    while (1) // Infinite Loop
    {
        x=0; // re initializing x to 0;

       P1OUT ^=BIT7; //turning on bit 7 permanently except on the glow of red led
      //Red_led();  uncomment and other interrupt related queris to see the wrking function of red led glowing

    if((P1IN & BIT4)== 0 && (BIT3 & P1IN) != 0) // if pb5 is off and pb6 is on
    {
            P1OUT ^=BIT6;  // p1.6 or green led onn
            // __delay_cycles(250000);

        }

    else if((P1IN & BIT4)== 0 && (BIT3 & P1IN)== 0) // if pb5 and pb6 both are on here it can work as an interrupt as well
        {
        //unclear if we turn the other leds on or off so we turned off
             P1OUT ^=BIT0; //blue led on
             P1OUT &= ~BIT5; // red led off
             P1OUT &= ~BIT6; //green led off

        }


    else
          {
        // turning off all the other led's
                P1OUT &= ~BIT5;
                P1OUT &= ~BIT6;
                P1OUT &= ~BIT0;
          }


}

}

