#ifndef DATATOIRBYTECONTROL_HPP
#define DATATOIRBYTECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "SendIrMessageControl.hpp"
#include "StructData.hpp"
/// @file


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
		rtos::channel< struct parameters, 128 > gameParametersChannel; 
		rtos::channel< struct shootdata, 128 > triggerChannel; 
		struct shootdata sData;
		struct parameters gamePara;

	public:
		DataToIrbyteControl(hwlib::pin_out& ledpin):
			task(3, "dataToIRByteControl"),
			sendIrMessageControl(ledpin),
			gameParametersChannel(this, "gameParametersChannel"),
			triggerChannel(this, "triggerChannel")
			
		{}
		void sendingGameParametersChannel(struct parameters para){gameParametersChannel.write(para);}
		void sendTriggerChannel(struct shootdata trigger){triggerChannel.write(trigger);}

		void sendGameParameters(int gamemodeID, int gameTime, int timeUntilStart) {
			//Gamemode and Gametime
			uint_fast16_t information = 0;
			if ((gamemodeID > 1) || (gameTime > 7) || (timeUntilStart > 31))return;
			information += 1 << 15;
			information += gamemodeID << 8;
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
			hwlib::wait_us_busy(3000);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_us_busy(4200);
			

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

			hwlib::wait_us_busy(3000);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_us_busy(4200);
		}

		void sendTrigger(int playerID, int weaponStrength) {
			uint_fast16_t information = 0;
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
			hwlib::wait_us_busy(3000);
			sendIrMessageControl.sendBytes(information);
			hwlib::wait_us_busy(4200);
		}

	private:
		void main(){

			for(;;){
				switch(state)
				{
				case idle: {
					auto event = wait(triggerChannel + gameParametersChannel); 
					if (event == triggerChannel) {
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
					sData = triggerChannel.read();
					sendTrigger(sData.playerID, sData.weaponStrength);
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

#endif // DATATOIRBYTECONTROL_HPP