#include <msp430.h> 
#include<templateEMP.h>
int i,j,x;//global integer variables
int count=0;
int tune_1 [9] = {100 ,200,1500,600,120,205,400,150};//array for melody 1
int tune_2 [5] = {2000 ,1000,500,300,145};//array for melody 2
void initialization()   //initalization of pins
{

    P3DIR |= BIT6 ; // P3 .6 output
       P3SEL |= BIT6 ; // P3 .6 TA0 .2 option
     //  P3REN |=BIT6;
       //P3OUT |=BIT6;
       /*****For Interrupt***********/
       P1DIR &= ~ BIT3 ; // Set as input
        P1REN |= BIT3 ; // Enable pull - resistors
        P1OUT |= BIT3 ; // Set to pull -up
        P1IE |= BIT3 ; // Enable interrupt
        P1IES |= BIT3 ; // High /Low - Edge
        P1IFG &= ~ BIT3 ; // Clear interrupt flag

        P1DIR &= ~ BIT4 ;
      P1REN |= BIT4 ;
               P1OUT |= BIT4 ;

        _BIS_SR(GIE); //Global enable interrupt

}
void tuning(int tune_1, int tune_2) //generating pwm
{
    if(tune_1)
    {   TA0CCR0 = tune_1;
        TA0CCR2 = tune_1/2; // CCR2 PWM duty cycle (50 %)
    }
    else if(tune_2)
    {   TA0CCR0 = tune_2;
        TA0CCR2 = tune_2/2;

    }
}
void playdefault()// default music playback
{
    for(i=0;i<8;i++)//traversing through integers in array
        {
            tuning(tune_1[i],0);
           __delay_cycles(500000);

        }
    __delay_cycles(1000000);

    for(j=0;j<4;j++)
           {
               tuning(0,tune_2[j]);
               __delay_cycles(500000);
           }
    __delay_cycles(1000000);
}
# pragma vector = PORT1_VECTOR  //interrupt program
__interrupt void Port_1 ( void )
{

   __delay_cycles(250000);

   count++;//incrementing count


P1IFG &= ~ BIT3 ; //clear interrupt

}
void piezo()
{
    P3DIR &= ~BIT6;  // Set the buzzer in input mode.
       P3REN |=  BIT6;
       P3OUT |=  BIT6;
       if ((P3IN & BIT6) != 0)
       {  // If knock is detected...
          count++;
       }
     //  P3DIR |= BIT6;  // Reset the buzzer in output mode.
}


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
initMSP();
    initialization();
    TA0CCTL2 = OUTMOD_3 ; // CCR2 set / reset
    TA0CTL = TASSEL_2 + MC_1 ; // SMCLK ; MC_1 -> up mode ;



while(1)
    {
         //playdeafult(); uncomment to play default melodies one after the other

     /**********selecting melodies using count variable in interrupt***************/


   P3DIR |= BIT6;
    if (count>0)
    {
      x++;
      if(x>1000)
      {
          P1IE &= ~BIT3;

     if(count==1)
             {
         TA0CTL = TASSEL_2 + MC_1 ;

         for(i=0;i<4;i++)
                {
                    tuning(tune_1[i],0);
                   __delay_cycles(500000);

                }
         TA0CTL = TASSEL_2| MC_0;


             }

     else
        {TA0CTL = TASSEL_2 + MC_1 ;

         for(j=0;j<4;j++)
                   {
                       tuning(0,tune_2[j]);
                       __delay_cycles(500000);
                       }
         TA0CTL = TASSEL_2| MC_0;
        }
         count=0;
         x=0;
         P1IE |= BIT3 ;

          }
      __delay_cycles(1000);
    }

    }


    return 0;
}



