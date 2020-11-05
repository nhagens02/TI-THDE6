#ifndef RECEIVEIRMESSAGECONTROL_HPP
#define RECEIVEIRMESSAGECONTROL_HPP

#include "BitDetector.hpp"

class ReceiveIrMessageControl {
private:
	ReceiveIrByteToDataControl receiveIrByteToDataControl;
	uint_fast16_t message;
	uint_fast16_t bitAmount;
public:
	ReceiveIrMessageControl()
	{}
	 void receiveBit(bool bit) {
		 message += (bit << (15 - bitAmount));
		 bitAmount++;
		 if (bitAmount == 16) {
			 receiveIrByteToDataControl.receiveMessage(message);
		 }
	}
};

#endif // RECEIVEIRMESSAGECONTROL_HPP