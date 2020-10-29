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
						intervalTimer.set(800);

						//other events
						wait(intervalTimer);
						bool signal = IrReceiver.getCurrentSignal();
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