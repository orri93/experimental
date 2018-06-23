// BinaryStream.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

#include <stdint.h>

#include <Windows.h>

#include <memory>

void Split16ToTwo8(uint16_t value, uint8_t* low, uint8_t* high);
void Split8ToTwo4(uint8_t value, uint8_t* low, uint8_t* high);
uint8_t CombineTwo4To8(uint8_t low, uint8_t high);
uint16_t CombineTwo8To16(uint8_t low, uint8_t high);

void FourByteStream(uint16_t value, uint8_t* output);
void ParseStream(
  uint16_t*output,
  long outlen,
  long* outread,
  uint8_t* buffer,
  int start,
  int length,
  int* lastReadablePosition
  );
void ReadFromTheSerialPort(HANDLE hSerialPort);

int _tmain(int argc, _TCHAR* argv[])
{
  char ComPortName[] = "\\\\.\\COM11";

  HANDLE hSerialPort;               // Handle to the Serial port
  BOOL Status;                      // Status of the various operations
  DWORD dwEventMask;                // Event mask to trigger

  hSerialPort = ::CreateFileA(
    ComPortName,    // Name of the Port to be Opened
    GENERIC_READ,   // Read only   
    0,              // No Sharing, ports can't be shared
    NULL,           // No Security
    OPEN_EXISTING,  // Open existing port only
    0,              // Non Overlapped I/O
    NULL            // Null for COM Devices
    );

  if (hSerialPort != INVALID_HANDLE_VALUE)
  {
    printf("\n\n    Serial Port %s opened Successfully", ComPortName);
  }
  else
  {
    printf("\n    Error! - Port %s can't be opened\n", ComPortName);
    return -1;
  }

  DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  Status = GetCommState(hSerialPort, &dcbSerialParams);      //retreives  the current settings

  if (Status == FALSE)
    printf("\n    Error! in GetCommState()");

  dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 115200
  dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
  dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
  dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

  Status = ::SetCommState(hSerialPort, &dcbSerialParams);  //Configuring the port according to settings in DCB 

  if (Status == FALSE)
  {
    printf("\n    Error! in Setting DCB Structure");
  }
  else //If Successfull display the contents of the DCB Structure
  {
    printf("\n\n    Setting DCB Structure Successfull\n");
    printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
    printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
    printf("\n       StopBits = %d", dcbSerialParams.StopBits);
    printf("\n       Parity   = %d", dcbSerialParams.Parity);
  }

  COMMTIMEOUTS timeouts = { 0 };
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if (::SetCommTimeouts(hSerialPort, &timeouts))
  {
    printf("\n\n    Setting Serial Port Timeouts Successfull");
  }
  else
  {
    printf("\n\n    Error! in Setting Time Outs");
    return -1;
  }

  Status = ::SetCommMask(hSerialPort, EV_RXCHAR); // Configure Windows to Monitor the serial device for Character Reception

  if (Status)
  {
    printf("\n\n    Setting CommMask successfull");
  }
  else
  {
    printf("\n\n    Error! in Setting CommMask");
    return -1;
  }
  
  printf("\n\n    Waiting for Data Reception");

  Status = ::WaitCommEvent(hSerialPort, &dwEventMask, NULL); //Wait for the character to be received
  if (Status)
  {
    printf("\n\n    Stream Received");
    ReadFromTheSerialPort(hSerialPort);
  }
  else
  {
    printf("\n    Error! in Setting WaitCommEvent()");
    return -1;
  }

	return 0;
}

void ReadFromTheSerialPort(HANDLE hSerialPort)
{
  const int outputLength = 127 ;
  const int bufferLength = (outputLength + 1) * 4;
  BOOL status;
  uint16_t output[outputLength];
  uint8_t serialBuffer[bufferLength];   // Buffer Containing Rxed Data
  long outputRead;
  std::unique_ptr<uint8_t> combined;    // The combined buffer with remainder from last read and new read
  std::unique_ptr<uint8_t> remainder;   // The remainder from the last read
  size_t remainderLength = 0;
  DWORD dwNoBytesRead;                  // Bytes read by ReadFile()
  int lastReadPosition;
  int i = 0;

  while (true)
  {
    status = ::ReadFile(hSerialPort, (LPVOID)serialBuffer, bufferLength, &dwNoBytesRead, NULL);
    if (status)
    {
      combined.reset(new uint8_t[remainderLength + dwNoBytesRead]);
      if (remainderLength > 0)
      {
        ::memcpy(combined.get(), remainder.get(), remainderLength);
      }
      ::memcpy(combined.get() + remainderLength, serialBuffer, dwNoBytesRead);
      ParseStream(output, outputLength, &outputRead, combined.get(), 0, dwNoBytesRead, &lastReadPosition);
      printf("\n\n");
      for (int i = 0; i < outputRead; i++)
      {
        printf("%d\n", output[i]);
      }
      remainderLength = dwNoBytesRead - lastReadPosition;
      if (remainderLength > 0)
      {
        remainder.reset(new uint8_t[remainderLength]);
        ::memcpy(remainder.get(), serialBuffer + lastReadPosition, remainderLength);
      }
    }
    else
    {
      printf("\n    Error! While reading from serial port");
    }
  }
}

void Split16ToTwo8(uint16_t value, uint8_t* low, uint8_t* high)
{
  *low = value & 0xff;
  *high = (value >> 8);
}

void Split8ToTwo4(uint8_t value, uint8_t* low, uint8_t* high)
{
  *low = value & 0x0f;
  *high = (value >> 4);
}

uint8_t CombineTwo4To8(uint8_t low, uint8_t high)
{
  return low | (high << 4);
}

uint16_t CombineTwo8To16(uint8_t low, uint8_t high)
{
  return ((uint16_t)low) | (((uint16_t)high) << 8);
}

void FourByteStream(uint16_t value, uint8_t* output)
{
  uint8_t low8, high8, low8low4, low8high4, high8low4, high8high4;
  Split16ToTwo8(value, &low8, &high8);
  Split8ToTwo4(low8, &low8low4, &low8high4);
  Split8ToTwo4(high8, &high8low4, &high8high4);

  low8low4 |= 0x10;   // no 1.
  low8high4 |= 0x20;  // no 2.
  high8low4 |= 0x30;  // no 3.
  high8high4 |= 0x40; // no 4.

  output[0] = low8low4;
  output[1] = low8high4;
  output[2] = high8low4;
  output[3] = high8high4;
}

void ParseStream(uint16_t* output, long outlen, long* outread, uint8_t* buffer, int start, int length, int* lastReadablePosition)
{
  *outread = 0;
  *lastReadablePosition = start;
  uint8_t read, first, second, third, fourth;
  int findCount = 0;
  long valueReadCount = 0;
  for (int i = start; i < length; i++)
  {
    *lastReadablePosition = i;
    if (((read = buffer[i]) & 0xf0) == 0x10) {
      if (findCount == 0)
      {
        first = read;
        findCount++;
      }
      else 
      {
        // garbage, start over
        findCount = 0;
      }
    }
    else if (((read = buffer[i]) & 0xf0) == 0x20) {
      if (findCount == 1)
      {
        second = read;
        findCount++;
      }
      else
      {
        // garbage, start over
        findCount = 0;
      }
    }
    else if (((read = buffer[i]) & 0xf0) == 0x30) {
      if (findCount == 2)
      {
        third = read;
        findCount++;
      }
      else
      {
        // garbage, start over
        findCount = 0;
      }
    }
    else if (((read = buffer[i]) & 0xf0) == 0x40) {
      if (findCount == 3)
      {
        fourth = read;
        findCount++;
      }
      else
      {
        // garbage, start over
        findCount = 0;
      }
    }
    else
    {
      // garbage, start over
      findCount = 0;
    }

    if (findCount == 4) {
      uint8_t low8 = CombineTwo4To8(first & 0xf, second & 0xf);
      uint8_t high8 = CombineTwo4To8(third & 0xf, fourth & 0xf);
      uint16_t value = CombineTwo8To16(low8, high8);
      output[valueReadCount] = value;
      valueReadCount++;
      if (valueReadCount >= outlen)
      {
        *outread = valueReadCount;
        return;
      }
      findCount = 0;
    }
    *outread = valueReadCount;
  }
}
