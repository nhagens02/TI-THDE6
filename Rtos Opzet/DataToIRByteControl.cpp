#include "hwlib.hpp"
#include "rtos.hpp"

struct parameters{
	int gameMode,
	int gameTime,
	int timeUntilStart;
};

struct shootdata{
	int playerID,
	int weaponstrenght;
};


class DataToIRbyteControl : public rtos::task<>{
	enum state_t = {idle, sendingTrigger, sendingGameParameters};

	private:
		state_t state = idle;
		sendIrMessageControl sendIrMessageControl;
		rtos::channel gameParametersChannel;
		rtos::channel triggerChannel;


	DataToIRbyteControl():
	{}

	public:
		void sendingGameParameters(struct parameters para){gameParametersChannel.write(para);}
		void sendTrigger(struct shootdata trigger){triggerChannel.write(trigger);}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events

						//other events
						auto events = wait(triggerChannel,gameParametersChannel);
						if (events == triggerChannel) {
							struct shootdata sData = gameParametersChannel.read();
							state = sendingTrigger;
						}
						if (events == gameParametersChannel) {
							struct parameters gamePara = gameParametersChannel.read();
							state = sendingGameParameters;
						}
						break;

					case sendingTrigger:
						//entry events
						sendIrMessageControl.sendTrigger(sData.playerID, sData.weaponstrenght);

						//other events
						state = idle;

						break;

					case sendingGameParameters:
						//entry events
						sendIrMessageControl.sendIrMessageControl(gamePara.gameMode, gamePara.gameTime, gamePara.timeUntilStart);

						//other events
						state = idle;

						break;

					default:break;
				}
			}
		}

};