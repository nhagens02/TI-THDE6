#include "hwlib.hpp"
#include "rtos.hpp"
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
		bool getBit(const uint16_t& message, const int& index) {
			int x = 0;
			if (value & (1 << index)) {
				x = 1;
			}
			return x;
		}
		void decode(uint16_t message) {
			if (!( (getBit(message, 15)) && (getBit(message, 14)) && (getBit(message, 13)) && (getBit(message, 12)) && (getBit(message, 11)) )) {
				//It is player 0. So it is gameData.

			}
			else {
				//It is player 1-31. So it is triggerData.
				
			}

		}

	ReceiveIRByteToDataControl():

	{}	

	public:
		void getMessage(uint16_t message) { messageChannel.write(message); }

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