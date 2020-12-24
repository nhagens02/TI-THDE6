#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
/// @file


/// \brief
/// TimerControl CLASS
/// \details
/// This class manages the timer for the game and the timer how long before the game start.
/// This class uses the struct: "timer_struct". If the bool runGameTime from the timer_struct is true. The time in the struct will contain the lenght of the game. 
/// If the bool runGameTime is false. The time in the struct will contain in how many minutes the game will start.
/// This class uses rtos::task<>.
class TimerControl : public rtos::task<>{
	enum state_t {idle, timer};

	private:
		state_t state = idle;
		rtos::timer set_Timer;
		rtos::flag setTimerFlag;
		rtos::pool<struct timer_struct> setTimerPool;
		rtos::flag endTimerFlag;
		rtos::timer untilGameTimer;
		rtos::timer startGameTimer;
		struct timer_struct timerData;


	TimerControl():
		task("Timer Controller"),
		set_Timer(this, "timer"),
		setTimerFlag(this, "Flag for timer set"),
		setTimerPool("pool with struct: timer_struct"),
		endTimerFlag(this, "end timer flag"),
		untilGameTimer(this, "Game finished timer"),
		startGameTimer(this, "Start Game Timer")
	{}

	public:
		void setTimer(struct timer_struct timer_s) {setTimerPool.write(timer_s); setTimerFlag.set();}
		void endTimer() {endTimerFlag.set();}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events


						//other events
						wait(setTimerFlag);
						timerData = setTimerPool.read();
						if (timerData.runGameTime == false) {
							untilGameTimer.set(timerData.time);
							state = timer;
							break;
						}
						else if (timerData.runGameTime == true) {
							startGameTimer.set(timerData.time);
							state = timer;
							break;
						}
						state = idle;
						break;

					case timer:
						//entry events
						//DisplayControl.showMessage(TimerControl.timeremaining());

						//other events
						auto events = wait(untilGameTimer + startGameTimer);
						if (events == untilGameTimer) {
							endTimerFlag.set();
							state = idle;
							break;
						}
						if (events == startGameTimer) {
							//runGameControl.gameOver();
							state = idle;
							break;
						}
						state = idle;
						break;

					//default:break;

				}
			}
		}
};