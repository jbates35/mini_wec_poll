#include "Arduino.h"
#include "MicSensor.h"

MicSensor::MicSensor(int a_pin, int d_pin)
{
  analog_pin = a_pin;
  digital_pin = d_pin;

  pinMode(analog_pin, INPUT);
  pinMode(digital_pin, INPUT);

}


MicSensor::~MicSensor()
{
}


float MicSensor::readVoltage()
{
  return analogRead(analog_pin) * (5.0 / 1023.0);
}

bool MicSensor::readExtreme()
{
  return digitalRead(digital_pin);
}