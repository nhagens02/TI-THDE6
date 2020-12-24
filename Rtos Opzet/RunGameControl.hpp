#ifndef RUNGAMECONTROL_HPP
#define RUNGAMECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayController.hpp"

#include "playerEntity.hpp"
/// @file

/// \brief
/// enum eButtonID
/// \details
/// This enum will define the possible states for the button's.
/// The Buttons in the class RunGameControl are: reloadButton, TriggerButton.
enum eButtonID {reloadButton, TriggerButton};


/// \brief
/// CLASS
/// \details
/// This class manages the lazergame itself.
/// This class will check if the player received a hit(and from who) and processed that.
/// This class will check if the buttons reload and trigger buttons are being pressed and and processed that.
/// This class uses rtos::task<>. 
class RunGameControl : public rtos::task<>{
	enum state_t {idle, saveParameters, start_game, run_game, gameOverState, hit_received, weaponButtonPressed, shoot, reload, sendDataToComputer, start_timer_until_gamestart};

	private:
		state_t state = idle;
		rtos::channel< struct shootdata, 128 > sendHitChannel;
		rtos::flag buttonFlag;
		rtos::pool <int> buttonIDPool;
		rtos::channel< struct parameters, 128 > sendGameParametersChannel;
		rtos::flag flagGameOver;
		rtos::flag StartGameFlag;
		DataToIrbyteControl& dataToIrbyteControl;
		//transferHitsControl& transferHitsControl;
		DisplayController& displayControl;
		PlayerEntity& playerEntity;
		struct parameters para;
		struct shootdata sData;
		int bnID;

	public:
		RunGameControl(DataToIrbyteControl& dataToIrbyteControl, DisplayController& displayControl, PlayerEntity& playerEntity) ://, transferHitsControl& transferHitsControl,
			task("Run Game Control"),
			sendHitChannel(this, "sendHitChannel"),
			buttonFlag(this, "buttonFlag"),
			buttonIDPool("buttonIDPool"),
			sendGameParametersChannel(this, "sendGameParametersChannel"),
			flagGameOver(this, "flagGameOver"),
			StartGameFlag(this, "StartGameFlag"),
			dataToIrbyteControl(dataToIrbyteControl),
			//transferHitsControl (transferHitsControl),
			displayControl(displayControl),
			playerEntity(playerEntity)

		{}
		void sendHit(struct shootdata sData) { sendHitChannel.write(sData);}
		void buttonPressed(int buttonID){buttonIDPool.write(buttonID); buttonFlag.set();}
		void sendGameParameters(struct parameters para){sendGameParametersChannel.write(para);}
		void gameOver(){flagGameOver.set();}
		void StartGame() { StartGameFlag.set(); }

	private:
		void main(){
			for(;;){
				switch(state)
				{
					case idle:
						//task::suspend();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						//entry events
						wait(sendGameParametersChannel);//wait disabled for testing
						para = sendGameParametersChannel.read();

						//para.gameMode = 1;
						//para.gameTime = 5;
						//para.timeUntilStart = 10;

					
						hwlib::cout << para.gameMode << hwlib::endl;
						hwlib::cout << para.gameTime << hwlib::endl;
						hwlib::cout << para.timeUntilStart << hwlib::endl;
						state = start_timer_until_gamestart;
						break;

					case start_timer_until_gamestart:
						//entry events
						//timerControl.setUntilStartTimer(timeUntilStart);
						hwlib::cout << "before timer" << hwlib::endl;
						wait(StartGameFlag);
						//other events
						state = start_game;
						break;

					case start_game:
						//entry events
						displayControl.showMessage(para.gameMode);
						displayControl.showMessage(para.gameTime);
						//timerControl.setTimer(para.gameTime);
						break;
					
					case run_game: {
						//entry events
						displayControl.showMessage(para.gameMode);
						displayControl.showMessage(para.gameTime);
						displayControl.showMessage(playerEntity.getlives());
						//other events
						auto event = wait(flagGameOver + sendHitChannel + buttonFlag);
						if (event == flagGameOver) {
							state = gameOverState;
							break;
						}
						if (event == sendHitChannel) {
							state = hit_received;
							break;
						}
						if (event == buttonFlag) {
							bnID = buttonIDPool.read();
							if (bnID == reloadButton) {
								state = reload;
								break;
							}
							if (bnID == TriggerButton) {
								state = shoot;
								break;
							}
							else {
								state = run_game;
								break;
							}
						}
						break;
					}
					case hit_received:
						//entry events
						//weaponID = sendHitChannel.read();
						playerEntity.addData(sendHitChannel.read());
						playerEntity.setLives(playerEntity.getlives() - 1);
						//soundControl.playSound(2);
						//hitReceivedTimer.set(800);

						//other events
						//wait(hitReceivedTimer);

						if (playerEntity.getlives() >= 1) {
							state = run_game;
							break;
						}
						else{
							state = gameOverState;
							break;
							//misch aanpassen naar flag setten en terug naar run game gaan.
						}
						break;

					case reload:
						//entry events
						//soundControl.playSound(3);
						//reloadTimer.start();

						//other events
						//wait(reloadTimerFlag);
						state = run_game;
						break;

					case shoot:
						//entry events
						sData.playerID = playerEntity.getPlayerID();
						sData.weaponStrength = playerEntity.getWeaponPower();
						dataToIrbyteControl.sendTriggerChannel(sData);
						//soundControl.playSound(1);

						//shootTimer.Start();
						//wait(shootTimer);
						state = run_game;
						break;

					case gameOverState:
						displayControl.showMessage("game Over");
						//soundControl.showMessage(4);
						displayControl.showMessage(playerEntity.getPlayerID());
						displayControl.showMessage(playerEntity.getlives());
						//transferHitsControl.gameOver.set();
						state = idle;
						break;

					default:break;
				}
			}
		}

};

#endif // RUNGAMECONTROL_HPP