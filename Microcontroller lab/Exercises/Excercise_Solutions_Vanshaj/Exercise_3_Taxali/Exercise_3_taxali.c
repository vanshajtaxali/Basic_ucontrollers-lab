/**********************************EXERCISE 3********************************/
/* NAME: VANSHAJ TAXALI
 * MTR NR:4558621
 */

#include <msp430.h> 
#include<templateEMP.h> //including templateEMP header
  int z=0;
void initial() // function to initialize the port and pins
{
    P2SEL=0;
    P2SEL2=0;
    P2DIR |= BIT0 + BIT1 +BIT2 + BIT3 + BIT4 + BIT5 + BIT6;
    P2OUT &= ~BIT4;
    P2DIR &= ~BIT7; //bit 7 as input
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

void rshift1() // function to set right shift direction of the data in register 1
{
    P2OUT |=BIT2;
    P2OUT &= ~BIT3;
}

void rshift2()  //function to set right shift direction of the data in register 2
{
    P2OUT |=BIT0;
    P2OUT &= ~BIT1;
}
void lshift2()      //function to set left shift direction of the data in register 2
{
    P2OUT |=BIT1;
        P2OUT &= ~BIT0;

}

void play(int d) // function to run the led sequence from D1 -D4 value of d i.e 0 or 1 decides the speed
{

    P2OUT |= (BIT0 + BIT1+ BIT6);   // parallel data shift in register 2 with BIT6 as high
          clock(1);
          rshift2();
       //  lshift2();
         int j=4;

              while(j>0 && z==0)        //right shifting 1
                    {

                       clock(1);
                       if(d==0)
                       {
                       __delay_cycles(250000);
                       }
                       if(d==1)
                       {
                           __delay_cycles(125000);
                       }
                           P2OUT &= ~BIT6;
                           // P2OUT |=BIT6;
                              j--;

                    }
    }

void rewind(int d)  // function to run the led sequence from D4 -D1 value of d i.e 0 or 1 decides the speed
{
    P2OUT |= (BIT0 + BIT1+ BIT6);// parallel data shift in register 2 with BIT6 as high
    //P2OUT |= BIT6;
    clock(1);
          rshift2(); //right shift 1
          clock(1);
          P2OUT &= ~BIT6; //set SR=0
          clock(3); //right shifting 0 3 times
          lshift2(); // applying left shift to sequence 0001
       __delay_cycles(50000);
                   int j=4;
                        while(j>0 )
                              {
                                 clock(1);
                                 if(d==0)
                                 {
                                 __delay_cycles(250000);
                                 }
                                 if(d==1)
                                 {
                                     __delay_cycles(125000);
                                 }
                              //   P2OUT &= ~BIT6;
                                // P2OUT |=BIT6;
                                        j--;
                              }

}




int main(void)
{
//  WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    initMSP();
    initial(); //calling initial function that initializes ports and pins
    int count;  // initializing count variable which will help in detecting the mode of playback
    P2OUT &= ~BIT5; // clearing the register

    P2OUT |= BIT5; //setting bit 5 as high

   while(1)
   {
       P2OUT |=BIT2 + BIT3; // parallel data shift register 1
       P2OUT &=~(BIT0 + BIT1); // no shift in register 2
clock(1);
rshift1();
count=0;    // initializing count = 0 at every iteration
while((P2IN & BIT7)== 0 && count<3) // ************** Defining & Detecting the modes******************//
{           // count cannot be more than 3 because it will keep on incrementing
    clock(1);
    count++;

}
if (count==3 && (P2IN & BIT7))
    {
            z=1;
        rewind(1); // calling rewind when the sequence becommes 0001 and count=3
            z=0;
    }
else if(count==2 && (P2IN & BIT7))
{
   z=1;// cannot pause but stop the sequence

    //pause();
   //count==2;
}
else if(count==1)
{   z=0;
    play(0);        // resume normal playback when 3 is pressed and sequence is 0001 at count=1
}

     // rshift1();
   //clock(3);
else if(P2IN & BIT7)  // as soon as d4 is pressed it starts fast foward mode
       {
           play(1);
       }
       else if((P2IN & BIT7)== 0 && count==3) // any other condition leads to normal play back
       {
           play(0);
           //count=0;

       }
       }



return 0;
}
