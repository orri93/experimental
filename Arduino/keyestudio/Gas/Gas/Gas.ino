#include <SimpleDHT.h>

const int gasSensorPin = A0;
const int alcoholSensorPin = A1;
const int soundSensorPin = A2;
const int photocellSensorPin = A3;

const byte flameSensorPin = 2;
const byte vibrationSensorPin = 3;
const byte dht11pin = 4;
const byte pirSensorPin = 5;

SimpleDHT11 dht11;

struct SensorValues {
  int Gas;
  int Alcohol;
  int Sound;
  int Photocell;
  int Flame;
  int Vibration;
  int Pir;
  struct Dht11 {
    byte Temperature;
    byte Humidity;
  } DhtValues;
};

void OutputHeader();
void OutputFormat(SensorValues* sensorValues);

void setup()
{
  pinMode(flameSensorPin, INPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(pirSensorPin, INPUT);
  
  Serial.begin(9600); //Set serial baud rate to 9600 bps
  OutputHeader();
}

void loop()
{
  SensorValues values;
  
  values.Gas = analogRead(gasSensorPin);
  values.Alcohol = analogRead(alcoholSensorPin);
  values.Sound = analogRead(soundSensorPin);
  values.Photocell = analogRead(photocellSensorPin);

  values.Flame = digitalRead(flameSensorPin);
  values.Vibration = digitalRead(vibrationSensorPin);
  values.Pir = digitalRead(pirSensorPin);

  byte dht11Temperature, dht11Humidity;

  int dht11Error = SimpleDHTErrSuccess;
  dht11Error = dht11.read(dht11pin, &dht11Temperature, &dht11Humidity, NULL);
  if(dht11Error != SimpleDHTErrSuccess) {
    dht11Temperature = -1;
    dht11Humidity = -1;
  }

  OutputFormat(&values);
  
  delay(2000);
}

void OutputHeader() {
  Serial.println("gas, alcohol, sound, photocell, flame, vibration, pir, dhtTemperature, dhtHumidity");
}

void OutputFormat(SensorValues* sensorValues) {
  Serial.print(sensorValues->Gas, DEC);

  Serial.print(", ");
  Serial.print(sensorValues->Alcohol, DEC);

  Serial.print(", ");
  Serial.print(sensorValues->Sound, DEC);
  
  Serial.print(", ");
  Serial.print(sensorValues->Photocell, DEC);

  Serial.print(", ");
  Serial.print(sensorValues->Flame);

  Serial.print(", ");
  Serial.print(sensorValues->Vibration);

  Serial.print(", ");
  Serial.print(sensorValues->Pir);

  Serial.print(", ");
  Serial.print(sensorValues->DhtValues.Temperature, DEC);
  Serial.print(", ");
  Serial.print(sensorValues->DhtValues.Humidity, DEC);
  
  Serial.println();
}
