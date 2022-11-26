#ifndef _DOTMATRIX
#define _DOTMATRIX


#include <MD_MAX72xx.h>
#include "MD_Parola.h"
#include <SPI.h>

class DotMatrix {

  public:
    DotMatrix();

    ~DotMatrix();

    void print_to_matrix(String x);

  private:
    

};



#endif