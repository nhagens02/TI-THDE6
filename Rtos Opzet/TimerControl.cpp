#include "hwlib.hpp"
#include "rtos.hpp"
/// @file

/// \brief
/// Timer Struct
/// \details
/// Struct used to save data about the timer used.
/// Option 1: If runGameTime is true, the time in timer used is the time how long the game is.
/// Option 2: If runGameTime is false, the time in timer used is not the time how long the game is.
/// the time will uses minutes.
struct timer_struct timer_s{
	int time;
	bool runGameTime;
};


/// \brief
/// TimerControl CLASS
/// \details
/// This class manages the timer for the game and the timer how long before the game start.
/// This class uses the struct: "timer_struct". If the bool runGameTime from the timer_struct is true. The time in the struct will contain the lenght of the game. 
/// If the bool runGameTime is false. The time in the struct will contain in how many minutes the game will start.
/// This class uses rtos::task<>.
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