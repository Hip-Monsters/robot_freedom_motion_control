/*
 * Created by: HipMonsters.com
 * Description: Controls RobotFreedom.com robots
 * Created: Jan 1 2022
 * Last Update: Oct 13 2024
 * License MIT
 */ 

#include "Stepper.h" 
#include <FastLED.h>
 
// Define number of steps per rotation:
const int stepsPerRevolution = 2048;   
#define LED_PIN     12
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

long debounceDelay = 10; 

int motor1Pin1l = 3;   
int motor1Pin2l = 4;  

int motor2Pin1l = 5;  
int motor2Pin2l = 6;   

int motor3Pin1l = 7;  
int motor3Pin2l = 8;   

int motor4Pin1l = 9;   
int motor4Pin2l = 10;   

//6
//7 

int motor1Pin1r = 22;   
int motor1Pin2r = 23;   

int motor2Pin1r = 24;  
int motor2Pin2r = 25;   

int motor3Pin1r = 26;  
int motor3Pin2r = 27;   

int motor4Pin1r = 28;  
int motor4Pin2r = 29;   


/// Two more

int motor5Pin1 = 30;  
int motor5Pin2 = 32;  
int motor5Pin3 = 34;  
int motor5Pin4 = 35;  

int motor6Pin1 = 31;  
int motor6Pin2 = 33;  
int motor6Pin3 = 35;  
int motor6Pin4 = 37;  

Stepper myStepper = Stepper(stepsPerRevolution,
                             motor6Pin1, 
                             motor6Pin2, 
                             motor6Pin3, 
                             motor6Pin4);

// Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

long randNumber_1;
long randNumber_2;
 
int flag      = 0;  //makes sure that the serial only prints once the state
int stateStop = 0;
int state;

// Timer Variables
int relay             = 8;
int init_motors       = 0;
long lastDebounceTime = 0;
long movementDelay    = 5000;    
int armDeplay         = 2500;

volatile byte relayState = LOW;

//
volatile byte leftArmState  = 0; 
volatile byte rightArmState = 0; 
volatile byte headState     = 0; 

volatile byte direction     = 'B';
volatile byte tmp_dir       = 'F' ;


void setup() {
  // sets the pins as outputs:
  delay( 3000 ); // power-up safety delay

  // left arms
  pinMode(motor1Pin1l, OUTPUT);
  pinMode(motor1Pin2l, OUTPUT); 
  pinMode(motor2Pin1l, OUTPUT);
  pinMode(motor2Pin2l, OUTPUT); 
  pinMode(motor3Pin1l, OUTPUT);
  pinMode(motor3Pin2l, OUTPUT); 
  pinMode(motor4Pin1l, OUTPUT);
  pinMode(motor4Pin2l, OUTPUT); 

  // right arms
  pinMode(motor1Pin1r, OUTPUT);
  pinMode(motor1Pin2r, OUTPUT); 
  pinMode(motor2Pin1r, OUTPUT);
  pinMode(motor2Pin2r, OUTPUT); 
  pinMode(motor3Pin1r, OUTPUT);
  pinMode(motor3Pin2r, OUTPUT); 
  pinMode(motor4Pin1r, OUTPUT);
  pinMode(motor4Pin2r, OUTPUT); 
 

  pinMode(motor5Pin1, OUTPUT);
  pinMode(motor5Pin2, OUTPUT);
  pinMode(motor5Pin3, OUTPUT);

  myStepper.setSpeed(5);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();
  currentPalette  = RainbowColors_p;
  currentBlending = LINEARBLEND;

  Serial.begin(9600);
}

void reset_motors(){
   // Left
   digitalWrite(motor1Pin1l, HIGH);
   digitalWrite(motor1Pin2l, LOW);  
   digitalWrite(motor2Pin1l, HIGH);
   digitalWrite(motor2Pin2l, LOW);  
   digitalWrite(motor3Pin1l, HIGH);
   digitalWrite(motor3Pin2l, LOW);  
   digitalWrite(motor4Pin1l, HIGH);
   digitalWrite(motor4Pin2l, LOW);  
   // Right
   digitalWrite(motor1Pin1r, HIGH);
   digitalWrite(motor1Pin2r, LOW);  
   digitalWrite(motor2Pin1r, HIGH);
   digitalWrite(motor2Pin2r, LOW);  
   digitalWrite(motor3Pin1r, HIGH);
   digitalWrite(motor3Pin2r, LOW);  
   digitalWrite(motor4Pin1r, HIGH);
   digitalWrite(motor4Pin2r, LOW); 

   // moters
   digitalWrite(motor5Pin1, HIGH);
   digitalWrite(motor5Pin2, LOW); 
   digitalWrite(motor5Pin3, LOW); 

   digitalWrite(motor6Pin1, HIGH);
   digitalWrite(motor6Pin2, LOW);  
   digitalWrite(motor6Pin3, LOW);  
}

void move_it(byte state) {

  // For print out
  flag = 0;
  Serial.println(state);    

  // Raise/Lower Left Arm 
  if (state == 'q') {

    digitalWrite(motor1Pin1l, HIGH);
    digitalWrite(motor1Pin2l, LOW); 

    if (flag == 0) {
      Serial.println("Lift Left Arm");
      flag = 1;
    } 
    delay(50);  
  } 
  else if (state == 'a')  {
    digitalWrite(motor1Pin1l, LOW);
    digitalWrite(motor1Pin2l, HIGH); 

    if (flag == 0) {
      Serial.println("Lower Left Arm");
      flag = 1;
    } 
    delay(50); 
  } 

  // iBicept control Logic
  else if (state == 'w') { 
    digitalWrite(motor2Pin1l, HIGH);
    digitalWrite(motor2Pin2l, LOW); 
    if (flag == 0) {
      Serial.println("Flex Left Bicept");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 's') { 
    digitalWrite(motor2Pin1l, LOW);
    digitalWrite(motor2Pin2l, HIGH); 
    if (flag == 0) {
      Serial.println("Relax Left Bicept");
      flag = 1;
    }
    delay(50);  
  }

  // For Left hand control
  else if (state == 'e') { 
    digitalWrite(motor3Pin1l, HIGH);
    digitalWrite(motor3Pin2l, LOW);
    if (flag == 0) {
      Serial.println("Open Left Hand");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 'd') { 
    digitalWrite(motor3Pin1l, LOW);
    digitalWrite(motor3Pin2l, HIGH);
    if (flag == 0) {
      Serial.println("Close Left Hand");
      flag = 1;
    }
    delay(50);  
  }
  // r and f for forth 
 
  // Raise/Lower right arm
  else if (state == 't')   { 
    digitalWrite(motor1Pin1r, HIGH);
    digitalWrite(motor1Pin2r, LOW); 
    if (flag == 0) {
      Serial.println("Lift Right Arm");
      flag = 1;
    }
    delay(50); 
  } 
  else if (state == 'g')   { 
    digitalWrite(motor1Pin1r, LOW);
    digitalWrite(motor1Pin2r, HIGH); 
    if (flag == 0) {
      Serial.println("Lower Right Arm");
      flag = 1;
    }
    delay(50); 
  }

  // Right Bicept Conttrol
  else if (state == 'y') { 
    digitalWrite(motor2Pin1r, HIGH);
    digitalWrite(motor2Pin2r, LOW); 
    if (flag == 0) {
      Serial.println("Flex Right Bicept");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 'h') { 
    digitalWrite(motor2Pin1r, LOW);
    digitalWrite(motor2Pin2r, HIGH); 
    if (flag == 0) {
      Serial.println("Relax Right Bicept");
      flag = 1;
    }
    delay(50);  
  } 

  // Right hand control
  else if (state == 'u') { 
    digitalWrite(motor3Pin1r, HIGH);
    digitalWrite(motor3Pin2r, LOW); 
    if (flag == 0) {
      Serial.println("Open Right Hand");
      flag = 1;
    }
    delay(50);  
  }
  else if (state == 'j') { 
    digitalWrite(motor3Pin1r, LOW);
    digitalWrite(motor3Pin2r, HIGH); 
    if (flag == 0) {
      Serial.println("Close Right Hand");
      flag = 1;
    }
    delay(50);  
  }

  // Right hand control
  else if (state == 'i') { 
    digitalWrite(motor4Pin1r, HIGH);
    digitalWrite(motor4Pin2r, LOW); 
    if (flag == 0) {
      Serial.println("Open Right Hand");
      flag = 1;
    }
    delay(50);  
  }
  else if (state == 'k') { 
    digitalWrite(motor4Pin1r, LOW);
    digitalWrite(motor4Pin2r, HIGH); 
    if (flag == 0) {
      Serial.println("Close Right Hand");
      flag = 1;
    }
    delay(50);  
  } 

  // i k Steps motoer https://www.makerguides.com/28byj-48-stepper-motor-arduino-tutorial/
  // Head turn control
  else if (state == 'o')    { 
    
    myStepper.step(-stepsPerRevolution);
    if (flag == 0) {
      Serial.println("Move head left");
      flag = 1;
    } 
    delay(50);   

  } 
  else if (state == 'l')   {  
    myStepper.step(stepsPerRevolution);
    if (flag == 0) {
      Serial.println("Move Head Right");
      flag = 1;
    }

    delay(50);  
    if (headState =='LEFT') {
        headState = 'CENTER';
    }
    else{
        headState = 'RIGHT';

    }
  } 

  // motor
  else if (state == 'c') { 
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor5Pin2, HIGH); 
    digitalWrite(motor5Pin3, HIGH); 
    digitalWrite(motor5Pin4, LOW); 
    if (flag == 0) {
      Serial.println("Forward");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 'v') { 
    digitalWrite(motor5Pin1, HIGH);
    digitalWrite(motor5Pin2, LOW); 
    digitalWrite(motor5Pin3, LOW); 
    digitalWrite(motor5Pin4, HIGH); 
    if (flag == 0) {
      Serial.println("Reverse");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 'b') { 
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor5Pin2, HIGH); 
    digitalWrite(motor5Pin3, LOW); 
    digitalWrite(motor5Pin4, HIGH); 
    if (flag == 0) {
      Serial.println("Left");
      flag = 1;
    }
    delay(50);  
  } 
  else if (state == 'n') { 
    digitalWrite(motor5Pin1, HIGH);
    digitalWrite(motor5Pin2, LOW); 
    digitalWrite(motor5Pin3, HIGH); 
    digitalWrite(motor5Pin4, LOW); 
    if (flag == 0) {
      Serial.println("Right");
      flag = 1;
    }
    delay(50);  
  }   

  // if the state is 'm' recset all motros to default postion
  else if (state == 'z') {
    digitalWrite(motor1Pin1l, HIGH);
    digitalWrite(motor1Pin2l, LOW); 
    digitalWrite(motor2Pin1l, HIGH);
    digitalWrite(motor2Pin2l, LOW);
    digitalWrite(motor3Pin1l, HIGH);
    digitalWrite(motor3Pin2l, LOW); 
    digitalWrite(motor4Pin1l, HIGH);
    digitalWrite(motor4Pin2l, LOW);

    digitalWrite(motor1Pin1r, HIGH);
    digitalWrite(motor1Pin2r, LOW); 
    digitalWrite(motor2Pin1r, HIGH);
    digitalWrite(motor2Pin2r, LOW);
    digitalWrite(motor3Pin1r, HIGH);
    digitalWrite(motor3Pin2r, LOW); 
    digitalWrite(motor4Pin1r, HIGH);
    digitalWrite(motor4Pin2r, LOW);


    // headr
    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor5Pin2, LOW);
    digitalWrite(motor5Pin3, LOW);
    digitalWrite(motor5Pin4, LOW);
    // wheels
    digitalWrite(motor6Pin1, LOW);
    digitalWrite(motor6Pin2, LOW);
    digitalWrite(motor6Pin3, LOW);
    digitalWrite(motor6Pin4, LOW);

    if (flag == 0) {
      Serial.println("z- rest");
      flag = 1;
    } 
    delay(50);  
  }
  // if state 'x' shut everything down
  else if (state == 'x') {
    digitalWrite(motor1Pin1l, LOW);
    digitalWrite(motor1Pin2l, LOW);
    digitalWrite(motor2Pin1l, LOW);
    digitalWrite(motor2Pin2l, LOW);
    digitalWrite(motor3Pin1l, LOW);
    digitalWrite(motor3Pin2l, LOW);
    digitalWrite(motor4Pin1l, LOW);
    digitalWrite(motor4Pin2l, LOW);

    digitalWrite(motor1Pin1r, LOW);
    digitalWrite(motor1Pin2r, LOW);
    digitalWrite(motor2Pin1r, LOW);
    digitalWrite(motor2Pin2r, LOW);
    digitalWrite(motor3Pin1r, LOW);
    digitalWrite(motor3Pin2r, LOW);
    digitalWrite(motor4Pin1r, LOW);
    digitalWrite(motor4Pin2r, LOW); 

    digitalWrite(motor5Pin1, LOW);
    digitalWrite(motor5Pin2, LOW);
    digitalWrite(motor5Pin3, LOW);
    digitalWrite(motor6Pin1, LOW);
    digitalWrite(motor6Pin2, LOW);
    digitalWrite(motor6Pin3, LOW);
    digitalWrite(motor6Pin4, LOW);
    if (flag == 0) {
      Serial.println("x- stop");
      flag = 1;
    } 
    delay(50);  
  }
  //For debugging purpose
  //Serial.println(state);
} 

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};





void loop() {
  //if some date is sent, reads it and saves in state

  if (init_motors ==0){
    init_motors =1;
    reset_motors();
  }

  if (Serial.available() > 0) {
    state = Serial.read(); 
    flag = 0;
    Serial.println(state);  
    move_it(state);
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    lastDebounceTime = millis();
    ChangePalettePeriodically(); 
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */ 
    FillLEDsFromPaletteColors( startIndex); 
    FastLED.show();
   } 
 
}
 


 