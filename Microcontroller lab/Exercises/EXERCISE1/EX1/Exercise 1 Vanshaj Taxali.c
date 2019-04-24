#include <msp430.h> 
#include<templateEMP.h> //including templateEMP header

int main(void)
{
    initMSP();

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |=0x0010;     //Declaring Pin 1.4 as an output connected to K3 which is connected to RED D6
    P1DIR |=0x0020;     /*Declaring Pin 1.5 as an output connected to X10 which is further connected to BLUE LED D7.
    This pin is also connected to "HEATER".To activate it just remove the jumper at JP4 and connect.*/

    while(1)// infinite loop to continusly run the program
    {
         //function to check if serial data is availaible or not if yess it returns a value 1 else 0;

    while(!(serialAvailable())) /* calling function serialAvailable() defined in templateEMP.h returns a char value 1
    if there is a serial data availaible else it returns 0 hence used also as a conditional statement in whilee loop*/
    {

    P1OUT ^=0X0010; //Turning Pin 1.4 on
    P1OUT ^=0X0020; // Turning Pin 1.5 on
    __delay_cycles(250000); // Delay cycles at 50% duty cycle for 2hz for Red LED and 1Hz for Blue Led.
    P1OUT ^=0X0010;
    __delay_cycles(250000);
    }
// while loop breaks if serial data is entered
    if((P1IN & BIT5)== 0)// conditional statement to check if Port1 input and pin 5 input are 0 then print off
    {
        serialPrintln(" OFF "); //using serialPrint() function defined in templateEMP.h  to print data on hterm serial monitor
    }
    else
    {
        serialPrintln(" ON ");
    }
    serialFlush();//  again pre-defined function that resets the buffer to 0;

__delay_cycles(5000000); // delay to just hold the status of led for checker to verify the status at serial monitor as well
//after delay code resumes to blinking i.e the second while loop untill the user enters a serial data.


    }
    return 0;
}

