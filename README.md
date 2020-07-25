# Lap-Counter

This project is a device to count laps when swimming. It uses an ultrasonic sensor to detect the swimmer and counts 50 yds/meters (out and back of a short course standard swimming pool. It calls out the distance on a speaker and shows on a 4 digit 7 seg LED. Future version may display the time also or transmit distance/time to a smart watch.

Uses: https://github.com/going-digital/Talkie for speech synth calling out the numbers
Adafruit librarys for LED display.

### Parts:
1. Generic Arduino Nano clone
2. Mono Amp
   SparkFun Mono Audio Amp Breakout - TPA2005D1
   https://www.sparkfun.com/tutorials/392  
3. Battery Charger Board
   SparkFun LiPo Charger Basic - Micro-USB
4. LiPo Battery - Lithium Ion Battery - 400mAh
5. Battery Booster  https://www.adafruit.com/product/4654 (3v to 5v)
6. 4 digit 7 seg LED. With controlling circuit. https://www.adafruit.com/product/878
7. Momentary Switch
8. On/Off toggle switch
9. 8-ohm Speaker (salvaged) 

![Fritzing Jpg](https://github.com/PeterQuinn925/Lap-Counter/blob/master/lap_counter.jpg)
