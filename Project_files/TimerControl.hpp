#ifndef TIMERCONTROL_HPP
#define TIMERCONTROL_HPP


#include "StructData.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// TimerControl CLASS
/// \details
/// This class manages the timer for the game and the timer how long before the game start.
/// This class uses the struct: "timer_struct". If the bool runGameTime from the timer_struct is true. The time in the struct will contain the lenght of the game. 
/// If the bool runGameTime is false. The time in the struct will contain in how many minutes the game will start.
/// This class uses rtos::task<>.
class TimerControl : public rtos::task<>{
	enum state_t {idle, timeUntilStartState, gameTimerState};

	public:
		TimerControl(RunGameControl& runGameControl) :
			task("Timer Controller"),
			setTimerFlag(this, "flag for timer set"),
			setTimerPool("pool with struct: timer_struct"),
			untilGameTimer(this, "game finished timer"),
			gameTimer(this, "Game Timer"),
			runGameControl(runGameControl)
		{
			this->state = this->idle;
		}

		void setTimer(struct parameters timerData)
		{
			setTimerPool.write(timerData);
			setTimerFlag.set();
		}

	private:
		state_t state;
		rtos::flag setTimerFlag;
		rtos::pool<struct parameters> setTimerPool;
		rtos::timer untilGameTimer;
		rtos::timer gameTimer;
		struct parameters timerData;
		RunGameControl& runGameControl;

		void main() {
			for(;;)
			{
				switch(this->state)
				{
					case this->idle:
						wait(this->setTimerFlag);

						this->timerData = this->setTimerPool.read();
						this->state = timeUntilStartState;
						break;
					case this->timeUntilStartState:
						this->untilGameTimer.set((4000 * this->timerData.timeUntilStart) * rtos::ms); //example if timeUntil Start = 2, 2*2000 = 4 sec wait time.

						wait(this->untilGameTimer);

						this->runGameControl.startGame();
						this->state = this->gameTimerState;

						break;
					case this->gameTimerState:
						if(this->timerData.gameTime == 0){ this->gameTimer.set(60000 * rtos::ms); }
						else if (this->timerData.gameTime == 1) { this->gameTimer.set(120000 * rtos::ms); }
						else if (this->timerData.gameTime == 2) { this->gameTimer.set(240000 * rtos::ms); }
						else if (this->timerData.gameTime == 3) { this->gameTimer.set(360000 * rtos::ms); }
						else if (this->timerData.gameTime == 4) { this->gameTimer.set(480000 * rtos::ms); }
						else if (this->timerData.gameTime == 5) { this->gameTimer.set(600000 * rtos::ms); }
						else if (this->timerData.gameTime == 6) { this->gameTimer.set(720000 * rtos::ms); }
						else if (this->timerData.gameTime == 7) { this->gameTimer.set(840000 * rtos::ms); }

						wait(this->gameTimer);
						
						this->runGameControl.gameOver();

						this->state = this->idle;
						break;
					default:break;
				}
			}
		}
};

#endif // TIMERCONTROL_HPP
