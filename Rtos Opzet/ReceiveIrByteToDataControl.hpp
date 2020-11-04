#ifndef RECEIVEIRBYTETODATACONTROL_HPP
#define RECEIVEIRBYTETODATACONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "bitDetector.hpp"
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
class ReceiveIRByteToDataControl : public rtos::task<> {
	enum state_t {idle, decodeData};

	private:
		state_t state = idle;
		rtos::channel messageChannel;
		RunGameControl& RunGameControl;
		registerGameParameters& registerGameParameters;
		receiveIrMessageControl& receiveIrMessageControl;
		uint_fast8_t gamemode = 0;
		uint_fast8_t gametime = 0;
		uint_fast16_t previousMessage = 0;
		bool previousTransmitWasFirstDataTransmit = 0;
		

	ReceiveIRByteToDataControl():

	{}	

	public:
		void getMessage(uint16_t message) { messageChannel.write(message); }
		void receiveMessage(uint_fast16_t message) {
			if (message != previousMessage) {
				//decode
				if ((message >> 15) == 1) {
					message -= 32768; // set first bit to 0

					//Check XOR
					uint_fast8_t player = message >> 10;
					uint_fast8_t data = (message - (player << 10) >> 5);
					uint_fast8_txor = message - (message - (player << 10)) - (message - (data << 5));
					if ((player || data) == xor) {


						if ((message >> 10) == 0) { // player = 0
							// decode data
							if (previousTransmitWasFirstDataTransmit == 0) {
								// decode gamemode and gametime
								previousTransmitWasFirstDataTransmit = 1;
								gamemode = message >> 8;
								message -= (gamemode << 8);
								gametime = message >> 5;
								message -= (gametime << 5);

							}
							else {
								// decode timeUntilStart and call function
								uint_fast8_t timeUntilStart = message >> 5;
								previousTransmitWasFirstDataTransmit = 0;
							}
						}
						else {
							// decode hit
							uint_fast8_t player = message >> 10;
							message -= (player << 10);
							uint_fast8_t weaponStrength = message >> 5;
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
						uint16_t message = messageChanel.read();
						state = decode;
						break;

					case decodeData:
						//entry events
						decode(message)

						//other events
						state = idle;
						break;

					default:break;
				}
			}
		}

};

#endif //RECEIVEIRBYTETODATACONTROL_HPP