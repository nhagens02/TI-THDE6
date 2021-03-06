#ifndef RECEIVEIRBYTETODATACONTROL_HPP
#define RECEIVEIRBYTETODATACONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "bitDetector.hpp"
#include "StructData.hpp"
#include "RunGameControl.hpp"
#include "RegisterGameParametersControl.hpp"
#include "TimerControl.hpp"
/// @file


/// \brief
/// ReceiveIRByteToDataControl CLASS
/// \details
/// This class is used to convert the 16 bits to data to trigger functions in the game. 
/// If the class received game parameters the class will trigger the function the RegisterGameParametersControl class to send the game parameters.
/// If the class received a shot. The runGameControl class wil be trigged with the playerData and the weaponstrength. 
/// This class uses rtos::task<>. 
class ReceiveIrByteToDataControl : public rtos::task<> {
	enum state_t {idle, decodeData};

	private:
		state_t state = idle;
		rtos::channel <uint16_t, 64> messageChannel;
		parameters para;
		shootdata sData;
		RegisterGameParametersControl& registerGameParametersControl;
		RunGameControl& runGameControl;
		TimerControl& timerControl;
		uint16_t gamemode = 0;
		uint16_t gametime = 0;
		uint16_t previousMessage = 0;
		uint16_t message;
		bool previousTransmitWasFirstDataTransmit = false;

	public:
		ReceiveIrByteToDataControl(RegisterGameParametersControl& registerGameParametersControl, RunGameControl& runGameControl, TimerControl& timerControl):
			task(2,"receiveirbyte"),
			messageChannel(this, "Message Channel"),
			registerGameParametersControl ( registerGameParametersControl ),
			runGameControl ( runGameControl ),
			timerControl ( timerControl )
		{}

		void getMessage(uint16_t message) { messageChannel.write(message); }
		void receiveMessage(uint16_t message) {
			if (message != previousMessage) {
				previousMessage = message;
				//decode
				if ((message >> 15) == 1) {
					message -= 32768; // set first bit to 0

					//Check exclusiveOr
					uint16_t player = message >> 10;
					uint16_t data = (message << 6);
					data = (data >> 11);
						
					uint16_t exclusiveOr = (message << 11);
					exclusiveOr = (exclusiveOr >> 11);
					//calculate xor
					
					uint16_t calculatedXOR = 0;
					for (int i = 5; i > 0; i--) {
						bool x = 0;
						bool y = 0;
						if (player & (1 << (i-1)))x = 1;
						if (data & (1 << (i-1)))y = 1;
						calculatedXOR += (x ^ y) << (i - 1);
					}

					if (calculatedXOR == exclusiveOr) {

						if (player == 0) { // player = 0
							// decode data
							if (!previousTransmitWasFirstDataTransmit) {
								// decode gamemode and gametime
								previousTransmitWasFirstDataTransmit = true;
								gamemode = data >> 3;
								gametime = (data << 13);
								gametime = (gametime >> 13);
								para.gameMode = gamemode;
								para.gameTime = gametime;
							}
							else {
								// decode timeUntilStart and call function
								uint_fast8_t timeUntilStart = message >> 5;
								previousTransmitWasFirstDataTransmit = false;
								para.timeUntilStart = timeUntilStart;
								registerGameParametersControl.SetParameters(para);
								runGameControl.sendGameParameters(para);
								timerControl.setTimer(para);
							}
						}
						else {
							// decode hit
							uint8_t player = message >> 10;
							message -= (player << 10);
							uint8_t weaponStrength = message >> 5;
							sData.playerID = player;
							sData.weaponStrength = weaponStrength;
							runGameControl.sendHit(sData);
						}
					}
				}
			}
			previousMessage = 0;
			//change state
		}

	private:

		void main() {

			for (;;) {
				switch (state)
				{
					case idle:
						wait(messageChannel);
						state = decodeData;
						break;

					case decodeData:
						//entry events
						message = messageChannel.read();
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