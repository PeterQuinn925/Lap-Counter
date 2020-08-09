#define echoPin 5 // attach pin D5 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D6 Arduino to pin Trig of HC-SR04
#define interruptPin 2 // attach interrupt switch to pin D2
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
//Pin A4 to DAT on LED
//Pin A5 to CLK on LED
Adafruit_7segment matrix = Adafruit_7segment();
//const byte interruptPin = 2;
volatile boolean restart_now = true;

// defines variables
unsigned long start_time;
unsigned long last_lap_t;
unsigned long last_clock;
int lap_count;
int yards;
int swap; //counter to determine when to swap time vs yards. Will swap every n loop iterations
float ave_time;
unsigned long min_lap_t = 40000; //40 seconds = 40000
unsigned long current_t;
int d1;//debugging
int num_ave = 3; //average this number of distance readings
int min_distance = 75; //trip lap counter when detecting an echo at 75 cm or less
bool disp_yards = true;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), restart, LOW);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  matrix.begin(0x70);
  matrix.println(333, DEC);
  matrix.writeDisplay();
  start_time = millis();
  last_lap_t = start_time;
}

void loop() {
  swap++;
  if (restart_now) {//Interrupt just finished, reset all times
    restart_now = false;
    //give me a chance to get ready and dive in after turing on the counter
    delay(1000);
    matrix.println(333, DEC);
    matrix.writeDisplay();
    delay(500);
    matrix.println(22, DEC);
    matrix.writeDisplay();
    delay(1000);
    matrix.println(1, DEC);
    matrix.writeDisplay();
    delay(1000);
    matrix.println(0000, DEC);
    matrix.writeDisplay();
    delay(1000);
    restart_now = false;//for some reason it's running twice. This cures it.
    start_time = millis();
    last_clock = start_time;
    last_lap_t = start_time;
    lap_count = 0;
    yards = 0;
    ave_time = 0;
    Serial.println("Lap Counter");
  }
  current_t = millis();

  //Serial.println(current_t -last_lap_t);
  if (not disp_yards)
  {
    if (TimeToInt(last_clock) < TimeToInt(millis() - start_time)) {
      matrix.println(TimeToInt(millis() - start_time));
      matrix.drawColon(true);
      matrix.writeDisplay();
      last_clock = millis()-start_time;
      //Serial.println(TimeToInt(millis() - start_time));
    }
  }
  if (swap % 50 == 0)
  { if (disp_yards)
    { disp_yards = false;
    }
    else {
      matrix.println(yards);
      matrix.drawColon(false);
      matrix.writeDisplay();
      //Serial.println(yards);
      disp_yards = true;
    }
  }
  //delay(1000);
  if (GetDist() < min_distance && (current_t - last_lap_t > min_lap_t))
  {
    lap_count++;
    yards = lap_count * 50;
    do {
      Serial.print("Distance:");
      Serial.println(yards);
      Serial.print("Elapsed time: "); \
      Serial.println(TimeToString(millis() - start_time));
      Serial.print("Lap time: ");
      Serial.println(TimeToString(millis() - last_lap_t));
      matrix.println(yards);
      matrix.writeDisplay();
      matrix.drawColon(false);
      delay(3000);
    }
    while (GetDist() < min_distance);
    last_lap_t = millis();
    matrix.println(yards);
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
// only needed for debug
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
int TimeToInt (unsigned long t) //will format the time as mmss in an int. With a colon will display as mm:ss
{
  int t_int;
  t = t / 1000; //convert t to seconds
  long h = t / 3600; //ignore the h
  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  t_int = m * 100 + s;
  return t_int;
}

void restart() {//restart on pin2 brought low
  restart_now = true;
}
