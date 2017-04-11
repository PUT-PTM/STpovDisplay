#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2            //Diods connection pin.
#define DIODS      16               //Number of diods.
const byte interruptPin = 16;       //Interruption pin.
volatile byte interruptCounter =  0;//Number of interrupts 0/1/2.
unsigned long time[4];                        //Time of loops, 4 -> last loop.
unsigned long avgTime;
byte picture[180][16][3];   //We have to write picture generator.
int pictureLine = 0;
int lastLine = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(DIODS, PIN, NEO_GRB + NEO_KHZ800);


void setup() 
{
  for(int i=0;i<4;i++)
    time[i]=0;
  avgTime=0;
  
  pixels.begin(); // This initializes the NeoPixel library.
  
  pinMode(interruptPin, INPUT_PULLUP);    //Interrupt initialization
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
}

void handleInterrupt()
{
  switch(interruptCounter)
  {
    case 0:
      time[0] = time[3] - micros();
      interruptCounter++;      
      break;

    case 1:
      time[1] = time[3] - micros();
      interruptCounter++;      
      break;

    case 2:
      time[2] = time[3] - micros();
      interruptCounter=0;   
      break;

    time[3]=micros();
    avgTime = (time[0] + time[1] + time[2])/3;  //Average loop time.
  }
}

void loop() {
  pictureLine = ((int)((float)(time[3] - milis())/avgTime))*180.0);

  if(pictureLine!=lastLine)
  {
      for(int i=0;i<NUMPIXELS;i++)
    {
     // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     pixels.setPixelColor(i, pixels.Color(picture[pictureLine][i][0]*150, picture[pictureLine][i][1]*150, picture[pictureLine][i][2]*150)); // Set pixel color.
  
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
}
