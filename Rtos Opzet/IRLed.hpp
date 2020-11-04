#ifndef IRLED_HPP
#define IRLED_HPP

#include "hwlib.hpp"
#include "rtos.hpp"


class IRLed {
private:
	hwlib::pin_out& ledPin;

public:
	IRLed(hwlib::pin_out& ledPin):
		ledPin(ledPin)
    {}

    void turnOn(){
		ledPin.write(1);
		ledPin.flush();
    }
    void turnOff(){
		ledPin.write(0);
		ledPin.flush();
    }

};

#endif // IRLED_HPP
