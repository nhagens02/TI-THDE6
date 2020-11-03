#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendIrMessageControl.cpp"
/// @file


/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// All variables are of the type int. 
struct parameters {
	int gameMode;
	int gameTime;
	int timeUntilStart;
};


/// \brief
/// Shoot data struct
/// \details
/// This struct contains a playerID and weaponstrenght. 
/// This struct will be used to send data of a shot. 
struct shootdata {
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
		rtos::channel< struct shootdata, 1024 > triggerChannel; 
		struct shootdata sData;
		int test;
		int test2;
		struct parameters gamePara;


	

	public:
		//mayby by private
		DataToIrbyteControl(hwlib::pin_out& ledpin):
			task("dataToIRByteControl"),
			sendIrMessageControl(ledpin),
			gameParametersChannel(this, "gameParametersChannel"),
			triggerChannel(this, "triggerChannel")
			
		{}
		void sendingGameParametersfun(struct parameters para){gameParametersChannel.write(para);}
		void sendTriggerfun(struct shootdata trigger){triggerChannel.write(trigger);}

		void sendGameParameters(int gamemodeID, int gameTime, int timeUntilStart) {
			//Gamemode and Gametime
			uint16_t information = 0;
			if ((gamemodeID > 1) || (gameTime > 7) || (timeUntilStart > 31))return;
			information += 1 << 15;
			information += gamemodeID << 7;
			information += gameTime << 5;
			//XOR
			for (int i = 5; i > 0; i--) {
				bool x = 0;
				bool y = 0;
				if (information & (1 << (i + 9)))x = 1;
				if (information & (1 << (i + 4)))y = 1;
				information += (x ^ y) << (i - 1);
			}
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);

			//Time until start

			information = 0;
			if ((gamemodeID > 1) || (gameTime > 7) || (timeUntilStart > 31))return;
			information += 1 << 15;
			information += timeUntilStart << 5;
			//XOR
			for (int i = 5; i > 0; i--) {
				bool x = 0;
				bool y = 0;
				if (information & (1 << (i + 9)))x = 1;
				if (information & (1 << (i + 4)))y = 1;
				information += (x ^ y) << (i - 1);
			}
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);
		}
		void sendTrigger(int playerID, int weaponStrength) {
			//hwlib::cout << "test function" << hwlib::endl;
			uint16_t information = 0;
			if ((playerID > 31) || (weaponStrength > 31))return;
			information += 1 << 15;
			information += playerID << 10;
			information += weaponStrength << 5;
			//XOR
			for (int i = 5; i > 0; i--) {
				bool x = 0;
				bool y = 0;
				if (information & (1 << (i + 9)))x = 1;
				if (information & (1 << (i + 4)))y = 1;
				information += (x ^ y) << (i - 1);
			}
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_ms(3);
		}

	private:
		void main(){

			for(;;){
				switch(state)
				{
				case idle: {
					//hwlib::cout << "before wait" << hwlib::endl; //entry events//other events
					auto event = wait(triggerChannel + gameParametersChannel); //
					//hwlib::cout << "after wait" << hwlib::endl;
					if (event == triggerChannel) {
						//hwlib::cout << "in if" << hwlib::endl;
						sData = triggerChannel.read();
						//hwlib::cout << sData.playerID << hwlib::endl;
						//hwlib::cout << sData.weaponstrenght << hwlib::endl;
						state = sendingTrigger;
						break;
					}
					if (event == gameParametersChannel) {
						gamePara = gameParametersChannel.read();
						state = sendingGameParameters;
						break;
					}
					else {
						state = idle;
						break;
					}
					break;
				}
				
				case sendingTrigger:
					//entry events
					//hwlib::cout << "trigger" << hwlib::endl;
					//
					sendTrigger(sData.playerID, sData.weaponstrenght);
					//other events
					//hwlib::cout << "aftertrigger" << hwlib::endl;
					state = idle;
					break;
				case sendingGameParameters:
					//entry events
					
					sendGameParameters(gamePara.gameMode, gamePara.gameTime, gamePara.timeUntilStart);
					//other events
					state = idle;
					break;
				default:break;
				}
			}
		}

};