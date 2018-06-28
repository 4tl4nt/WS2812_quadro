#include <Adafruit_NeoPixel.h>

#define PIN_WS2812 0
#define LED_COUNT 52
#define DELAY_BLINK 30
#define DELAY_RUNNING 10
#define LONG_DELAY 130
#define LED_LEFT 0
#define LED_RIGHT (LED_COUNT/2)
#define LED_NUM_LEFT (LED_RIGHT-1)
#define LED_OFF 0
#define BRIGHT 255
#define MAX_LIGHT 255
//#define DEBUG
#define DIGISPARK

#define REAR_LEFT_START 0 
#define REAR_LEFT_END 7
#define REAR_MIDLE_START 8
#define REAR_MIDLE_END 35
#define REAR_RIGHT_START 36
#define REAR_RIGHT_END 43
#define FRONT_LEFT_START 44
#define FRONT_LEFT_END 51
#define FRONT_RIGHT_START 52
#define FRONT_RIGHT_END 59


int 
  left_red = 1*MAX_LIGHT,
  left_blue = 0*MAX_LIGHT,
  right_red = 0*MAX_LIGHT,
  right_blue = 1*MAX_LIGHT;
  
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT+1, PIN_WS2812, NEO_GRB + NEO_KHZ800);

int dright = 255;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(BRIGHT);
  #ifndef DIGISPARK
  Serial.begin(9600);
  #endif
}

void ShiftLed(int Start, int End, uint8_t direct = 1){
  uint8_t *p = strip.getPixels();
  uint16_t i;
  uint8_t last[3] = {0,0,0};
  if (direct) {
    last[0] = *(p+Start*3);
    last[1] = *(p+1+Start*3);
    last[2] = *(p+2+Start*3);
    for (i=Start; i<End; i++){
      *(p+i*3) = *(p+(i+1)*3);
      *(p+i*3+1) = *(p+(i+1)*3+1);
      *(p+i*3+2) = *(p+(i+1)*3+2);
    }
    *(p+i*3) = last[0];
    *(p+i*3+1) = last[1];
    *(p+i*3+2) = last[2];
  }
  else{
    last[0] = *(p+(End)*3);
    last[1] = *(p+(End)*3+1);
    last[2] = *(p+(End)*3+2);
    for (i=End; i>Start; i--){
      *(p+i*3) = *(p+(i-1)*3);
      *(p+i*3+1) = *(p+(i-1)*3+1);
      *(p+i*3+2) = *(p+(i-1)*3+2);
    }
    *(p+i*3) = last[0];
    *(p+1+i*3) = last[1];
    *(p+2+i*3) = last[2];
  }
  strip.show();
}

#ifdef DEBUG
void Debug(const char * string = 0){
  #ifndef DIGISPARK
  if (string){
    Serial.println(string);
  }
  else Serial.print("debug");
  //#else
  strip.setPixelColor(LED_COUNT,10,10,10);
  delay(100);
  strip.setPixelColor(LED_COUNT,0,0,0);
  delay(100);
  strip.show();
  #endif
}
#else
#define Debug
#endif

void Running_lighter(int Start, int End){
  int Count=0;
  for (int i=Start;i<=End; i++) strip.setPixelColor(i,0,0,0);
  //strip.setPixelColor((Count++)+Start,MAX_LIGHT-200,MAX_LIGHT-200,MAX_LIGHT-200);
  //strip.setPixelColor((Count++)+Start,MAX_LIGHT,MAX_LIGHT,MAX_LIGHT);
  strip.setPixelColor((Count++)+Start,LED_OFF,LED_OFF,MAX_LIGHT);
  strip.setPixelColor((Count++)+Start,MAX_LIGHT,MAX_LIGHT,MAX_LIGHT);
  strip.setPixelColor((Count++)+Start,LED_OFF,LED_OFF,MAX_LIGHT);
  //strip.setPixelColor((Count++)+Start,MAX_LIGHT-200,MAX_LIGHT-200,MAX_LIGHT-200);
  strip.show();
  delay(10);
  while(1) {
    for (int i=Start;i<=(End-Count); i++){
      ShiftLed(Start, End, 0);
      delay(DELAY_RUNNING);
    }
    for (int i=Start;i<=(End-Count); i++){
      ShiftLed(Start, End, 1);
      delay(DELAY_RUNNING);
    }
  }
}
void police(){
  int i=0;
  for (;i<LED_COUNT/2;i++){
    strip.setPixelColor(i,255,0,0);
  }
  for (;i<LED_COUNT;i++){
    strip.setPixelColor(i,0,0,255);
  }
  while(1) {
    //ShiftLed(1,Start, End);
    delay(5);
  }
}
void setColor(unsigned char Red, unsigned char Green, unsigned char Blue, int Pos, int Num){
  int last = Pos+Num;
  for (int cur = Pos; cur < last; cur++)
    strip.setPixelColor(cur, Red,Green,Blue);
  strip.show(); 
}
void police2() {
  while (1){
    if (left_red) left_red = 0*MAX_LIGHT;
    else left_red = 1*MAX_LIGHT;
    if (left_blue) left_blue = 0*MAX_LIGHT;
    else left_blue = 1*MAX_LIGHT;
    if (right_red) right_red = 0*MAX_LIGHT;
    else right_red = 1*MAX_LIGHT;
    if (right_blue) right_blue = 0*MAX_LIGHT;
    else right_blue = 1*MAX_LIGHT;

    for (int j=0; j<5; j++){
      for (int i=0; i<3; i++){
        delay(DELAY_BLINK);  
        setColor(right_red,LED_OFF,right_blue,LED_RIGHT,LED_NUM_LEFT);
        delay(DELAY_BLINK);
        setColor(LED_OFF,LED_OFF,LED_OFF,LED_RIGHT,LED_NUM_LEFT);
      }
      for (int i=0; i<3; i++){
        setColor(left_red,LED_OFF,left_blue,LED_LEFT,LED_NUM_LEFT);
        delay(DELAY_BLINK);
        setColor(LED_OFF,LED_OFF,LED_OFF,LED_LEFT,LED_NUM_LEFT);
      } 
    }
    delay(LONG_DELAY);
    for (int j=0; j<3; j++){ 
      for (int i=0; i<3; i++){
        delay(DELAY_BLINK);  
        setColor(right_red,LED_OFF,right_blue,LED_RIGHT,LED_NUM_LEFT);
        setColor(left_red,LED_OFF,left_blue,LED_LEFT,LED_NUM_LEFT);
        delay(DELAY_BLINK);
        setColor(LED_OFF,LED_OFF,LED_OFF,0,LED_COUNT);
      }
      delay(LONG_DELAY);
    }
  }
}

void loop() {
  for (int i=0;i<LED_COUNT; i++) strip.setPixelColor(i,0,0,0);
  //strip.show();
  setColor(MAX_LIGHT, LED_OFF, LED_OFF, REAR_LEFT_START, 8);
  setColor(MAX_LIGHT, LED_OFF, LED_OFF, REAR_RIGHT_START, 8);
  setColor(LED_OFF, MAX_LIGHT, LED_OFF, FRONT_LEFT_START, 8);
  //setColor(LED_OFF, MAX_LIGHT, LED_OFF, FRONT_RIGHT_START, 8);
  //while(1);
  Running_lighter(REAR_MIDLE_START, REAR_MIDLE_END);
  //Running_lighter(0, REAR_MIDLE_END);
}

