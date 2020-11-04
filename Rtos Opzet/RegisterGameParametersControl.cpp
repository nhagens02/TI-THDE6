#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "playerEntity.hpp"
#include "DisplayController.hpp"
/// @file


/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// All variables are of the type int. 
//struct parameters{
//	int gameMode,
//	int gameTime,
//	int timeUntilStart;
//};


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
		rtos::channel< int, 1024 > buttonChannel;
		rtos::channel< struct parameters, 1024 > SetParametersChannel;
		PlayerEntity& playerEntity;
		DisplayController& displayControl;
	

	public:
		RegisterGameParametersControl(PlayerEntity& playerEntity, DisplayController& displayControl):
			task("register parameters control"),
			buttonChannel(this, "button press Channel"),
			SetParametersChannel(this, "parameters channel"),
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
					//entry events


					//other events
					wait(buttonChannel);
					bnID = buttonChannel.read();
					if (bnID == 10) { //A = 10
						playerEntity.setWeaponPower(0);
						playerEntity.setPlayerID(0);
						state = enterPlayerID;
					}
					else if (bnID == 11) { //B = 11
						state = enterWeaponPower;
					}
					break;

				case enterPlayerID:
					//entry events
					displayControl.showMessage("\fcommand\n");
					displayControl.showMessage("Enter PlayerID:\n");
					displayControl.showMessage(playerEntity.getPlayerID());
					playerEntity.setPlayerID(playerID);

					//other events
					wait(buttonChannel);
					bnID = buttonChannel.read();
					if (bnID == 11) { // 11 = B
						if ((playerEntity.getPlayerID() >= 2) && (playerEntity.getPlayerID() <= 31) {
							state = enterWeaponPower;
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
						state = enterPlayerID;
						break;
					}
					break;

				case enterWeaponPower:
					//entry events
					displayControl.showMessage("\fCommand\n");
					hwlib::wait_ms(0);
					displayControl.showMessage("Enter weapon Power:\n");

					//other events
					wait(buttonChannel);
					bnID == buttonChannel.read();
					if (bnID == 11) { // 11 = B
						if ((playerEntity.getWeaponID() >= 1) && (playerEntity.getWeaponID() <= 9) {
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
					displayControl.showmessage("Waiting for game Leader for information about the game....");

					//other events
					wait(SetParametersChannel)
						struct parameters para = SetParametersChannel.read();
					runGameControl.sendGameParameters(para);
				}
			}
			}

		}
		}
};