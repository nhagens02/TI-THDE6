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
	enum state_t {idle, timeUntilStartState ,gameTimerState};

	private:
		state_t state = idle;
		//rtos::timer set_Timer;
		rtos::flag setTimerFlag;
		rtos::pool<struct parameters> setTimerPool;
		//rtos::flag endTimerFlag;
		rtos::timer untilGameTimer;
		rtos::timer startGameTimer;
		struct parameters timerData;
		RunGameControl& runGameControl;


	public:
		TimerControl(RunGameControl& runGameControl) :
			task("Timer Controller"),
			//set_Timer(this, "timer"),
			setTimerFlag(this, "Flag for timer set"),
			setTimerPool("pool with struct: timer_struct"),
			//endTimerFlag(this, "end timer flag"),
			untilGameTimer(this, "Game finished timer"),
			startGameTimer(this, "Start Game Timer"),
			runGameControl(runGameControl)
		{}

		void setTimer(struct parameters timerData) {setTimerPool.write(timerData); setTimerFlag.set();}
		//void endTimer() {endTimerFlag.set();}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						hwlib::cout << "waiting for game data. . .\n";
						wait(this->setTimerFlag);

						timerData = setTimerPool.read();
						state = timeUntilStartState;
						
						break;

					case timeUntilStartState:
						untilGameTimer.set((2000 * timerData.timeUntilStart) * rtos::ms); //example if timeUntil Start = 2, 2*2000 = 4 sec wait time.

						hwlib::cout << "waiting for game countdown to start. . .\n";
						wait(this->untilGameTimer);

						this->runGameControl.StartGame();
						this->state = gameTimerState;

						break;

					case gameTimerState:
						this->startGameTimer.set( (/* here must calulation for timer*/ 1 ) * rtos::ms);

						hwlib::cout << "waiting for game to end. . .\n";
						wait(startGameTimer);

						state = idle;
						break;
					
					default:break;
					/*
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
					*/
					//default:break;
				}
			}
		}
};