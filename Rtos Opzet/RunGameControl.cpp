#include "hwlib.hpp"
#include "rtos.hpp"
/// @file

/// \brief
/// enum eButtonID
/// \details
/// This enum will define the possible states for the button's.
/// The Buttons in the class RunGameControl are: reloadButton, TriggerButton.
enum eButtonID = {reloadButton, TriggerButton};


/// \brief
/// CLASS
/// \details
/// This class manages the lazergame itself.
/// This class will check if the player received a hit(and from who) and processed that.
/// This class will check if the buttons reload and trigger buttons are being pressed and and processed that.
/// This class uses rtos::task<>. 
class RunGameControl : public rtos::task<>{
	enum state_t = {idle, saveParameters, start_game, run_game, gameOver,hit_received, weaponButtonPressed, shoot, reload, sendDataToComputer};

	private:
		state_t state = idle;
		rtos::channel sendHitChannel;
		rtos::flag buttonFlag;
		rtos::pool buttonIDPool;
		rtos::channel sendGameParametersChannel;
		rtos::flag flagGameOver;
		dataToIRControl& dataToIRControl;
		transferHitsControl& transferHitsControl;
		displayControl& displayControl;
		playerEntity& playerEntity;
		int gamemode = 0;
		int gameTime = 0;
		int timeUntilStart = 0;

	RunGameControl(dataToIRControl& dataToIRControl, transferHitsControl& transferHitsControl, displayControl& displayControl, playerEntity& playerEntity):
	dataToIRControl (dataToIRControl),
	transferHitsControl (transferHitsControl),
	displayControl (displayControl),
	playerEntity (playerEntity)
	{}

	public:
		void sendHit(playerinfo,weaponStrength){sendHitChannel.write(playerinfo,weaponStrength);}
		void buttonPressed(eButtonID buttonID){buttonIDPool.write(buttonID); buttonFlag.set();}
		void sendGameParameters(gamemode,gameTime,timeUntilStart){sendGameParametersChannel.write(gamemode,gameTime,timeUntilStart);}
		void gameOver(){flagGameOver.set();}

	private:
		void main(){
			for(;;){
				switch(state)
				{
					case idle:
						//entry events
						auto events = wait(sendGameParametersChannel);
						if (events == sendGameParametersChannel){
							events = saveParameters;
						}
					break;

					case start_timer_until_gamestart:
						//entry events
						timerControl.setUntilStartTimer(timeUntilStart);
						wait(timerControl.untilStartTimerFlag);
						//other events
						state = start_game;
						break;

					case start_game:
						//entry events
						displayControl.showMessage(gameMode);
						displayControl.showMessage(gameTime);
						timerControl.setTimer(gameTime);
						break;
					
					case run_game:
						//entry events
						displayControl.showMessage(gameMode);
						displayControl.showMessage(gameTime);
						displayControl.showMessage(playerEntity.getLives());
						//other events
						auto event = wait(gameOverFlag,sendHitChannel,buttonPressedChannel);
						if (event == gameOverFlag) {
							state = gameOver;
						}
						if (event == sendHitChannel) {
							state = hit_received;
						}
						if (event == buttonPressedChannel) {
							bnID = buttonPressedChannel.read();
							if (bnID == reloadButton) {
								state = reload;
							}
							if (bnID == TriggerButton ) {
								state = shoot;
							}
							else{
								state = run_game;
							}
						}
						break;

					case hit_received:
						//entry events
						//weaponID = sendHitChannel.read();
						playerEntity.addHit(sendHitChannel.read());
						playerEntity.setLives(playerEntity.getLives - sendHitChannel.read());
						soundControl.playSound(2);
						hitReceivedTimer.start();

						//other events
						wait(hitReceivedTimerFlag);

						if (playerEntity.getLives >= 1) {
							state = run_game;
						}
						else{
							state = gameOver;
							//misch aanpassen naar flag setten en terug naar run game gaan.
						}
						break;

					case reload:
						//entry events
						soundControl.playSound(3);
						reloadTimer.start();

						//other events
						wait(reloadTimerFlag);

					case shoot:
						//entry events
						DataToIRByteControl.sendTrigger(playerEntity.getPlayerID(),playerEntity.getWeaponPower());
						soundControl.playSound(1);

						shootTimer.Start();
						wait(shootTimerFlag);

					case gameOver:
						displayControl.showMessage("game Over");
						soundControl.showMessage(4);
						displayControl.showMessage(playerEntity.getPlayerID());
						displayControl.showMessage(playerEntity.getLives());
						transferHitsControl.gameOver.set();
						state = idle;
						break;

					default:break;
				}
			}
		}

};