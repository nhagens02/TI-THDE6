#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// all variables are of the type int. 
struct parameters{
	int gameMode,
	int gameTime,
	int timeUntilStart;
};


/// \brief
/// Enum buttons keypad.
/// \details
/// This enum contains all possable buttons from the keypad to be pressed.
enum eButtonID = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5,BUTTON_6,BUTTON_7,BUTTON_8,BUTTON_9,BUTTON_0,BUTTON_*,BUTTON_#,BUTTON_A,BUTTON_B,BUTTON_C,BUTTON_D};


/// \brief
/// RegisterGameParametersControl CLASS
/// \details
/// This class
/// This class uses rtos::task<>. 
class RegisterGameParametersControl : public rtos::task<>{
	enum state_t = {idle,enterPlayerID,enterWeaponPower, waitForGameData, };

	private:

	RegisterGameParametersControl():

	{}

	public:
		void buttonPressed(eButtonID buttonID){buttonChannel.write(buttonID);}
		void SetParameters(struct parameters para){SetParametersChannel.write(para);}


	private:
		void main() {

			for(;;){
					switch(state)
					{
						case idle:
							//entry events


							//other events
							hwlib::wait(buttonChannel);
							bnID = buttonChannel.read();
							if (bnID == BUTTON_A) {
								playerEntity.setWeaponPower(0);
								playerEntity.setPlayerID(0)
								state = enterPlayerID;
							}
							else (bnID == BUTTON_B) {
								state = enterWeaponPower;						
							}
							break;

						case enterPlayerID:
							//entry events
							displayControl.showMessage("command");
							displayControl.showMessage("Enter PlayerID: [2 - 32]");
							playerEntity.setPlayerID(playerID);

							//other events
							hwlib::wait(buttonChannel);
							bnID = buttonChannel.read();
							if (bnID == BUTTON_B) {
								if ((playerEntity.getPlayerID() >= 2) && (playerEntity.getPlayerID() <= 31) {
									state = enterWeaponPower;
								}
								else{
									state = idle;
								}
							}
							else if ((bnID == BUTTON_0) || (bnID == BUTTON_1) || (bnID == BUTTON_2) || (bnID == BUTTON_3) || (bnID == BUTTON_4) || (bnID == BUTTON_5) || (bnID == BUTTON_6) || (bnID == BUTTON_7) || (bnID == BUTTON_8) || (bnID == BUTTON_9)) {
									if (bnID == BUTTON_0) {playerEntity.setPlayerID(0);}
									else if (bnID == BUTTON_1) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 1);}
									else if (bnID == BUTTON_2) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 2);}
									else if (bnID == BUTTON_3) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 3);}
									else if (bnID == BUTTON_4) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 4);}
									else if (bnID == BUTTON_5) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 5);}
									else if (bnID == BUTTON_6) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 6);}
									else if (bnID == BUTTON_7) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 7);}
									else if (bnID == BUTTON_8) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 8);}
									else if (bnID == BUTTON_9) {playerEntity.setPlayerID(playerEntity.getPlayerID() + 9);}
									state = enterWeaponPower;
								}
							else {
								state = enterPlayerID;
							}
							break;

						case enterWeaponPower:
							//entry events
							displayControl.showMessage("Command");
							displayControl.showMessage("Enter weapon Power: [1-9]");

							//other events
							hwlib::wait(buttonChannel);
							bnID == buttonChannel.read();
							if (bnID == BUTTON_B) {
								if ((playerEntity.getWeaponID() >= 1) && (playerEntity.getWeaponID() <= 9) {
									state = waitForGameData;
								}
								else{
									state = idle;
								}
							}
							else if ((bnID == BUTTON_0) || (bnID == BUTTON_1) || (bnID == BUTTON_2) || (bnID == BUTTON_3) || (bnID == BUTTON_4) || (bnID == BUTTON_5) || (bnID == BUTTON_6) || (bnID == BUTTON_7) || (bnID == BUTTON_8) || (bnID == BUTTON_9)) {
									if (bnID == BUTTON_0) {playerEntity.setWeaponID(0);}
									else if (bnID == BUTTON_1) {playerEntity.setWeaponID(1);}
									else if (bnID == BUTTON_2) {playerEntity.setWeaponID(2);}
									else if (bnID == BUTTON_3) {playerEntity.setWeaponID(3);}
									else if (bnID == BUTTON_4) {playerEntity.setWeaponID(4);}
									else if (bnID == BUTTON_5) {playerEntity.setWeaponID(5);}
									else if (bnID == BUTTON_6) {playerEntity.setWeaponID(6);}
									else if (bnID == BUTTON_7) {playerEntity.setWeaponID(7);}
									else if (bnID == BUTTON_8) {playerEntity.setWeaponID(8);}
									else if (bnID == BUTTON_9) {playerEntity.setWeaponID(9);}
									state = enterWeaponPower;
							}
							else {
								state = enterWeaponPower;
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
};