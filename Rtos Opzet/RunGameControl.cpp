#include "hwlib.hpp"
#include "rtos.hpp"

enum eButtonID = {instellingenKnopID, minKnopID, plusKnopID, okKnopID};

class RunGameControl : public rtos::task<>{
	enum state_t = {idle, saveParameters, start_game, run_game, gameOver,hit_received, weaponButtonPressed, shoot, reload, sendDataToComputer};

	private:
		state_t state = idle;
		rtos::channel sendHitChannel;
		rtos::flag buttonFlag;
		rtos::pool buttonIDPool;
		rtos::channel sendGameParametersChannel;
		rtos::flag flagGameOver;
		int gamemode = 0;
		int gameTime = 0;
		int timeUntilStart = 0;

	RunGameControl():
	{}

	public:
		void buttonPressed(eButtonID buttonID){buttonIDPool.write(buttonID); buttonFlag.set();}
		void gameOver(){flagGameOver.set();}

	private:
		void main(){
			for(;;){
				switch(state)
				{
					case idle:
						//entry events
						auto events = wait(sendGameParametersChannel);
						if (events == sendGameParametersChannel){
							events = saveParameters;
						}
					break;
					case saveParameters:
						//entry events
						auto events = wait();
				}
			}
		}

};