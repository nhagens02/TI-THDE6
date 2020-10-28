#include "hwlib.hpp"
#include "SendIRMessageControl.hpp"


enum gameModes {
	regular = 0, goldenGun = 1
};


enum gameTimes {
	veryShortTime = 0, shortTime = 1, mediumTime = 2, longTime = 3, veryLongTime = 4, testing = 7
};


class DataToIRByteControl {
private:
	SendIRMessageControl sendIRMessageControl;
public:
	DataToIRByteControl(hwlib::pin_out& ledPin):
		sendIRMessageControl(ledPin)
	{}

	void sendGameParameters(int gamemodeID, int gameTime, int timeUntilStart) {
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
			if (information & (1 << (i + 4)))y=1;
			information += (x ^ y) << (i - 1);
		}
		sendIRMessageControl.sendBytes(information);
	}
};


int main() {
	auto irPin = hwlib::target::d2_38kHz();
	DataToIRByteControl dataToIR(irPin);
	hwlib::wait_ms(2000);
	int gamemode = regular;
	int gameTime = testing;
	int timeUntilStart = 30;
	int lastTime = 0;
	auto button_ = hwlib::target::pin_in(hwlib::target::pins::d44);
	auto button = hwlib::invert(button_);
	bool buttonAlreadyPressed = 0;
	while (timeUntilStart > 0) {
		if (button.read()) {
			if (!buttonAlreadyPressed) {
				dataToIR.sendGameParameters(gamemode, gameTime, timeUntilStart);
				buttonAlreadyPressed = 1;
			}
		}
		else {
			buttonAlreadyPressed = 0;
		}

		int currentTime = hwlib::now_us() / 2'000'000;
		if (currentTime > lastTime) {
			lastTime++;
			timeUntilStart--;
		}
		hwlib::wait_ms(50);
	}
	hwlib::cout << "done!\n";
}