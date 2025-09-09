#include "config.h"

#define MOTOR1_IN_1 RB0
#define MOTOR1_IN_2 RB1
#define MOTOR2_IN_1 RB2
#define MOTOR2_IN_2 RB3
#define MOTOR1_EN RC1

#define IR_LEFT RB4
#define IR_CENTER RB5
#define IR_RIGHT RB6

#define S0 PORTDbits.RD4
#define S1 PORTDbits.RD5
#define S2 PORTDbits.RD6
#define S3 PORTDbits.RD7
#define OUT PORTCbits.RC2
#define RED_LED RC4
#define GREEN_LED RC5
#define BLUE_LED RC6

#define IR_COLOR RB7

#define WHITE_DETECTED 0
#define BLACK_DETECTED 1


//Motor Variables
unsigned int M1_Speed = 525;
unsigned int leftRotationSpeed = 700;
unsigned int rightRotationSpeed = 700;
unsigned int kickstart = 1000;
unsigned int kickstartDelay = 50;
unsigned int backwardDelay = 1000;

//Terminal Variables
unsigned int terminalDetected = 0;
unsigned int ongoing = 1;

//Color Sensor Variables
unsigned short Count = 0;
unsigned short Red,Green,Blue;
unsigned int interpretingColor = 0;
unsigned int stopMode = 0;


void Delay(unsigned long Count)
{
    while(Count > 0)
    {
        Count--;
    }
}

void initializePWM(){
    TRISCbits.TRISC1 = 0; //ENA and ENB
    MOTOR1_EN = 0; //Turn off ENA and ENB initially
    CCP2CON = 0x0C;       // Set PWM mode on
}

void initializeInterrupts(void)
{
    TRISCbits.TRISC2 = 1; // Set OUT of TCS3200 as Input
    T1CON = 0x31;          // Timer 1 off, internal clock, 1:1 prescaler 
    TMR1 = 0;             // Clear Timer 1 
    CCP1CON = 0x05;        // Capture every rising edge
    CCP1IE = 1;           // Enable CCP1 interrupt
    TMR1ON = 1;           // Start Timer 1
    PEIE = 1;            // Enable peripheral interrupts
    GIE = 1;             // Enable global interrupts
}

void rundutycycle_motor1(unsigned int dutycyc){
    T2CON = 0x01;           // Set Prescaler to be 4, hence PWM frequency is set to 4.88KHz.
    T2CON |= 0x04;          // Enable the Timer2, hence enable the PWM.
    CCPR2L   = dutycyc>>2;        	       // Put MSB 8 bits in CCPR1L
    CCP2CON &= 0xCF;                	// Make bit4 and 5 zero
    CCP2CON |= (0x30&(dutycyc<<4));         // Assign Last 2 LSBs to CCP1CON
//if using CCP1 then CCPR1L and CCP1CON
}

void initializeMotor(){
    TRISBbits.TRISB0 = 0; //IN1
    TRISBbits.TRISB1 = 0; //IN2
    TRISBbits.TRISB2 = 0; //IN3
    TRISBbits.TRISB3 = 0; //IN4
    RB0 = 0; //Set all motors to stop
    RB1 = 0;
    RB2 = 0;
    RB3 = 0;
}

void initializeIR(){
    TRISBbits.TRISB4 = 1; //LEFT IR
    TRISBbits.TRISB5 = 1; //CENTER IR
    TRISBbits.TRISB6 = 1; //RIGHT IR
}

void kickstartMotor(){
    MOTOR1_IN_1 = 1; //Push motor from off state to on state
    MOTOR1_IN_2 = 0;
    MOTOR2_IN_1 = 1;
    MOTOR2_IN_2 = 0;
    rundutycycle_motor1(kickstart);
    __delay_ms(kickstartDelay);
}

void initializeColor(){
    TRISDbits.TRISD4 = 0; //S0
    TRISDbits.TRISD5 = 0; //S1
    TRISDbits.TRISD6 = 0; //S2
    TRISDbits.TRISD7 = 0; //S3
    TRISBbits.TRISB7 = 1; //OUT
    TRISCbits.TRISC4 = 0; //RED LED
    TRISCbits.TRISC5 = 0; //GREEN LED
    TRISCbits.TRISC6 = 0; //BLUE LED
    RD4 = 1; //Set to 20% output frequency scaling
    RD5 = 0;
    RD6 = 0; //Start as RED
    RD7 = 0;
    RC4 = 0; //Turn on Green LED
    RC5 = 1;
    RC6 = 0;
    TMR1ON = 0; // Stop Timer 1 to reset the count
    TMR1 = 0;   // Clear Timer 1 count
    TMR1ON = 1; // Restart Timer 1 for capture
}

void forward(){
    MOTOR1_IN_1 = 1;
    MOTOR1_IN_2 = 0;
    MOTOR2_IN_1 = 1;    
    MOTOR2_IN_2 = 0;
    rundutycycle_motor1(M1_Speed);
}

void backward(){
    MOTOR1_IN_1 = 0;
    MOTOR1_IN_2 = 1;
    MOTOR2_IN_1 = 0;
    MOTOR2_IN_2 = 1;
    rundutycycle_motor1(M1_Speed);
}

void left(){
    MOTOR1_IN_1 = 1;
    MOTOR1_IN_2 = 0;
    MOTOR2_IN_1 = 0;
    MOTOR2_IN_2 = 0;
    rundutycycle_motor1(leftRotationSpeed);   
}

void right(){
    MOTOR1_IN_1 = 0;
    MOTOR1_IN_2 = 0;
    MOTOR2_IN_1 = 1;
    MOTOR2_IN_2 = 0;
    rundutycycle_motor1(rightRotationSpeed);
}

void stop(){
    MOTOR1_IN_1 = 0;
    MOTOR1_IN_2 = 0;
    MOTOR2_IN_1 = 0;
    MOTOR2_IN_2 = 0;
}

void followLine(){ 
    if(IR_CENTER == BLACK_DETECTED && IR_LEFT == BLACK_DETECTED && IR_RIGHT == BLACK_DETECTED){
        terminalDetected = 1;
        forward();
        __delay_ms(50);
    }
    else if(IR_LEFT == BLACK_DETECTED){
        terminalDetected = 0;
        right();
    }
    else if(IR_RIGHT == BLACK_DETECTED){
        terminalDetected = 0;
        left();
    }
    else if(IR_CENTER == BLACK_DETECTED){
        terminalDetected = 0;
        forward();
    }
    else{
        if(terminalDetected == 1){
            ongoing = 0;
            stop();
        }
        left();
        }
    }

void interpretColor(){
    if(Red < Blue && Green < Blue){ //If yellow is chosen color, don't respond
        return;
    }
    else if(Red < Blue && Red < Green) //If red is chosen color, turn on RED LED and stop robot
    {
        RED_LED = 1;
        GREEN_LED = 0;
        BLUE_LED = 0;
        stopMode = 1;
        stop();         
        return;
    }    
    else if(Green < Red && Green < Blue) //If green is chosen color, check first if robot is stopped, if stopped then kickstart and resume
    {
        if(stopMode){
            kickstartMotor();
            RED_LED = 0;
            GREEN_LED = 1;
            BLUE_LED = 0;
            stopMode = 0;
        }            
        return;
    }    
    else if(Blue < Red && Blue < Green) //If blue is chosen color, check first is robot is stopped, if not, then reverse for 1s then resume
    {
        RED_LED = 0;
        GREEN_LED = 0;
        BLUE_LED = 1;
        backward();
        __delay_ms(backwardDelay);
        if(stopMode){
            stop();
            RED_LED = 1;
            GREEN_LED = 0;
            BLUE_LED = 0;
        }  
        else{
            kickstartMotor();
            RED_LED = 0;
            GREEN_LED = 1;
            BLUE_LED = 0;
        }
        return;
    }
}

void getColor(){
    if(interpretingColor == 1){ //This locks the function to ensure only one color is read
        return;
    }    
    S2 = 0;//Red
    S3 = 0;
    Delay(5000);
    Red = Count;

    S2 = 0;//Blue
    S3 = 1;
    Delay(5000);
    Blue = Count;

    S2 = 1;//Green
    S3 = 1;
    Delay(5000);
    Green = Count;

    GIE = 0;
    interpretColor(); 
    GIE = 1;
}  

void main(void) {
    initializeInterrupts();
    initializePWM();
    initializeMotor();
    initializeIR();
    initializeColor();
    kickstartMotor();
    
    while(ongoing){
        if(!IR_COLOR){ //If IR sensor detects an object
            __delay_ms(100);
            getColor(); //We get color
            interpretingColor = 1; //Then we lock the getColor function until IR sensor stops detecting the object
        }
        if(!stopMode){ //If robot is not stopped
            followLine(); //Follow line
        }
        if(IR_COLOR && interpretingColor == 1){ //If IR sensor stops detecting object and a color has been interpreted, unlock getColor
            interpretingColor = 0;
        }
        __delay_ms(20);
    }
    
    while(1){ //Used to permanently stop the robot
        stop();
        RED_LED = 1;
        GREEN_LED = 0;
        BLUE_LED = 0;
    }
    return;
}

void __interrupt() isroutine(void)            
{    
    
    if(CCP1IF == 1)
    {     
        Count = CCPR1;
        TMR1H = 0x00;
        TMR1L = 0x00;

        CCP1IF = 0;
    }
    
    if(TMR1IF == 1)
    { 
        TMR1H = 0x00;
        TMR1L = 0x00; 
        TMR1IF = 0;
    } 
}
