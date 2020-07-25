#define echoPin 5 // attach pin D5 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D6 Arduino to pin Trig of HC-SR04
#define amponPin 12 //attach pin D12 to the SD pin of amp to control power saving
#define interruptPin 2 // attach interrupt switch to pin D2
#include "Talkie.h"
#include "Vocab_US_Large.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
//Pin A4 to DAT on LED
//Pin A5 to CLK on LED
Adafruit_7segment matrix = Adafruit_7segment();
//talkie uses digital pin 3
Talkie voice;
//const byte interruptPin = 2;
volatile boolean restart_now = true;

// defines variables
unsigned long start_time;
unsigned long last_lap_t;
int lap_count;
int yards;
float ave_time;
unsigned long min_lap_t = 40000; //40 seconds = 40000
unsigned long current_t;
int d1;//debugging
int num_ave = 3; //average this number of distance readings
int min_distance = 75;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(amponPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), restart, LOW);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  matrix.begin(0x70);
  start_time = millis();
  last_lap_t = start_time;
  // Serial.println("Lap Counter");
}

void loop() {
  int yards;
  if (restart_now) {//Interrupt just finished, reset all times
    restart_now = false;
    digitalWrite(amponPin, HIGH); // turn on the amp
    Serial.println("amp on");
    voice.say(sp4_READY);
    voice.say(sp2_TWO);
    voice.say(sp4_START);
    delay(1000);
    matrix.println(333,DEC);
    matrix.writeDisplay();
    voice.say(sp2_THREE);
    delay(500);
    matrix.println(22,DEC);
    matrix.writeDisplay();
    voice.say(sp2_TWO);
    delay(500);
    matrix.println(1,DEC);
    matrix.writeDisplay();    
    voice.say(sp2_ONE);
    delay(500);
    matrix.println(0000,DEC);
    matrix.writeDisplay();
    voice.say(sp2_GO);
    digitalWrite(amponPin, LOW); // turn off the amp
    Serial.println("amp off");
    delay(100);
    restart_now = false;//for some reason it's running twice. This cures it.
    start_time = millis();
    last_lap_t = start_time;
    lap_count = 0;
    yards = 0;
    ave_time = 0;
    Serial.println("Lap Counter");
  }
  current_t = millis();

  //Serial.println(current_t -last_lap_t);
  if (GetDist() < min_distance && (current_t - last_lap_t > min_lap_t))
  {
    lap_count++;
    yards = lap_count * 50;
    while (GetDist() < min_distance) {
      Serial.print("Distance:");
      Serial.println(yards);
      Serial.print("Elapsed time: "); \
      Serial.println(TimeToString(millis() - start_time));
      Serial.print("Lap time: ");
      Serial.println(TimeToString(millis() - last_lap_t));
      matrix.println(yards);
      matrix.writeDisplay();
      CallLap(yards);
      delay(3000);
    }
    last_lap_t = millis();
  }
}

int GetDist()
{
  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement
  int sum_dist = 0; // average distance intermediate
  d1++;
  for (int i = 1; i <= num_ave; i++)
  {
    //Serial.println("GetDist()");
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    sum_dist = sum_dist + distance;
  }
  distance = sum_dist / num_ave;
  if (d1 % 50 == 0) {
    Serial.println(distance);
  }
  return distance;
}

// t is time in miliseconds
char * TimeToString(unsigned long t)
{
  static char str[12];
  t = t / 1000; //convert t to seconds
  long h = t / 3600;
  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  sprintf(str, "%02d:%02d", m, s);
  //sprintf(str, "%04ld:%02d:%02d", h, m, s);
  return str;
}

void CallLap(int dist) //expects 100, 150, 200, 250, etc
{
  int hund = dist / 100 ; //number of hundreds
  int fifty = (dist % 100) / 50; //0 if even hundred, 1 if a 50
  int thou = dist / 1000 ; // will call out thousands differently

  digitalWrite(amponPin, HIGH); // turn on the amp
  Serial.println("amp on");
  //for 100 - 900 just say n hundred, for the 50s, just add on 50
  //for 1100 - 1950 do the same
  if (dist < 1000 || (dist >= 1100 && dist <= 1950)) {

    if (hund > 0) {
      SayNum(hund);
    }
    if (fifty > 0) {
      voice.say(sp3_FIFTY);
    }
    else
    {
      voice.say(sp3_HUNDRED);
    }
  }
  //if equal to an even thousand or thousand 50
  else if (dist % 1000 < 51)
  {
    SayNum(thou);
    voice.say(sp2_THOUSAND);
    if (fifty > 0) {
      voice.say(sp3_FIFTY);
    }
  }
  // for the 2100 - 2950 say 21 hundred, 22 hundred, 21 50, 22 50, etc
  else if (dist >= 2100 && dist <= 2950) {
    hund = (dist - 2000) / 100;
    String fnam1 = String(hund, DEC);
    String fnam2 = String(fnam1 + ".wav");
    voice.say(sp2_TWENTY);
    SayNum(hund);
    if (fifty > 0) {
      voice.say(sp3_FIFTY);
    }
    else
    {
      voice.say(sp3_HUNDRED);
    }
  }
  else {  //more than 3000
    SayNum(thou);
    voice.say(sp2_THOUSAND);

    if (hund > 0) {
      hund = hund - thou * 10;
      SayNum(hund);
    }
    if (fifty > 0) {
      voice.say(sp3_FIFTY);
    }
    else
    {
      voice.say(sp3_HUNDRED);
    }
  }
  digitalWrite(amponPin, LOW); // turn off the amp
  Serial.println("amp off");
}


void SayNum(int num)
{
  Serial.println(num);
  switch (num)
  {
    case 1:
      voice.say(sp3_ONE);
      break;
    case 2:
      voice.say(sp3_TWO);
      break;
    case 3:
      voice.say(sp3_THREE);
      break;
    case 4:
      voice.say(sp3_FOUR);
      break;
    case 5:
      voice.say(sp3_FIVE);
      break;
    case 6:
      voice.say(sp3_SIX);
      break;
    case 7:
      voice.say(sp3_SEVEN);
      break;
    case 8:
      voice.say(sp3_EIGHT);
      break;
    case 9:
      voice.say(sp3_NINE);
      break;
    case 11://skip 10 - this is a special case
      voice.say(sp3_ELEVEN);
      break;
    case 12:
      voice.say(sp3_TWELVE);
      break;
    case 13:
      voice.say(sp3_THIRTEEN);
      break;
    case 14:
      voice.say(sp3_FOURTEEN);
      break;
    case 15:
      voice.say(sp3_FIFTEEN);
      break;
    case 16:
      voice.say(sp3_SIXTEEN);
      break;
    case 17:
      voice.say(sp3_SEVENTEEN);
      break;
    case 18:
      voice.say(sp3_EIGHTEEN);
      break;
    case 19:
      voice.say(sp3_NINETEEN);
      break;
    case 20:
      voice.say(sp3_TWENTY);
      break;
    default:
      voice.say(sp3_ERROR);
      break;
  }
}
void restart() {//restart on pin2 brought low
  restart_now = true;
}
