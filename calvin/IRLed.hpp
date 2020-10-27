#include "hwlib.hpp"

#ifndef IRLED_HPP
#define IRLED_HPP

class IRLed {
private:
  hwlib::pin_in ledPin;

public:
  IRLed(hwlib::pin_in ledPin):
    ledPin(ledPin)
    {}

    void turnOn(){
      ledPin.write(1);
    }
    void turnOff(){
      ledPin.write(0)
    }

};

#endif
