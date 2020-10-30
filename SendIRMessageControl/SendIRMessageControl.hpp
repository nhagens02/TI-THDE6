#ifndef SENDIRMESSAGECONTROL_HPP
#define SENDIRMESSAGECONTROL_HPP

#include "../calvin/IRLed.hpp"

class SendIRMessageControl {
private:
	IRLed irLed;
	void sendZero(){
		irLed.turnOn();
		uint_fast32_t currentTime = hwlib::now_us();
		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
		}

		irLed.turnOff();

		currentTime = hwlib::now_us();
		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
		}
	}
	void sendOne() {
		irLed.turnOn();

		uint_fast32_t currentTime = hwlib::now_us();
		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
		}

		irLed.turnOff();

		currentTime = hwlib::now_us();
		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
		}
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
