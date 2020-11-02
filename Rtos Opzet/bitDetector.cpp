#include "hwlib.hpp"
#include "rtos.hpp"
//#include "IrReceiver.cpp"
/// @file


/// \brief
/// bitDetector CLASS
/// \details
/// This class will detect using the boundary object:"Ir Receiver".
/// This class poll every 800 microseconds if ther is a change in signal.
/// This class uses the ir protocol from the THDE casus. 
/// This class uses rtos::task<>. 
class bitDetector : public rtos::task<>{
	enum state_t {idle, startReceiving};

	private:
		state_t state = idle;
		//IrReceiver & IrReceiverLed;
		//hwlib::target::pin_in(hwlib::target::pins::d6) irReceiverLed;
		//s16BitConverter& s16BitConverter;
		rtos::timer intervalTimer;
		rtos::timer afterBitTimer;
	bitDetector()//:
		//IrReceiverLed ( irReceiverLed )
	{}

	public:


	private:
		void main(){


			for(;;){
				switch(state)
				{
					case idle:
						//entry events
						intervalTimer.set(800);

						//other events
						wait(intervalTimer);
						bool signal = IrReceiver.get();
						if (signal == 1){
							int timing_low = 0;
							int timing_high = 0;
							afterBitTimer.set(2400);
							state = startReceiving;
						}
						else {
							state = idle;
						}
						break;


					case startReceiving:
						//entry events
						intervalTimer.set(800);

						//other events
						auto event = wait(intervalTimer, afterBitTimer);
						if (event == intervalTimer) {
							bool signal = IrReceiver.getCurrentSignal();
							if (signal == 1) {
								timing_high++;
								state = startReceiving;
							}
							else if (signal == 0) {
								timing_low++;
								state = startReceiving;
							}
						}
						if (event == afterBitTimer) {
							if (timing_high == 1) && (timing_low == 2) {
								16BitConverter.sendBit(0);
								state = idle;
							}
							else if (timing_high == 2) && (timing_low == 1) {
								16BitConverter.sendBit(1);
								state = idle;
							}
							else {
								state = idle;
							}
						}
						break;
					default:break;
				}



			}

		}


};