#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// IrReceiver CLASS
/// \details
/// This class is be used to read out the ir receiver. 
/// This class uses rtos::task<>. 
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