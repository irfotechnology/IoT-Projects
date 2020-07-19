import net.java.games.input.*;
import org.gamecontrolplus.*;
import org.gamecontrolplus.gui.*;

import cc.arduino.*;
import org.firmata.*;

/*
arduino_output

Demonstrates the control of digital pins of an Arduino board running the
StandardFirmata firmware.  Clicking the squares toggles the corresponding
digital pin of the Arduino.  

To use:
* Using the Arduino software, upload the StandardFirmata example (located
  in Examples > Firmata > StandardFirmata) to your Arduino board.
* Run this sketch and look at the list of serial ports printed in the
  message area below. Note the index of the port corresponding to your
  Arduino board (the numbering starts at 0).  (Unless your Arduino board
  happens to be at index 0 in the list, the sketch probably won't work.
  Stop it and proceed with the instructions.)
* Modify the "arduino = new Arduino(...)" line below, changing the number
  in Arduino.list()[0] to the number corresponding to the serial port of
  your Arduino board.  Alternatively, you can replace Arduino.list()[0]
  with the name of the serial port, in double quotes, e.g. "COM5" on Windows
  or "/dev/tty.usbmodem621" on Mac.
* Run this sketch and click the squares to toggle the corresponding pin
  HIGH (5 volts) and LOW (0 volts).  (The leftmost square corresponds to pin
  13, as if the Arduino board were held with the logo upright.)
  
For more information, see: http://playground.arduino.cc/Interfacing/Processing
*/

import processing.serial.*;

Arduino arduino;

ControlDevice cont;
ControlIO control;

color off = color(4, 79, 111);
color on = color(84, 145, 158);

int value = Arduino.LOW;
float btnValue;

void setup() {
  size(470, 200);
  
  control = ControlIO.getInstance(this);
  cont = control.getMatchedDevice("joystick");
  
  if( cont == null){
     println("no controller found!"); 
     System.exit(-1);
  }
  
  // Prints out the available serial ports.
  //println(Arduino.list());
  
  // Modify this line, by changing the "0" to the index of the serial
  // port corresponding to your Arduino board (as it appears in the list
  // printed by the line above).
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  
  // Alternatively, use the name of the serial port corresponding to your
  // Arduino (in double-quotes), as in the following line.
  // arduino = new Arduino(this, "/dev/tty.usbmodem621", 57600);
  
  // Set the Arduino digital pins as outputs.
  //for (int i = 0; i <= 13; i++)
  arduino.pinMode(13, Arduino.OUTPUT);

}

public void getUserInput(){
    println(cont.getButton("onoff").getValue());
    println(cont.getSlider("zAxis").getValue());
    btnValue = map(cont.getButton("onoff").getValue(), 0, 8 , Arduino.LOW, Arduino.HIGH);
    //println(btnValue);
}

void draw() {
  int pin = 13; 
  background(off);
  stroke(on);
  getUserInput();
  value = (int)btnValue;
  if (value == Arduino.HIGH)
     fill(on);
   else
     fill(off);
      
    rect(30, 30, 20, 20);
  if(value == Arduino.LOW) {
      arduino.digitalWrite(pin, Arduino.HIGH);
      value = Arduino.HIGH;
    }
  else {
      arduino.digitalWrite(pin, Arduino.LOW);
      value = Arduino.LOW;
    }
}

void mousePressed()
{
  int pin = 13; // (450 - mouseX) / 30;
  // Toggle the pin corresponding to the clicked square.
  value = (int)btnValue;
  if (value == Arduino.LOW) {
    arduino.digitalWrite(pin, Arduino.HIGH);
    value = Arduino.HIGH;
  } else {
    arduino.digitalWrite(pin, Arduino.LOW);
    value = Arduino.LOW;
  }
}
