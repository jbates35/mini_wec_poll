#ifndef _MICSENSOR
#define _MICSENSOR


class MicSensor {

  public:
    MicSensor(int a_pin, int d_pin);

    ~MicSensor();

    float readVoltage();

    bool readExtreme();

    int analog_pin;
    int digital_pin;

  private:


};

#endif