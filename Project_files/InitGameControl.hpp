#ifndef INITGAMECONTROL_HPP
#define INITGAMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "DataToIrByteControl.hpp"
#include "DisplayController.hpp"
#include "RunGameControl.hpp"
#include "TimerControl.hpp"
/// @file


/// \brief
/// InitGameControl CLASS
/// \details
/// This class will be used by the game leader. 
/// This class will be used to set the game parameters. 
/// It sets the gamemode, gameTime and Time until the game start. 
/// This class uses rtos::task<>. 
class InitGameControl : public rtos::task<>{
	enum state_t {idle, init, enterPlayTime, setGameMode, SetTimeUntilStart, sendData};

	private:
		struct parameters para;
		uint64_t preTimer;
		bool isSending;
		state_t state;
		int bnID;
		int baseTime;

		rtos::channel< int, 128 > buttonChannel;
		rtos::clock intervalKeyCheck;
		DataToIrbyteControl& dataToIrByteControl;
		DisplayController& displayControl;
		RunGameControl& runGameControl;
		TimerControl& timerControl;

	public:
		InitGameControl(DataToIrbyteControl& dataToIrByteControl, DisplayController& displayControl, RunGameControl& runGameControl, TimerControl& timerControl) :
			task("init game controller"),
			buttonChannel(this, "button press Channel"),
			intervalKeyCheck(this, (2000*rtos::ms), "keypad interval checker"),
			dataToIrByteControl(dataToIrByteControl),
			displayControl( displayControl ),
			runGameControl( runGameControl ),
			timerControl(timerControl)
		{
			this->state = this->idle;
			this->isSending = false;
		}

		void buttonPressed(int buttonID)
		{
			buttonChannel.write(buttonID);
		}

	private:
		void main(){
			for(;;)
			{
				switch(this->state)
				{
					case this->idle:
						wait(this->buttonChannel);
						this->bnID = this->buttonChannel.read();

						if (this->bnID == 12) { //bnID 12 = BUTTON C
							this->para = {0, 0, 0};
							this->state = this->enterPlayTime;
						}
						break;
					case this->enterPlayTime:
						this->displayControl.showMessage("\fcommand\nEnter play time:\n");
						this->displayControl.showMessage(this->para.gameTime);

						wait(this->buttonChannel);
						this->bnID = this->buttonChannel.read();

						if (this->bnID == 15 && this->para.gameTime <= 7) { //bnID 15 = BUTTON #
							this->state = setGameMode;
						}
						else if (this->bnID >= 0 && this->bnID <= 7) {
							this->para.gameTime = this->bnID;
						}
						break;
					case this->setGameMode:
						this->displayControl.showMessage("\fcommand\nEnter game Mode:\n");
						this->displayControl.showMessage(this->para.gameMode);

						wait(this->buttonChannel);
						this->bnID = this->buttonChannel.read();

						if (this->bnID == 15 && this->para.gameMode <= 1) { //bnID 15 = BUTTON #
							this->state = this->SetTimeUntilStart;
						}
						else if (this->bnID <= 1) {
							this->para.gameMode = this->bnID;
						}
						break;
					case this->SetTimeUntilStart:
						this->displayControl.showMessage("\fcommand\nEnter time until\nstart:");
						this->displayControl.showMessage(this->para.timeUntilStart);

						wait(this->buttonChannel);
						this->bnID = this->buttonChannel.read();
						
						if (this->bnID == 15 && this->para.timeUntilStart >= 1 && this->para.timeUntilStart <= 31) { //bnID 15 = BUTTON #
							this->state = this->sendData;
						}
						else if (this->bnID >= 0 && this->bnID <= 9) {
							this->para.timeUntilStart = this->para.timeUntilStart * 10;
							this->para.timeUntilStart += this->bnID;
						}
						break;
					case this->sendData: {
						this->displayControl.showMessage("\fPress * to send para\nPress # to go to\nStart.\n");
						
						uint64_t time = hwlib::now_us() - this->preTimer;

						if(!this->isSending) {
							this->preTimer = hwlib::now_us();
							baseTime = (((this->para.timeUntilStart * 4000) * rtos::ms) / 4000) / 1000;
							this->isSending = true;
						}
						else if(((this->para.timeUntilStart * 4000) * rtos::ms) > time) {
							if(wait(this->buttonChannel + this->intervalKeyCheck) == this->buttonChannel) {
								this->bnID = this->buttonChannel.read();
								if (this->bnID == 14) { //bnID 14 = BUTTON *
									// adjust time
									this->para.timeUntilStart = (baseTime - ((time / 4000) / rtos::ms));
									this->dataToIrByteControl.sendingGameParametersChannel(this->para);
								}
							}
						}
						else {
							this->preTimer = 0;
							this->isSending = false;

							this->para.timeUntilStart = 1;
							this->runGameControl.sendGameParameters(this->para);
							this->timerControl.setTimer(this->para);

							this->state = this->idle;
						}
						break;
					}
					default:break;
				}
			}
		}
};

#endif // INITGAMECONTROL_HPP
