#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Servo.h>

const uint16_t WAIT_TIME = 6000;
#define SERVO_INT 500
unsigned long servo_time = millis();
#define CLOCK_INTERVAL 60000 //should be 60000

//enum state {SET_TIME , SET_ALARM, DISPLAY_TIME, ALARM, SNOOZE};

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

#define SET_TIME 0
#define SET_ALARM 1
#define DISPLAY_TIME 2
#define ALARM 3
#define SNOOZE 4

Servo myServo1;
int myServo1_max = 170;
int myServo1_min = 10;
bool myServo1_flag = 0;

long unsigned alarm_length_time=millis();

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
int state;
int hour = 0;
int minutes = 0;
int set_HM;


// Hardware SPI connection

#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4 
#define BUTTON4 5

#define DEBOUNCE_TIME 70

//globals
unsigned long time;
unsigned long disp_time;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    
unsigned long alarm_time = millis();  
unsigned long clock_time = millis();  
unsigned long alarm_clock_int = millis();
unsigned long clock_time_int = millis();

bool button1flag;
bool button2flag;
bool button3flag;
bool button4flag;

bool button1on;
bool button2on;
bool button3on;
bool button4on;

// Variables will change:
int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
bool ledflag;

const int buzzer = 9; //buzzer to arduino pin 9

bool alarm_mode_on; //can be alarm or not alarm
#define HOUR_MAX 24
int alarm_H;
#define MINUTE_MAX 60
int alarm_M;

bool alarm_on; // true if alarm is on
bool snooze_on;

void print_time(int hour, int minute, MD_Parola &x)
{

  bool boResult;
  String hour_s = String(hour);
  String minute_s = String(minute);
  
  if (minute < 10)
    minute_s = "0" + minute_s;

  String time = " " + hour_s + ":" + minute_s;

  x.print(time);
  
}

int alarm_has_been_turned_on=0;
void setup() {

  Serial.begin(9600);

  P.begin();
  state = SET_TIME;
  set_HM = 0;

  //Set up buttons
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ledflag = false;

  button1flag = false;
  button2flag = false;
  button3flag = false;
  button4flag = false;
  alarm_mode_on = false;

  print_time(alarm_H, alarm_M, P);

  alarm_H=0;
  alarm_M=0;

  myServo1.attach(6);
}

void loop() {

  if(alarm_mode_on) {
    //SET 
     if((millis()-alarm_clock_int) > 300) {
       alarm_clock_int = millis();
        //Update clock here

    }   
  } else {
    if((millis()-clock_time) > 60000) {
      clock_time=millis();
        //Update clock here

    }
  }

  switch (state) 
  {
case SET_TIME:
      if((millis()-alarm_clock_int) > 300) {
        alarm_clock_int = millis();
          //Update clock here
        
        print_time(hour, minutes, P);

      }   
      break;

    case SET_ALARM:
      //SET 
      if((millis()-alarm_clock_int) > 300) {
        alarm_clock_int = millis();
          //Update clock here
        
        print_time(alarm_H, alarm_M, P);

      }   
      break;
      
      /*
      if((millis()-alarm_clock_int) > 300) {
        alarm_clock_int = millis();
          //Update clock here
        
        print_time(alarm_H, alarm_M, P);

      }   */
      break;

    case DISPLAY_TIME:
      if((millis()-alarm_clock_int) > 300) {
        alarm_clock_int = millis();
          //Update clock here
        
        print_time(hour, minutes, P);

      }   
      break;

    case ALARM:


      break;

    case SNOOZE:
      break;

      default: break;

  }

  if(digitalRead(BUTTON1)==LOW) {
    delay(DEBOUNCE_TIME);
    if(digitalRead(BUTTON1)==LOW) {
      button1on = true;
    }
  }
  if(digitalRead(BUTTON2)==LOW) {
    delay(DEBOUNCE_TIME);
    if(digitalRead(BUTTON2)==LOW) {
      button2on = true;
    }
  }
  if(digitalRead(BUTTON3)==LOW) {
    delay(DEBOUNCE_TIME);
    if(digitalRead(BUTTON3)==LOW) {
      button3on = true;
    }
  }
  if(digitalRead(BUTTON4)==LOW) {
    delay(DEBOUNCE_TIME);
    if(digitalRead(BUTTON4)==LOW) {
      button4on = true;
    }
  }

//BUTTON 1 ISR
  if(button1on==true) {
    button1on=false;
    // ledState = !ledState;
    // digitalWrite(LED_BUILTIN, ledState);
    
    state++;
    if(state>1) state=0;
    Serial.println("clock mode ");
    Serial.println(state);

    //alarm_on = true;
    //alarm_time = millis();
  }
//BUTTON 2 ISR
  if(button2on==true) {
    button2on=false;

    if(state==SET_ALARM) {
      alarm_H++;
      if(alarm_H > 23) alarm_H = 0;
      Serial.println("alarm hours");
      Serial.println(alarm_H);
    } else if (state==SET_TIME) {
      hour++;
      if(hour>23) hour=0;
      Serial.println("clock hours");
      Serial.println(alarm_H);
    }

    // ledState = !ledState;
    // digitalWrite(LED_BUILTIN, ledState);
  }
//BUTTON 3 ISR
  if(button3on==true) {
    button3on=false;

    if(state==SET_ALARM) {
      alarm_M++;
      if(alarm_M > 59) alarm_M = 0;
      Serial.println("minutes");
      Serial.println(alarm_M); 
    } else if (state==SET_TIME) {
      minutes++;
      if(minutes>59) minutes=0;
      Serial.println("clock minutes");
      Serial.println(minutes);
    }
    // ledState = !ledState;
    // digitalWrite(LED_BUILTIN, ledState);
    alarm_on=false;
  }
//BUTTON 4 ISR
  if(button4on==true) {
    button4on=false;
    // ledState = !ledState;
    // digitalWrite(LED_BUILTIN, ledState);
    Serial.println("alarm off");
    if(alarm_on) {
       Serial.println("Alarm off for real");
       alarm_on=false;
    }
  }

    if((millis()-alarm_length_time) >= 5000) { 
      alarm_on = false; 
    }

  //Make sure time gets set when alarm goes off
  if(alarm_on==true) 
  {

    if( (millis()-servo_time) >= SERVO_INT) {
      servo_time = millis();
      myServo1_flag = !myServo1_flag;
      if(myServo1_flag==true) 
        myServo1.write(myServo1_max);
      else
        myServo1.write(myServo1_min);
    }
    
    int i=0;
    if((millis()-alarm_time)>500) {
      noTone(buzzer);
      digitalWrite(LED_BUILTIN, LOW);  
    } else {
      tone(buzzer, 1000-i*100);
      digitalWrite(LED_BUILTIN, HIGH);
      i++;
      if(i>3) i=0;
    }
    if((millis()-alarm_time)>1000) alarm_time = millis();
  } 
  else {
      noTone(buzzer);
      digitalWrite(LED_BUILTIN, LOW);      
  }

  if((millis()-clock_time_int)>CLOCK_INTERVAL) { 
    clock_time_int = millis();
    minutes++;
    if(minutes>59) {
      hour++;
      minutes=0;
    }
    if(hour>23) {
      hour=0;
    }
  }
  
  if(alarm_H == hour && alarm_M == minutes && alarm_has_been_turned_on==false) {
    alarm_on = true;
    alarm_length_time = millis();
    alarm_time = millis();
    alarm_has_been_turned_on = true;
  } 
  if(alarm_H != hour || alarm_M != minutes) 
  {
    alarm_on = false;
    alarm_has_been_turned_on = false;
  } 


}


