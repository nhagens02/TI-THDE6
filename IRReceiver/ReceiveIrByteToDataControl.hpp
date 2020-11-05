#ifndef RECEIVEIRBYTETODATACONTROL_HPP
#define RECEIVEIRBYTETODATACONTROL_HPP

#include "BitDetector.hpp"
//#include "RunGameControl.hpp"

class ReceiveIrByteToDataControl {
private:
	//RunGameControl runGameControl&;
	//RegisterGameParameters registerGameParameters&;
	uint_fast8_t gamemode = 0;
	uint_fast8_t gametime = 0;
	uint_fast16_t previousMessage = 0;
	bool previousTransmitWasFirstDataTransmit = 0;
public:
	void receiveMessage(uint_fast16_t message) {
		if (message != previousMessage) {
			//decode
			if ((message >> 15) == 1) {
				message -= 32768; // set first bit to 0

				//Check XOR
				uint_fast8_t player = message >> 10;
				uint_fast8_t data = (message - (player << 10) >> 5);
				uint_fast8_t xor = message - (message - (player << 10)) - (message - (data << 5));
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
};

#endif // RECEIVEIRBYTETODATACONTROL_HPP