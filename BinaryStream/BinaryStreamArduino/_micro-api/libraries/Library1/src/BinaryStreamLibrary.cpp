#include "Arduino.h"
#include "BinaryStreamLibrary.h"
#include "foo/Baa.h"

BinaryStreamLibraryClass::BinaryStreamLibraryClass()
{
}

BinaryStreamLibraryClass::~BinaryStreamLibraryClass()
{
}

void BinaryStreamLibraryClass::hello(String msg)
{
	Serial.println(msg);
}
