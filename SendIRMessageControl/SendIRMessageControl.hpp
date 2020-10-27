#ifndef SENDIRMESSAGECONTROL_HPP
#define SENDIRMESSAGECONTROL_HPP

#include "../calvin/IRLed.hpp"

class SendIRMessageControl {
private:
	IRLed irLed;
	void sendZero(){
		//send 0
	}
	void sendOne() {
		//send 1
	}
public:
	SendIRMessageControl(hwlib::pin_out& ledPin) :
		irLed(ledPin)
	{}

	void sendBytes(uint16_t information) {
		for (unsigned int i = 0; i < 16; i++) {
			if (information & (1 << (16 - i))) sendOne(); else { sendZero(); }
		}
	}
};

#endif // SENDIRMESSAGECONTROL_HPP
