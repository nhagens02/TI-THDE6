#include "hwlib.hpp"
#include "rtos.hpp"

enum gameModes {
	regular = 0, goldenGun = 1
};


enum gameTimes {
	veryShortTime = 0, shortTime = 1, mediumTime = 2, longTime = 3, veryLongTime = 4, testing = 7
};


class ReceiveIRByteToDataControl : public rtos::task<>{
	enum state_t = {idle, decodeData};

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