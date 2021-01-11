#ifndef REGISTERGAMEPARAMETERSCONTROL_HPP
#define REGISTERGAMEPARAMETERSCONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "playerEntity.hpp"
#include "DisplayController.hpp"
/// @file


/// \brief
/// RegisterGameParametersControl CLASS
/// \details
/// This class will set the player number and the weaponStrenght for the player. 
/// This class will be triggerd if a player presses on the A button on the keypad. 
/// This class uses rtos::task<>. 
class RegisterGameParametersControl : public rtos::task<>{
	enum state_t {idle,enterPlayerID,enterWeaponPower, waitForGameData};

	private:
		state_t state = idle;
		struct parameters para;
		int bnID;
		rtos::channel< int, 32 > buttonChannel;
		rtos::channel< struct parameters, 32 > SetParametersChannel;
		PlayerEntity& playerEntity;
		DisplayController& displayControl;

	public:
		RegisterGameParametersControl(PlayerEntity& playerEntity, DisplayController& displayControl):
			task("registerparameterscontrol"),
			buttonChannel(this, "buttonpressChannel"),
			SetParametersChannel(this, "parameterschannel"),
			playerEntity( playerEntity ),
			displayControl( displayControl )
		{}

		void buttonPressed(int buttonID){buttonChannel.write(buttonID);}
		void SetParameters(struct parameters para){SetParametersChannel.write(para);}

	private:
		void main() {
			for (;;) {
				switch (state)
				{
				case idle:
					wait(buttonChannel);
					bnID = buttonChannel.read();
					if (bnID == 10) { //A = 10
						playerEntity.setWeaponPower(0);
						playerEntity.setPlayerID(0);
						state = enterPlayerID;
						break;
					}
					else {
						state = idle;
						break;
					}
					break;

				case enterPlayerID:
					//entry events
					displayControl.showMessage("\fcommand\n");
					hwlib::wait_ms(0);
					displayControl.showMessage("Enter PlayerID:\n");
					hwlib::wait_ms(0);
					displayControl.showMessage(playerEntity.getPlayerID());

					//other events
					wait(buttonChannel);
					bnID = buttonChannel.read();
					if (bnID == 15) { // 15 = #
						if ((playerEntity.getPlayerID() >= 2) && (playerEntity.getPlayerID() <= 31)) {
							state = enterWeaponPower;
							break;
						}
						else {
							state = idle;
							break;
						}
					}
					else if ((bnID >= 0) || (bnID <= 9)) {
						playerEntity.setPlayerID(playerEntity.getPlayerID() * 10);
						playerEntity.setPlayerID(playerEntity.getPlayerID() + bnID);
						state = enterPlayerID;
						break;
					}
					else {
						state = enterPlayerID;
						break;
					}
					break;

				case enterWeaponPower:
					//entry events
					displayControl.showMessage("\fCommand\n");
					hwlib::wait_ms(0);
					displayControl.showMessage("Enter weapon\nPower:");
					hwlib::wait_ms(0);
					displayControl.showMessage(playerEntity.getWeaponPower());

					//other events
					wait(buttonChannel);
					bnID = buttonChannel.read();
					if (bnID == 15) { // 15 = #
						if ((playerEntity.getWeaponPower() >= 1) && (playerEntity.getWeaponPower() <= 9)) {
							state = waitForGameData;
							break;
						}
						else {
							state = idle;
							break;
						}
					}
					else if ((bnID >= 0) || (bnID <= 9)) {
						playerEntity.setWeaponPower(playerEntity.getWeaponPower() * 10);
						playerEntity.setWeaponPower(playerEntity.getWeaponPower() + bnID);
						state = enterWeaponPower;
						break;
					}
					else {
						state = enterWeaponPower;
						break;
					}
					break;

				case waitForGameData:
					//entry events
					displayControl.showMessage("\fWaiting for game\nLeader....\n");
					//other events
					wait(SetParametersChannel);

					state = idle;
					break;
				}
			}
		}	
};

#endif // REGISTERGAMEPARAMETERSCONTROL_HPP