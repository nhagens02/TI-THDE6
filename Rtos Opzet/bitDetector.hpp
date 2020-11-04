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
/// This class poll every 800 microseconds if ther is a change in signal.
/// This class uses the ir protocol from the THDE casus. 
/// This class uses rtos::task<>. 
class BitDetector : public rtos::task<> {
	enum state_t {idle, startReceiving};

	private:
		state_t state = idle;
		IrReceiver irReceiver;
		ReceiveIrMessageControl& receiveIrMessageControl;
		//hwlib::target::pin_in(hwlib::target::pins::d6) irReceiverLed;
		//s16BitConverter& s16BitConverter;
		rtos::clock intervalHunderdSignalCheck;
		//rtos::timer afterBitTimer;
	

	public:
		BitDetector(hwlib::pin_in& IrReceiverPin, ReceiveIrMessageControl& receiveIrMessageControl;):
			task("bit detector signal"),
			irReceiver ( IrReceiverPin ),
			receiveIrMessageControl( receiveIrMessageControl ),
			intervalHunderdSignalCheck(this, (200 * rtos::ms), "keypad interval checker")
		{}


	private:
		void main(){
			for(;;){
				switch(state)
				{
					case idle:
						//entry events

						//other events
						wait(intervalHunderdSignalCheck);
						bool signal = IrReceiver.getCurrentSignal();
						if (signal == 1){
							state = startReceiving;
							break;
						}
						else {
							state = idle;
							break;
						}
						break;


					case startReceiving:
						//entry events
						hwlib::wait_us(1200);
						bool bitValue = IrReceiver.getCurrentSignal();
						while (IrReceiver.getCurrentSignal()) { hwlib::wait_us(0); }
						receiveIrMessageControl.bitValueChannel(bitValue);
						
						//other events
						state = idle;
						break;

					default:break;
				}



			}

		}


};

#endif // BITDETECTOR_HPP