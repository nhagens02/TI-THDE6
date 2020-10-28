#include "hwlib.hpp"
#include "rtos.hpp"

class bitDetector : public rtos::task<>{
	enum state_t = {idle, startReceiving};

	private:
		16BitConverter& 16BitConverter;
		rtos::timer intervalTimer;
		rtos::timer afterBitTimer;
	bitDetector():

	{}

	public:


	private:
		void main(){


			for(;;){
				switch(state)
				{
					case idle:
						//entry events
						intervalTimer.start();

						//other events
						wait(intervalTimerFlag);
						bool signal = IrReceiver.getCurrentSignal();
						if (signal == 1){
							int timing_low = 0;
							int timing_high = 0;
							afterBitTimer.start();
							state = startReceiving;
						}
						else {
							state = idle;
						}
						break;


					case startReceiving:
						//entry events
						intervalTimer.start();

						//other events
						auto event = wait(intervalTimerFlag, afterBitTimer);
						if (event == intervalTimerFlag) {
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
							if (timing_high == 8) && (timing_low == 16) {
								16BitConverter.sendBit(0);
								state = idle;
							}
							else if (timing_high == 16) && (timing_low == 8) {
								16BitConverter.sendBit(1);
								state = idle;
							}
							else {
								state = idle;
							}
						}
						

				}



			}

		}


};