#include <Adafruit_NeoPixel.h>
#include <Adafruit_LEDBackpack.h>
#include <avr/power.h>

#define PIN			13
#define NUMPIXELS	8	
#define WAIT		0
#define READ_VALUE	1
#define BUTTONA		2
#define PRESSED		LOW

Adafruit_7segment matrix = Adafruit_7segment();                     
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t thisTime = 0, lastTime = 0;
int state = WAIT; 
String inString = "";
char inCmd;

void set_pixels(uint8_t value) {
	for (int x=0; x<8; x++) {
		
		if (value & (1<<x)) {
			pixels.setPixelColor(x, pixels.Color(50,0,0));
		} 
		else {
			pixels.setPixelColor(x, pixels.Color(0,0,0));
		}
	}
	pixels.show();
}

void run_cmd(char cmd, int value) {
	if (cmd == 'A')
		set_pixels(value);
	else if (cmd == 'B') {
 		matrix.print(value, DEC);        
       	matrix.writeDisplay(); 
	}

	
}


void setup(){
	Serial.begin(9600);

	matrix.begin(0x70);                                         
   	matrix.print(0, DEC);                                       
   	matrix.writeDisplay(); 

	pixels.begin();
	set_pixels(0x00);

	pinMode(BUTTONA, INPUT_PULLUP);
}


void loop() {
	if (Serial.available()) {
		int inChar = Serial.read();
		if (state == READ_VALUE) {
			if (isDigit(inChar))
				inString += (char)inChar;                   
			else if (inChar == '\n') {
				int inValue = inString.toInt();	
				run_cmd(inCmd, inValue);
				inString = "";
				state = WAIT;
			}
		}
		else if (state == WAIT) {
			if (isUpperCase(inChar)) {
				inCmd = (char)inChar;
				state = READ_VALUE;
			}	
      	}                                                   
	}
	thisTime = millis();
	if (thisTime - lastTime >= 1000) {
		int aVal = analogRead(3);
		if (BUTTONA == PRESSED) 
				bVal = true;
		else
				bval = false;
		Serial.print(y);
		lastTime = thisTime;
	}
}

