#ifndef TRANSFERHITSCONTROL_HPP
#define TRANSFERHITSCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "playerEntity.hpp"
/// @file


/// \brief
/// TransferHitsControl CLASS
/// \details
/// This is a class that implements the sending of the shots received of a game of lasergame.
/// The class will use the Hwlib cout function to output to a computer. 
/// This class uses rtos::task<>.
class TransferHitsControl : public rtos::task<>{
	enum state_t {idle, sending};

	private:
		state_t state = idle;
		rtos::flag gameOverFlag;
		PlayerEntity& playerEntity;
		data_s data;

	public:
		TransferHitsControl(PlayerEntity& playerEntity) :
			task("Transfer hit control"),
			gameOverFlag(this, "game over flag"),
			playerEntity(playerEntity)
		{}

		void gameOver() { gameOverFlag.set(); }

		void main() {
			for (;;) 
			{
				switch (state) {
					case idle:
						wait(gameOverFlag);
						state = sending;
						break;
					case sending:
						hwlib::cout << "Hits data for player ID " << playerEntity.getPlayerID() << ".\n\n";
						data = playerEntity.getData();
						for (unsigned int i = 0; i < data.counter; i++) {
							hwlib::cout << "Hit by player ID " << data.playerID[i] << " with weapon strength " << data.weaponStrength[i] << ".\n";
						}
						state = idle;
						break;
					default:break;
				}
			}
	}
};

#endif // TRANSFERHITSCONTROL_HPP