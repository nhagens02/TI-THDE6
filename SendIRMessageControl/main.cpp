#include "hwlib.hpp"

enum gameModes {
	regular = 0, goldenGun = 1
};

enum gameTimes {
	veryShortTime = 0, shortTime = 1, mediumTime = 2, longTime = 3, veryLongTime = 4, testing = 7
};

enum buttons {
	triggerButton = 0, dataButton = 1
};

class DataToIRByteControl {
private:
	//SendIRMessageControl sendIRMessageControl
public:
	void sendingGameParameters(int gamemodeID, int gameTime, int timeUntilStart) {

	}
};



bool buttonPressed(int buttonID) {
	if (buttonID == dataButton) {
		
	}
}

int main() {
	hwlib::wait_ms(2000);
	int gamemode = regular;
	int gameTime = testing;
	int timeUntilStart = 30;
	int lastTime = 0;
	while (timeUntilStart > 0) {

		if (buttonPressed(dataButton)) {

		}

		int currentTime = hwlib::now_us() / 500'000;
		if (currentTime > lastTime) {
			lastTime++;
			timeUntilStart--;
		}
	}
	hwlib::cout << "done!\n";
}