#ifndef RUNGAMECONTROL_HPP
#define RUNGAMECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "DisplayController.hpp"
#include "playerEntity.hpp"
#include "TransferHitsControl.hpp"
#include "soundControl.hpp"
/// @file


/// \brief
/// CLASS
/// \details
/// This class manages the lazergame itself.
/// This class will check if the player received a hit(and from who) and processed that.
/// This class will check if the buttons reload and trigger buttons are being pressed and and processed that.
/// This class uses rtos::task<>. 
class RunGameControl : public rtos::task<>{
	enum state_t {idle, start_timer_until_gamestart, run_game, hit_received, reload, shoot, gameOverState};

	private:
		state_t state = idle;
		rtos::channel< struct shootdata, 128 > sendHitChannel;
		rtos::flag buttonFlag;
		rtos::pool <eButtonID> buttonIDPool;
		rtos::channel< struct parameters, 128 > sendGameParametersChannel;
		rtos::flag flagGameOver;
		rtos::flag StartGameFlag;
		DataToIrbyteControl& dataToIrbyteControl;
		TransferHitsControl& transferHitsControl;
		DisplayController& displayControl;
		PlayerEntity& playerEntity;
		SoundControl& soundControl;
		struct parameters para;
		struct shootdata sData;
		struct shootdata weaponChanData;
		int bnID;
		int lives;

	public:
		RunGameControl(DataToIrbyteControl& dataToIrbyteControl, DisplayController& displayControl, PlayerEntity& playerEntity,TransferHitsControl& transferHitsControl, SoundControl& soundControl) :
			task("Run Game Control"),
			sendHitChannel(this, "sendHitChannel"),
			buttonFlag(this, "buttonFlag"),
			buttonIDPool("buttonIDPool"),
			sendGameParametersChannel(this, "sendGameParametersChannel"),
			flagGameOver(this, "game over flag"),
			StartGameFlag(this, "Start Game Flag"),
			dataToIrbyteControl(dataToIrbyteControl),
			transferHitsControl(transferHitsControl),
			displayControl(displayControl),
			playerEntity(playerEntity),
			soundControl(soundControl)
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
						//entry events
						wait(sendGameParametersChannel);
						para = sendGameParametersChannel.read();
						state = start_timer_until_gamestart;
						break;

					case start_timer_until_gamestart:
						//entry events
						displayControl.showMessage("\fGame is\nstarting\nSoon..");
						wait(StartGameFlag);
						if (playerEntity.getWeaponPower() == 1) {playerEntity.setAmmo(100);}
						else if (playerEntity.getWeaponPower() == 2) {playerEntity.setAmmo(50);}
						soundControl.playSound(1);
						//other events
						state = run_game;
						break;
					
					case run_game: {
						//entry events
						displayControl.showMessage("\flives: ");
						displayControl.showMessage(playerEntity.getlives());

						displayControl.showMessage("\nAmmo: ");
						displayControl.showMessage(playerEntity.getAmmo());
						displayControl.showMessage("\n");

						//other events
						auto evt = wait(sendHitChannel + flagGameOver + buttonFlag);
						if (evt == flagGameOver) {
							state = gameOverState;
							break;
						}
						else if (evt == sendHitChannel) {
							state = hit_received;
							break;
						}
						else if (evt == buttonFlag) {
							bnID = buttonIDPool.read();
							if (bnID == reloadButton) {
								state = reload;
								break;
							}
							else if (bnID == triggerButton) {
								state = shoot;
								break;
							}
							else {
								state = run_game;
								break;
							}
						}
						else {
							state = run_game;
							break;
						}
						state = run_game;
						break;
					}
					case hit_received:
						//entry events
						
						weaponChanData = sendHitChannel.read();

						playerEntity.addData(weaponChanData);
						playerEntity.setLives(playerEntity.getlives() - 1);

						soundControl.playSound(3);
						hwlib::wait_ms(0);
						//other events

						if (playerEntity.getlives() >= 1) {
							state = run_game;
							break;
						}
						else {
							state = gameOverState;
							break;
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
						//other events
						state = run_game;
						break;

					case shoot:
						//entry events
						if (playerEntity.getAmmo() <= 0) {
							state = run_game;
							break;
						}
						//ammo implenenatie
						sData.playerID = playerEntity.getPlayerID();
						sData.weaponStrength = playerEntity.getWeaponPower();
						playerEntity.setAmmo(playerEntity.getAmmo()-1);
						dataToIrbyteControl.sendTriggerChannel(sData);
						soundControl.playSound(2);

						hwlib::wait_ms(0);
						state = run_game;
						break;

					case gameOverState:
						displayControl.showMessage("\fgame Over\nplayerID: ");
						displayControl.showMessage(playerEntity.getPlayerID());
						hwlib::wait_ms(0);
						displayControl.showMessage("\nlives: ");
						displayControl.showMessage(playerEntity.getlives());
						hwlib::wait_ms(0);
						transferHitsControl.gameOver();
						state = idle;
						break;

					default:break;
				}
			}
		}
};

#endif // RUNGAMECONTROL_HPP