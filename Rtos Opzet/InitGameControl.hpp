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
		state_t state = idle;
		struct parameters para;
		int bnID;
		int now = 0;
		rtos::channel< int, 128 > buttonChannel;
		DataToIrbyteControl& dataToIrByteControl;
		DisplayController& displayControl;
		RunGameControl& runGameControl;
		TimerControl& timerControl;

	public:
		InitGameControl(DataToIrbyteControl& dataToIrByteControl, DisplayController& displayControl, RunGameControl& runGameControl, TimerControl& timerControl) :
			task("init game controller"),
			buttonChannel(this, "button press Channel"),
			dataToIrByteControl(dataToIrByteControl),
			displayControl( displayControl ),
			runGameControl( runGameControl ),
			timerControl(timerControl)
		{}
		void buttonPressed(int buttonID){buttonChannel.write(buttonID);}

	private:
		void main(){
			for(;;)
			{
				switch(this->state)
				{
					case idle:
						wait(this->buttonChannel);
						bnID = this->buttonChannel.read();

						if (bnID == 12) { //bnID 12 = BUTTON C
							this->para = {0, 0, 0};
							this->state = enterPlayTime;
						}
						break;
					case enterPlayTime:
						this->displayControl.showMessage("\fcommand\nEnter play time:\n");
						this->displayControl.showMessage(this->para.gameTime);

						wait(this->buttonChannel);
						bnID = this->buttonChannel.read();

						if (bnID == 15 && this->para.gameTime <= 7) { //bnID 15 = BUTTON #
							this->state = setGameMode;
						}
						else if (bnID >= 0 && bnID <= 7) {
							this->para.gameTime = bnID;
						}
						break;
					case setGameMode:
						this->displayControl.showMessage("\fcommand\nEnter game Mode:\n");
						this->displayControl.showMessage(this->para.gameMode);

						wait(this->buttonChannel);
						bnID = this->buttonChannel.read();

						if (bnID == 15 && this->para.gameMode <= 1) { //bnID 15 = BUTTON #
							this->state = SetTimeUntilStart;
						}
						else if (bnID <= 1) {
							this->para.gameMode = bnID;
						}
						break;
					case SetTimeUntilStart:
						this->displayControl.showMessage("\fcommand\nEnter time until\nstart:");
						this->displayControl.showMessage(this->para.timeUntilStart);

						wait(this->buttonChannel);
						bnID = this->buttonChannel.read();
						
						if (bnID == 15 && this->para.timeUntilStart >= 1 && this->para.timeUntilStart <= 31) { //bnID 15 = BUTTON #
							this->state = sendData;
						}
						else if (bnID >= 0 && bnID <= 9) {
							this->para.timeUntilStart = this->para.timeUntilStart * 10;
							this->para.timeUntilStart += bnID;
						}
						break;
					case sendData:
						this->displayControl.showMessage("\fPress * to send para\nPress # to go to\nStart.\n");

						wait(this->buttonChannel);
						bnID = this->buttonChannel.read();

						if (bnID == 15) { //bnID 15 = BUTTON #
							this->runGameControl.sendGameParameters(this->para);
							this->timerControl.setTimer(this->para);
							this->state = idle;
						}
						else if (bnID == 14) { //bnID 14 = BUTTON *
							this->dataToIrByteControl.sendingGameParametersChannel(para);
						}
						break;
					default:break;
				}
			}
		}
};

#endif // INITGAMECONTROL_HPP
