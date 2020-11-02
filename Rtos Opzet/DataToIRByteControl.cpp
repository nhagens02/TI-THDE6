#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendIrMessageControl.cpp"
/// @file


/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// All variables are of the type int. 
struct parameters{
	int gameMode;
	int gameTime;
	int timeUntilStart;
};


/// \brief
/// Shoot data struct
/// \details
/// This struct contains a playerID and weaponstrenght. 
/// This struct will be used to send data of a shot. 
struct shootdata{
	int playerID;
	int weaponstrenght;
};


/// \brief
/// DataToIRbyteControl CLASS
/// \details
/// This class will convert the functions: sendTriggers and sendTriggers to 16 bits to be used by the sendIrMessageControl class. 
/// This class uses rtos::task<>. 
class DataToIrbyteControl : public rtos::task<>{
	enum state_t {idle, sendingTrigger, sendingGameParameters};

	private:
		state_t state = idle;
		SendIRMessageControl sendIrMessageControl;
		rtos::channel< struct parameters, 1024 > gameParametersChannel;
		rtos::channel< struct shootdata , 1024 > triggerChannel;
		struct shootdata sData;
		struct parameters gamePara;


	

	public:
		//mayby by private
		DataToIrbyteControl(hwlib::pin_out& ledpin):
			task("dataToIRByteControl"),
			sendIrMessageControl(ledpin),
			gameParametersChannel(this, "gameParametersChannelc"),
			triggerChannel(this, "triggerChannelc")
			
		{}
		//void sendingGameParameters(struct parameters para){gameParametersChannel.write(para);}
		//void sendTrigger(struct shootdata trigger){triggerChannel.write(trigger);}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events

						//other events
						auto events = wait(triggerChannel); //gameParametersChannel
						if (events == triggerChannel) {
							sData = gameParametersChannel.read();
							state = sendingTrigger;
						}
						if (events == gameParametersChannel) {
							gamePara = gameParametersChannel.read();
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