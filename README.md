# Lab 3 - Team Shaggy 
(Bryan Chen, Justin Su, Zhengming (Jack) Chen)

## Introduction

Because all of our Lab 2 musical instruments were along the lines of percussive instruments, that make sounds by hitting objects, we decided to incorporate them together as a band by having each instruments play a different beat to form interesting rhythms together. Instead of using objects to beat a surface, we decided to use the sounds of the servo to generate a beat. Thus, each servo would activate and deactivate to make a sound, in different rhythms from each other but at the same tempo, in order to achieve the goal of forming a “jam band”.

This system would have three main interfaces: User-application, application-servo and servo-servo/synchronization. I took charge of writing the User-application interface, which involves the web server backend. Jack took charge of the user-application interface, which involves translating commands from the user interface into servo movements, and Justin developed the MCU-MCU communication, which is done through light sensors. In addition, Jack developed a method to use the vibration sensor to act as an on/off switch for the whole system, that responds to the user tapping the sensor.

## Bill of Materials

-ESP8266 12E Development Board 

-ESP 12E Motor Shield  

-FS90 Micro 9g servo  

-Jumper cables (included with the servos) 

-Mini-usb cables

-Light-dependent resistors

-Vibration sensors

-50Ohm, 330Ohm, 10kOhm resistors

-Electrical tape

## Wiring Schematic

![Settings Window](https://raw.githubusercontent.com/bryanyuchen/Lab_3/master/image1.png)

          Picture(a):  Wire connect individual                          Picture(b): Wire connect together
	  (Additional images are included in the repo)

![Settings Window](https://raw.githubusercontent.com/bryanyuchen/Lab_3/master/image2.png)

	Circuit Schematic: Resonator circuit on the left, LDR/LED combination on the right.

## Sensing:
### Light-Dependent Resistors: 
When an LED light connected to the face of the LDR is turned on, the resistance of the LDR is lowered, resulting in voltage logic of 1 being read on the digital pin of the MCU. When the LED is off, the resistance of the LDR is high, resulting in a voltage logic of 0 being read on the digital pin of the MCU. To avoid interference and noise from ambient light sources, we used electrical tape to allow only light from the LED to come into contact with the LDR sensor head.

### Vibration sensor: 
When the vibration sensor is shaken, the component’s resistance increases such that a voltage logic of 0 is read on the digital pin of the MCU. When no vibration is detected, the component acts as an effective short, which results in a voltage logic of 1 being read on the digital pin of the MCU. We initialized a state machine-like system where reading the voltage logic of 0 will cause the system to switch between a sleep mode, and an active mode (in which the server is activated), thereby utilizing the vibration sensor as an on/off switch. Since the vibration sensor is not precise enough to measure tightly-packed discrete changes in states (as the vibrations take a non-negligible time to die off), we decided to use it as an on/off switch as we did not anticipate needing to turn it on and off repeatedly in a short amount of time.

### Tables and Figures

Vibration Sensor Data State: 0/1 | Tap: Yes/No | Time (ms)
--- | --- | ---
0 | No | 0
1 | Yes | 10
1 | No | 20
0 | No | 30
0 | No | 40
1 | No | 50
0 | No | 60
1 | No | 70
0 | No | 80
1 | No | 90
1 | No | 100
1 | No | 110
1 | No | 120
1 | No | 130
1 | No | 140

Table 1: As evidenced by the instability of the vibration sensor for several milliseconds after a tap, we needed to use a delay to prevent input from being read on the pin to avoid the transient state. With the delay acting as an effective ignore signal, we were able to more reliably use the vibration sensor as a switch.


LDR State: 0/1 | LED State: 0/1 | Time (ms)
--- | --- | ---
0 | 0 | 0
0 | 0 | 10
0 | 1 | 20
1 | 0 | 30
0 | 1 | 40
1 | 1 | 50
1 | 1 | 60
1 | 0 | 70

Table 2: There is a slight delay in the response between the LDR and LED, but as the states are in the 10s of milliseconds, it is all but unnoticeable. In contrast with the vibration sensor, the LDR is much more precise and easily controlled by the on/off state of the LED.

## Multi-robot Collaboration

Initially, we planned to use the vibration sensor to pick up sound signals when each instrument plays a beat; however, we found that this would incur delays that would throw the system out of sync. Therefore, we decided to go with light sensing technology - LDR’s. Each robotic instrument is connected through the LED/LDR junctions. When one servo plays a beat, it simultaneously flashes an LED at the LDR of the next servo, thus triggering the next servo in the sequence to commence its own rhythmic sequence. That second servo will then signal servo 3 via LED and LDR to propagate the rhythm down the chain. Each successive machine in the chain plays the designated rhythm twice as fast as the previous machine.

## User Interface
  The user-application interface consists of an HTML webpage that takes commands from a user. There are buttons for the user to send three types of commands, enumerated ‘sequence 1’, ‘sequence 2’ and ‘sequence 3’ that direct the system to play three different rhythmic sequences. In addition, each time the user sends a command, the onboard LED will flash 1,2, or 3 times according to the sequence number pressed.
Operation
	To operate this system, the user has access to two controls: the webpage user interface, and the vibration sensor, which acts as the on-off switch for the system. To use the vibration sensor, the user simply taps it to switch the system off, and then again to switch the system back on. For the web page, usage is done by pressing on the three buttons of the interface to play sequence 1, 2 or 3. These sequences are translated into commands to the system in order to play 1 of 3 default rhythms. 
	
## Demonstration 

<a href="http://www.youtube.com/watch?feature=player_embedded&v=DMyxoPTLQOk
" target="_blank"><img src="http://img.youtube.com/vi/DMyxoPTLQOk/0.jpg" 
alt="Lab 3 Demo" width="240" height="180" border="10" /></a>

https://www.youtube.com/watch?v=DMyxoPTLQOk

0:01 - initial shake to wake the system from sleep mode to sequence one

0:10 - transition from sequence one to sequence two

0:20 - transition from sequence two to sequence three
