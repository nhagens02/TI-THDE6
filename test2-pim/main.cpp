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
		//Gamemode and Gametime
		uint_fast16_t information = 0;
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
		hwlib::wait_ms(5);

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
		sendIRMessageControl.sendBytes(information);
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
		sendIRMessageControl.sendBytes(information);
	}
};


int main() {
	auto irPin = hwlib::target::d2_36kHz();
	DataToIRByteControl dataToIR(irPin);
	hwlib::wait_ms(2000);
	int gamemode = regular;
	int gameTime = testing;
	int timeUntilStart = 30;
	int lastTime = 0;

	int playerID = 1;
	int weaponStrength = 5;

	auto triggerButton_ = hwlib::target::pin_in(hwlib::target::pins::d3);
	auto triggerButton = hwlib::invert(triggerButton_);
	auto dataButton_ = hwlib::target::pin_in(hwlib::target::pins::d44);
	auto dataButton = hwlib::invert(dataButton_);
	bool dataButtonAlreadyPressed = 0;
	while (timeUntilStart > 0) {
		dataButton.refresh();
		if (dataButton.read()) {
			if (!dataButtonAlreadyPressed) {
				dataToIR.sendGameParameters(gamemode, gameTime, timeUntilStart);
				dataButtonAlreadyPressed = 1;
			}
		}
		else {
			dataButtonAlreadyPressed = 0;
		}

		int currentTime = hwlib::now_us() / 2'000'000;
		if (currentTime > lastTime) {
			lastTime++;
			timeUntilStart--;
			hwlib::cout << timeUntilStart << "\n";
		}
		hwlib::wait_ms(50);
	}
	hwlib::cout << "done!\n";

	bool triggerButtonAlreadyPressed = 0;
	while (true) {
		triggerButton.refresh();
		if (triggerButton.read()) {
			if (!triggerButtonAlreadyPressed) {
				dataToIR.sendTrigger(playerID, weaponStrength);
				triggerButtonAlreadyPressed = 1;
			}
		}
		else {
			triggerButtonAlreadyPressed = 0;
		}
	}
}