#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP
#include  "hwlib.hpp"

class IrReceiver {
private:
	hwlib::pin_in& receiverPin;
public:
	IrReceiver(hwlib::pin_in& receiverPin) :
		receiverPin(receiverPin)
	{}
	bool getCurrentSignal() {
		receiverPin.refresh();
		bool currentSignalInverse = receiverPin.read();
		return !currentSignalInverse;
	}
};

#endif // IRRECEIVER_HPP