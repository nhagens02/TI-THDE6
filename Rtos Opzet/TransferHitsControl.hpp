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
	void transferHits(PlayerEntity player) {
		hwlib::cout << "Hits data for player ID " << player.getPlayerID() << ".\n\n";
		data_s data = player.getData();
		for (unsigned int i = 0; i < data.counter; i++) {
			hwlib::cout << "Hit by player ID " << data.playerID[i] << " with weapon strength " << data.weaponStrength << ".\n";
		}
	}
};