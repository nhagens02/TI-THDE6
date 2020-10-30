#include "hwlib.hpp"
#include "rtos.hpp"

struct timer_struct timer_s{
	int time;
	bool runGameTime;
};

class TimerControl : public rtos::task<>{
	enum state state_t = {idle, timer};

	private:
		state_t state = idle;
		rtos::timer setTimer;
		rtos::flag setTimerFlag;
		rtos::pool setTimerPool;
		rtos::timer endTimerFlag;


	TimerControl():

	{}

	public:
		void setTimer(struct timer_struct timer_s) {setTimerPool.write(timer_s); flagSetTimer.set();}
		void endTimer() {endTimerFlag.set();}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events


						//other events
						wait(flagSetTimer);
						struct timer_struct timerData = setTimerPool.read();
						if (timerData.runGameTime == false) {
							untilGameTimer.set(timerData.time);
							state = timer;
						}
						else if (timerData.runGameTime == true) {
							startGameTimer.set(timerData.time);
							state = timer;
						}
						break;

					case timer:
						//entry events
						DisplayControl.showMessage(TimerControl.timeremaining());

						//other events
						events = wait(untilGameTimerFlag, startGameTimerFlag);
						if (events == untilGameTimerFlag) {
							endTimerFlag.set();
							state = idle;
						}
						if (events == startGameTimerFlag) {
							runGameControl.gameOver();
							state = idle;
						}
						break;
					default:break;

				}
			}
		}
};