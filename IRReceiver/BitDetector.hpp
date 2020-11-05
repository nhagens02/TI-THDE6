#ifndef BITDETECTOR_HPP
#define BITDETECTOR_HPP

#include "IrReceiver.hpp"
#include "ReceiveIrMessageControl.hpp"

class BitDetector {
private:
	IrReceiver irReceiver;
	ReceiveIrMessageControl& receiveIrMessageControl;
public:
	BitDetector(IrReceiver IrReceiver, ReceiveIrMessageControl receiveIrMessageControl) :
		irReceiver(irReceiver), receiveIrMessageControl(receiveIrMessageControl)
	{}
	// poll the IrReceiver and send the results to the ReceiveIrMessagecontrol receiveBit function
};

#endif // BITDETECTOR_HPP

//polling code
/*
wait(intervalTimer);
irReceiver.refresh();
bool currentSignal = irReceiver.read();
if (currentSignal == 1) {
	hwlib::wait_us(1200);
	bool bitValue = tsop_signal.read();
	while (tsop_signal.read()) { hwlib::wait_us(0); }
	bitValueChannel.write(bitValue);
}
state = idle;
*/