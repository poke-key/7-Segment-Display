#include <Arduino.h>

//7-segment display pins and implementation (change pin values if necessary):
int a = 8;
int b = 9;
int c = 11;
int d = 12;
int e = 13;
int f = 7;
int g = 6;
int dp = 10;
int LEDS[7] {a, b, c, d, e, f, g};
//Some values for a-g are provided as examples, implement the remaining a-g values corresponding to the comments below:
//1 = Segment ON, 0 = Segment OFF
int nums[17][7] {
  {1, 1, 1, 1, 1, 1, 0}, //0
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4 		<- Implement!
  {1, 0, 1, 1, 0, 1, 1}, //5 		<- Implement!
  {1, 0, 1, 1, 1, 1, 1}, //6 		<- Implement!
  {1, 1, 1, 0, 0, 0, 0}, //7 		<- Implement!
  {1, 1, 1, 1, 1, 1, 1}, //8 		<- Implement!
  {1, 1, 1, 1, 0, 1, 1}, //9 		<- Implement!
  {1, 1, 1, 0, 1, 1, 1}, //10 (A)
  {0, 0, 1, 1, 1, 1, 1}, //11 (b)
  {1, 0, 0, 1, 1, 1, 0}, //12 (C)	<- Implement in hex!
  {0, 1, 1, 1, 1, 0, 1}, //13 (d)	<- Implement in hex!
  {1, 0, 0, 1, 1, 1, 1}, //14 (E)	<- Implement in hex!
  {1, 0, 0, 0, 1, 1, 1}, //15 (F)	<- Implement in hex!
  {0, 0, 0, 0, 0, 0, 0} //off
// a  b  c  d  e  f  g
};

//Call this function in your code to output the integer x on the 7-segment display in hex
void outNum(int x) {
  for (int i = 0; i < 7; i++) {
    if (nums[x][i] == 1) {
      digitalWrite(LEDS[i], 1);
    }
    else {
      digitalWrite(LEDS[i], 0);
    }
  }
}

String incomingSTR; // for incoming serial data
bool inputReceived = false; //flag for decimal
bool bitSelected = false; //flag for LED
// put function declarations here:
int myFunction(int, int);


void setup()
{
//Define digital pins here:
pinMode(a, OUTPUT);
pinMode(b, OUTPUT);
pinMode(c, OUTPUT);
pinMode(d, OUTPUT);
pinMode(e, OUTPUT);
pinMode(f, OUTPUT);
pinMode(g, OUTPUT);
pinMode(dp, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {
	; // wait for serial port to connect. Needed for native USB
  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
  if (!inputReceived && !bitSelected) {
   Serial.println("Input a number from 0-15:");
   inputReceived = true; //set flag to true 
  }
  if (Serial.available() > 0 && !bitSelected) {  
    // read the incoming byte:
    incomingSTR = Serial.readString();
	  int incomingByte = atoi(incomingSTR.c_str());
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingSTR);
    Serial.print("Binary representation: ");
    Serial.println(incomingByte, BIN);
    Serial.print("HEX representation: ");
    Serial.println(incomingByte, HEX);
    
    /*perform bitwise AND for mask*/
    digitalWrite(2, incomingByte & 0x1);
    digitalWrite(3, incomingByte & 0x2);
    digitalWrite(4, incomingByte & 0x4);
    digitalWrite(5, incomingByte & 0x8);
     
    inputReceived = false; //set to false again
    bitSelected = true; //get out and select a bit

    Serial.println("Select a bit from the LED between (0-3) to right shift:");

    outNum(incomingByte);
   } 

   if (Serial.available() > 0 && bitSelected) {
    int selectedBit = Serial.parseInt(); //read the bit
    Serial.print("Entered value: ");
    Serial.println(selectedBit);


    if(selectedBit >= 0 && selectedBit <= 3) {
      bool bitValue = digitalRead(selectedBit + 2); //read selected bit and add 2 to get to the pin values

      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      if(bitValue) {
      digitalWrite(selectedBit+2, HIGH);

        for(int i = selectedBit + 3; i > 2; i--) {   //iterate until pin 2(last pin)

          digitalWrite(i, LOW);
          digitalWrite(i-1, HIGH); 
          outNum(0x0001 << i - 3);
          delay(1000);   //delay to show the shift effect
        }
      }
      else {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);     
        outNum(0);  
      }
    }
      bitSelected = false;
    
    }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

