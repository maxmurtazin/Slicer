
#include <ShiftRegister74HC595.h>
// create a global shift register object
// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<32> sr(9, 11, 12);
 //32*8=256
int myReg [255] = {};

void setup() {
}

int Key_On (int KeyNumber) { //on
sr.set(KeyNumber, HIGH);
};

int Key_Of (int KeyNumber) { //on
sr.set(KeyNumber, LOW);
};



void loop() {

 Key_On (0); // 0-255

}