#ifndef RUNGAMECONTROL_HPP
#define RUNGAMECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayController.hpp"

#include "playerEntity.hpp"
/// @file


/// \brief
/// CLASS
/// \details
/// This class manages the lazergame itself.
/// This class will check if the player received a hit(and from who) and processed that.
/// This class will check if the buttons reload and trigger buttons are being pressed and and processed that.
/// This class uses rtos::task<>. 
class RunGameControl : public rtos::task<>{
	enum state_t {idle, saveParameters, run_game, gameOverState, hit_received, weaponButtonPressed, shoot, reload, sendDataToComputer, start_timer_until_gamestart};

	private:
		state_t state = idle;
		rtos::channel< struct shootdata, 128 > sendHitChannel;
		rtos::flag buttonFlag;
		rtos::pool <eButtonID> buttonIDPool;
		rtos::channel< struct parameters, 128 > sendGameParametersChannel;
		rtos::flag flagGameOver;
		rtos::flag StartGameFlag;
		DataToIrbyteControl& dataToIrbyteControl;
		//transferHitsControl& transferHitsControl;
		DisplayController& displayControl;
		PlayerEntity& playerEntity;
		struct parameters para;
		struct shootdata sData;
		struct shootdata weaponChanData;
		int bnID;

	public:
		RunGameControl(DataToIrbyteControl& dataToIrbyteControl, DisplayController& displayControl, PlayerEntity& playerEntity) ://, transferHitsControl& transferHitsControl,
			task("Run Game Control"),
			sendHitChannel(this, "sendHitChannel"),
			buttonFlag(this, "buttonFlag"),
			buttonIDPool("buttonIDPool"),
			sendGameParametersChannel(this, "sendGameParametersChannel"),
			flagGameOver(this, "game over flag"),
			StartGameFlag(this, "StartGameFlag"),
			dataToIrbyteControl(dataToIrbyteControl),
			//transferHitsControl (transferHitsControl),
			displayControl(displayControl),
			playerEntity(playerEntity)

		{}
		void sendHit(struct shootdata sData) { sendHitChannel.write(sData);}
		void buttonPressed(eButtonID ButtonID){buttonIDPool.write(ButtonID); buttonFlag.set();}
		void sendGameParameters(struct parameters para){sendGameParametersChannel.write(para);}
		void gameOver() {flagGameOver.set();}
		void startGame(){StartGameFlag.set();}

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
						playerEntity.setLives(5);
						state = start_timer_until_gamestart;
						break;

					case start_timer_until_gamestart:
						//entry events
						//timerControl.setUntilStartTimer(timeUntilStart);
						hwlib::cout << "before timer" << hwlib::endl;
						wait(StartGameFlag);
						//other events
						state = run_game;
						break;
	
					/*case start_game:
						//entry events
						displayControl.showMessage(para.gameMode);
						displayControl.showMessage(para.gameTime);
						//timerControl.setTimer(para.gameTime);
						break;
						*/
					
					case run_game: {
						//entry events
						displayControl.showMessage("\f");
						//hwlib::wait_ms(0);
						//displayControl.showMessage(para.gameMode);
						//displayControl.showMessage("\n");
						hwlib::wait_ms(0);
						//displayControl.showMessage(para.gameTime);
						//displayControl.showMessage("\n");
						//hwlib::wait_ms(0);
						displayControl.showMessage("lives: ");
						displayControl.showMessage(playerEntity.getlives());
						displayControl.showMessage("\n");
						hwlib::wait_ms(0);

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
							if (bnID == triggerButton) {
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
						weaponChanData = sendHitChannel.read();

						playerEntity.addData(weaponChanData);
						playerEntity.setLives(playerEntity.getlives() - 1);
						//soundControl.playSound(2);
						//hitReceivedTimer.set(800);

						//other events
						//wait(hitReceivedTimer);

						if (playerEntity.getlives() >= 1) {
							state = run_game;
							break;
						}
						else {
							state = gameOverState;
							break;
							//misch aanpassen naar flag setten en terug naar run game gaan.
						}
						state = gameOverState;
						break;

					case reload:
						//entry events
						//soundControl.playSound(3);
						if (playerEntity.getWeaponPower() == 1) {
							playerEntity.setAmmo(100);
							hwlib::wait_us(2000000);
						}
						else if (playerEntity.getWeaponPower() == 2) {
							playerEntity.setAmmo(50);
							hwlib::wait_us(1000000);
						}
						else if (playerEntity.getWeaponPower() == 3) {
							playerEntity.setAmmo(15);
							hwlib::wait_us(500000);
						}

						//other events
						state = run_game;
						break;

					case shoot:
						//entry events
						if (playerEntity.getAmmo() <= 0) {
							state = run_game;
							break;
						}
						sData.playerID = playerEntity.getPlayerID();
						sData.weaponStrength = playerEntity.getWeaponPower();
						dataToIrbyteControl.sendTriggerChannel(sData);
						//soundControl.playSound(1);

						//shootTimer.Start();
						//wait(shootTimer);
						state = run_game;
						break;

					case gameOverState:
						displayControl.showMessage("game Over\n");
						//soundControl.showMessage(4);
						displayControl.showMessage(playerEntity.getPlayerID());
						displayControl.showMessage("\n");
						hwlib::wait_ms(0);
						displayControl.showMessage(playerEntity.getlives());
						hwlib::wait_ms(0);
						//transferHitsControl.gameOver.set();
						state = idle;
						break;

					default:break;
				}
			}
		}

};

#endif // RUNGAMECONTROL_HPP