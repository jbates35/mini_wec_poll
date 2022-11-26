
#include "DotMatrix.h"

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_Parola _display(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

DotMatrix::DotMatrix()
{
  _display.begin();
}

DotMatrix::~DotMatrix()
{

}

void DotMatrix::print_to_matrix(String x)
{
  _display.displayClear();
  _display.print(x);

}