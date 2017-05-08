#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2            //Diods connection pin.
#define DIODS      16               //Number of diods.
//#define avgTime 4000.0              //Avg time of loop
bool interruptsSwitch;
static byte interruptPin = 5;       //Interruption pin.
volatile byte interruptCounter =  0;//Number of interrupts 0/1/2.
unsigned long time[4];                        //Time of loops, 4 -> last loop.
unsigned long avgTime = 4000;
byte picture[180][16][3];   //We have to write picture generator.
int pictureLine = 0;
int lastLine = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(DIODS, PIN, NEO_GRB + NEO_KHZ800);

//bool test=false;

void setup() 
{
  interrupts();
  interruptsSwitch = true;
  Serial.begin(115200);
  Serial.println("Serial start");
  for(int i=0;i<4;i++)
    time[i]=1;
  avgTime=1;
  
  pixels.begin(); // This initializes the NeoPixel library.
  
  pinMode(interruptPin, INPUT_PULLUP);    //Interrupt initialization
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, RISING);

  for(int i=0;i<180;i++)
  {
    if(i<45)
    {
      for(int j=0;j<16;j++)
      {
        picture[i][j][0]=1;
        picture[i][j][1]=0;
        picture[i][j][2]=0;
      }
    }
    else if(i<90)
    {
      for(int j=0;j<16;j++)
      {
        picture[i][j][0]=0;
        picture[i][j][1]=1;
        picture[i][j][2]=0;
      }
    }
    else if(i<135)
    {
      for(int j=0;j<16;j++)
      {
        picture[i][j][0]=0;
        picture[i][j][1]=0;
        picture[i][j][2]=1;
      }
    }
    else
      {
      for(int j=0;j<16;j++)
      {
        picture[i][j][0]=1;
        picture[i][j][1]=1;
        picture[i][j][2]=0;
      }
    }
  }
}

void handleInterrupt()
{
  if(interruptsSwitch)
  { 
    interruptsSwitch=false;
    Serial.println("Interurupt ");
    noInterrupts();
    
    Serial.println("Interurupts deactivated");
    Serial.println(micros() - time[3]);
  
    switch(interruptCounter)
    {
      case 0:
        time[0] = micros() - time[3];
        interruptCounter++;      
        break;

      case 1:
        time[1] = micros() - time[3];
        interruptCounter++;      
        break;

      case 2:
        time[2] = micros() - time[3];
        interruptCounter=0;   
        break;
    }  
    time[3]= micros();
    avgTime = ((time[0] + time[1] + time[2])/3);  //Average loop time.
  }
}

void loop() 
{
  if(!interruptsSwitch && ((micros() - time[3])>10000))
  {
    interrupts();
    interruptsSwitch=true;
    Serial.println("Interurupts activated");
  }
  
 if((micros() - time[3])<avgTime)
 {
   pictureLine = (int)((((double)(micros() - time[3])/(double)avgTime))*180.0);
  Serial.println(micros() - time[3]);
  Serial.println(avgTime);
  if(pictureLine!=lastLine)
  {
    Serial.println(pictureLine);
    lastLine = pictureLine;
      for(int i=0;i<DIODS;i++)
    {
     // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     pixels.setPixelColor(i, pixels.Color(picture[pictureLine][i][0]*150, picture[pictureLine][i][1]*150, picture[pictureLine][i][2]*150)); // Set pixel color.
  
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  
  //delayMicroseconds((int)(avgTime/180));
 }
  
}
