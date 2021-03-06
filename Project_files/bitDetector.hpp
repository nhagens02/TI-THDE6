#ifndef BITDETECTOR_HPP
#define BITDETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "IrReceiver.hpp"
#include "ReceiveIrMessageControl.hpp"
/// @file


/// \brief
/// bitDetector CLASS
/// \details
/// This class will detect using the boundary object:"Ir Receiver".
/// This class poll every 100 microseconds if ther is a change in signal.
/// This class uses the ir protocol from the THDE casus. 
/// This class uses rtos::task<>. 
class BitDetector : public rtos::task<> {
	enum state_t {idle, startReceiving};

	private:
		state_t state = idle;
		IrReceiver irReceiver;
		ReceiveIrMessageControl& receiveIrMessageControl;
		rtos::clock intervalHunderdSignalCheck;
		bool bitValue;

	public:
		BitDetector(hwlib::pin_in& IrReceiverPin, ReceiveIrMessageControl& receiveIrMessageControl):
			task(0, "bit detector signal"),
			irReceiver ( IrReceiverPin ),
			receiveIrMessageControl( receiveIrMessageControl ),
			intervalHunderdSignalCheck(this, (100 * rtos::us), "interval checker")
		{}

	private:
		void main(){
			for(;;){
				switch(state)
				{
					case idle: {
						wait(intervalHunderdSignalCheck);
						bool signal = irReceiver.getCurrentSignal();
						if (signal == 1) {
							state = startReceiving;
							break;
						}
						else {
							state = idle;
							break;
						}
						break;
					}
					case startReceiving:
						//entry events
						hwlib::wait_us_busy(1200);
						bitValue = irReceiver.getCurrentSignal();
						while (irReceiver.getCurrentSignal()) { /*hwlib::wait_us(0);*/  }
						receiveIrMessageControl.sendBit(bitValue);		
						//other events
						state = idle;
						break;
					default:break;
				}
			}
		}
};
#endif // BITDETECTOR_HPP