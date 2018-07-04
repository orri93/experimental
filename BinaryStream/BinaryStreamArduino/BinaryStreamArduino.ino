/*
 Name:		BinaryStreamArduino.ino
 Created:	2018-06-24 00:02:13
 Author:	SigurdssonGO
*/

#include "Library1.h"
#include "src\LocalHeader1.h"

Library1Class lib1 = Library1Class();

// define prototypes for any methods that use 'user types' created in .ino code or use cpp/h files
//
void test(Library1Class _lib1);


// methods below here
//

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
	test(lib1);

	delay(100);
}

//This method demonstrates 1) using a library 2) using user types as function params
void test(Library1Class _lib1)
{
	_lib1.hello("Hello Visual Micro");
}

