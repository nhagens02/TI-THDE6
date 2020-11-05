#ifndef RECEIVEIRBYTETODATACONTROL_HPP
#define RECEIVEIRBYTETODATACONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "bitDetector.hpp"
#include "StructData.hpp"
#include "RunGameControl.hpp"
#include "RegisterGameParametersControl.hpp"
/// @file


/// \brief
/// gameModes enum
/// \details
/// This enum contains the gamemodes in the game.
/// This class uses rtos::task<>. 
enum gameModes {
	regular = 0, goldenGun = 1
};


/// \brief
/// GameTime options
/// \details
/// This enum contains all possible times for a game of lazergun.
enum gameTimes {
	veryShortTime = 0, shortTime = 1, mediumTime = 2, longTime = 3, veryLongTime = 4, testing = 7
};


/// \brief
/// ReceiveIRByteToDataControl CLASS
/// \details
/// This class is used to convert the 16 bits to data to trigger functions in the game. 
/// If the class received game parameters the class will trigger the function the RegisterGameParametersControl class to send the game parameters.
/// If the class received a shot. The runGameControl class wil be trigged with the playerData and the weaponstrenght. 
/// This class uses rtos::task<>. 
class ReceiveIrByteToDataControl : public rtos::task<> {
	enum state_t {idle, decodeData};

	private:
		state_t state = idle;
		rtos::channel <int, 128> messageChannel;
		struct parameters para;
		struct shootdata sData;
		RegisterGameParametersControl& registerGameParametersControl;
		RunGameControl& runGameControl;
		uint_fast8_t gamemode = 0;
		uint_fast8_t gametime = 0;
		uint_fast16_t previousMessage = 0;
		uint16_t message;
		bool previousTransmitWasFirstDataTransmit = 0;

	public:
		ReceiveIrByteToDataControl(RegisterGameParametersControl& registerGameParametersControl, RunGameControl& runGameControl):
			task("receiveirbyte"),
			messageChannel(this, "Message Channel"),
			registerGameParametersControl ( registerGameParametersControl ),
			runGameControl ( runGameControl )

		{}

		void getMessage(uint16_t message) { messageChannel.write(message); }
		void receiveMessage(uint_fast16_t message) {
			if (message != previousMessage) {
				//decode
				if ((message >> 15) == 1) {
					message -= 32768; // set first bit to 0

					//Check exclusiveOr
					uint_fast8_t player = message >> 10;
					uint_fast8_t data = (message - ((player << 10) >> 5));
					uint_fast8_t exclusiveOr = (message - (message - (player << 10)) - (message - (data << 5)));
					if ((player || data) == exclusiveOr) {


						if ((message >> 10) == 0) { // player = 0
							// decode data
							if (previousTransmitWasFirstDataTransmit == 0) {
								// decode gamemode and gametime
								previousTransmitWasFirstDataTransmit = 1;
								gamemode = message >> 8;
								message -= (gamemode << 8);
								gametime = message >> 5;
								message -= (gametime << 5);
								para.gameMode = gamemode;
								para.gameTime = gametime;
								hwlib::cout << "gameTime : " << para.gameTime << hwlib::endl;
								hwlib::cout << "gameMode : " << para.gameMode << hwlib::endl;
								//here

							}
							else {
								// decode timeUntilStart and call function
								uint_fast8_t timeUntilStart = message >> 5;
								previousTransmitWasFirstDataTransmit = 0;
								para.timeUntilStart = timeUntilStart;
								//registerGameParametersControl.SetParameters(para);
								hwlib::cout << "timeUn: " << para.timeUntilStart << hwlib::endl;
								//here
								runGamecontrol.sendGameParameters(para);
							}
						}
						else {
							// decode hit
							uint_fast8_t player = message >> 10;
							message -= (player << 10);
							uint_fast8_t weaponStrength = message >> 5;
							sData.playerID = player;
							sData.weaponStrength = weaponStrength;
							//runGameControl.sendHit(sData);
							//here
							
						}
					}
				}
			}
			previousMessage = message;
			//change state
		}

	private:

		void main() {

			for (;;) {
				switch (state)
				{
					case idle:
						//entry events

						//other events
						wait(messageChannel);
						message = messageChannel.read();
						state = decodeData;
						break;

					case decodeData:
						//entry events
						
						receiveMessage(message);
						//other events
						state = idle;
						break;

					default:break;
				}
			}
		}

};

#endif //RECEIVEIRBYTETODATACONTROL_HPP