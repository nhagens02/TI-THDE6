#ifndef TIMERCONTROL_HPP
#define TIMERCONTROL_HPP

#include "StructData.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
/// @file

enum state_t {idle, timeUntilStartState ,gameTimerState};

/// \brief
/// TimerControl CLASS
/// \details
/// This class manages the timer for the game and the timer how long before the game start.
/// This class uses the struct: "timer_struct". If the bool runGameTime from the timer_struct is true. The time in the struct will contain the lenght of the game. 
/// If the bool runGameTime is false. The time in the struct will contain in how many minutes the game will start.
/// This class uses rtos::task<>.
class TimerControl : public rtos::task<>{
	public:
		TimerControl(RunGameControl& runGameControl) :
			task("Timer Controller"),
			setTimerFlag(this, "flag for timer set"),
			setTimerPool("pool with struct: timer_struct"),
			untilGameTimer(this, "game finished timer"),
			startGameTimer(this, "start Game Timer"),
			runGameControl(runGameControl)
		{}
		void setTimer(struct parameters timerData) {setTimerPool.write(timerData); setTimerFlag.set();}

	private:
		state_t state = idle;
		rtos::flag setTimerFlag;
		rtos::pool<struct parameters> setTimerPool;
		rtos::timer untilGameTimer;
		rtos::timer startGameTimer;
		struct parameters timerData;
		RunGameControl& runGameControl;

		void main() {
			for(;;) {
				switch(this->state) {
					case idle:
						hwlib::cout << "waiting for game data. . .\n";
						wait(this->setTimerFlag);

						this->timerData = this->setTimerPool.read();
						this->state = timeUntilStartState;
						
						break;
					case timeUntilStartState:
						this->untilGameTimer.set((2000 * timerData.timeUntilStart) * rtos::ms); //example if timeUntil Start = 2, 2*2000 = 4 sec wait time.

						hwlib::cout << "waiting for game countdown to start. . .\n";
						wait(this->untilGameTimer);

						this->state = gameTimerState;

						break;
					case gameTimerState:
						this->startGameTimer.set(1000*rtos::ms);

						hwlib::cout << "waiting for game to start. . .\n";
						wait(this->startGameTimer);

						hwlib::cout << "game started!\n";
						this->runGameControl.StartGame();
						this->suspend();

						break;
					default:break;
				}
			}
		}
};

#endif
