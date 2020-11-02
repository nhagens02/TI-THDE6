#ifndef SENDIRMESSAGECONTROL_HPP
#define SENDIRMESSAGECONTROL_HPP

#include "../calvin/IRLed.hpp"

class SendIRMessageControl {
private:
	IRLed irLed;
	void sendZero(){
		irLed.turnOn();

		hwlib::wait_us(800);

		irLed.turnOff();

		hwlib::wait_us(1600);
	}
	void sendOne() {
		irLed.turnOn();

		hwlib::wait_us(1600);

		irLed.turnOff();

		hwlib::wait_us(800);
	}
public:
	SendIRMessageControl(hwlib::pin_out& ledPin) :
		irLed(ledPin)
	{}

	void sendBytes(uint16_t information) {
		for (int i = 16; i > 0; i--) {
			if (information & (1 << (i-1))) sendOne(); else { sendZero(); }
		}
	}
};

#endif // SENDIRMESSAGECONTROL_HPP
