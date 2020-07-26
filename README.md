# Lap-Counter

This project is a device to count laps when swimming. It uses an ultrasonic sensor to detect the swimmer and counts 50 yds/meters (out and back of a short course standard swimming pool. It calls out the distance on a speaker and shows on a 4 digit 7 seg LED. 

The container is a reused takeout container. I poked some holes in it for the ultrasonic "eyes" and put a couple of rolls of pennies in the bottom for ballast. It's ugly for sure.

Right now the volume on the speaker is still pretty low, even with the amp. It's also much noisier with the addition of the LED. I'm considering adding capacitors to reduce the noise or even getting rid of the speaker entirely. Other future considerations is to show the elapse time and the lap time - but I'm not sure what the best way to do that is. Future version may display the time also or transmit distance/time to a smart watch.

Uses: https://github.com/going-digital/Talkie for speech synth calling out the numbers
Adafruit librarys for LED display.

### Parts:
1. Generic Arduino Nano clone
2. Ultrasonic Sensor HC-SR04
3. Mono Amp
   SparkFun Mono Audio Amp Breakout - TPA2005D1
   https://www.sparkfun.com/tutorials/392  
4. Battery Charger Board
   SparkFun LiPo Charger Basic - Micro-USB
5. LiPo Battery - Lithium Ion Battery - 400mAh
6. Battery Booster  https://www.adafruit.com/product/4654 (3v to 5v)
7. 4 digit 7 seg LED. With controlling circuit. https://www.adafruit.com/product/878
8. Momentary Switch
9. On/Off toggle switch
10. 8-ohm Speaker (salvaged) 

![Fritzing Jpg](https://github.com/PeterQuinn925/Lap-Counter/blob/master/lap_counter.jpg)
![Photo](https://github.com/PeterQuinn925/Lap-Counter/blob/master/lap_counter_photo.jpg)
