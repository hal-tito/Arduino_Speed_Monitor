#include <TimerInterrupt.h>
#include <TimerInterrupt.hpp>
#include <ISR_Timer.h>
#include <ISR_Timer.hpp>

const int m1_p1 = 3;          //motor 1-polarity 1
const int m1_p2 = 5;          //motor 1-polarity 2
const int m2_p1 = 6;          //motor 2-polarity 1
const int m2_p2 = 11;         //motor 2-polarity 2
float count = 0.0;            //variable for number of ticks
float rpm = 0.0;              
float distance = 0.0;
float w_speed = 0.0;

int PWM = 25;
void setup() {
  pinMode(m1_p1, OUTPUT);
  pinMode(m1_p2, OUTPUT);
  pinMode(m2_p1, OUTPUT);
  pinMode(m2_p2, OUTPUT);

  //set pins as outputs
//  pinMode(8, OUTPUT);
//  pinMode(9, OUTPUT);
//  pinMode(13, OUTPUT);

//  pinMode(13, OUTPUT);

  attachInterrupt(0, tick, RISING);       //setup external interrupt

  Serial.begin(9600);                     //setup serial comms


  cli();//stop interrupts

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
//  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  OCR1A = 1561;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
}


ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

//        x pulses    1000ms       60s       1rev        
//rpm = --------- x -------- x -------- x ---------- ; pulses = count
//        100ms         1s         1min      240pulses 


//          x pulses    1000ms        1rev      2*pi*r mm
//speed = --------- x --------  x ---------- x ---------
//          100ms          1s      240pulses     1rev


  rpm = count * 2.50;       //revolutions/min
  w_speed = count * 7.854;  //speed in mm/s
  distance = distance + w_speed * 0.1;   //distance in mm
  count = 0.0;


  Serial.print(rpm);
  Serial.print('\r');
  Serial.print(w_speed);
  Serial.print('\r');
  Serial.print(distance);
  Serial.print('\r');
  Serial.print('\r');
  
  
  
}

void loop() {
  forward(150);
}


void tick(){
  count = count + 1.0;
}

void forward(int PWM){
  analogWrite(m1_p1, PWM);
  analogWrite(m1_p2, 0);
  analogWrite(m2_p1, PWM);
  analogWrite(m2_p2, 0);
}

void reverse(int PWM){
  analogWrite(m1_p1, 0);
  analogWrite(m1_p2, PWM);
  analogWrite(m2_p1, 0);
  analogWrite(m2_p2, PWM);
}

void left(int PWM){
  analogWrite(m1_p1, 0);
  analogWrite(m1_p2, PWM);
  analogWrite(m2_p1, PWM);
  analogWrite(m2_p2, 0);
}

void right(int PWM){
  analogWrite(m1_p1, PWM);
  analogWrite(m1_p2, 0);
  analogWrite(m2_p1, 0);
  analogWrite(m2_p2, PWM);
}

void halt(){
  analogWrite(m1_p1, 0);
  analogWrite(m1_p2, 0);
  analogWrite(m2_p1, 0);
  analogWrite(m2_p2, 0);
}
