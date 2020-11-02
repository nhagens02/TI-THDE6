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
/// InitGameControl CLASS
/// \details
/// This class will be used by the game leader. 
/// This class will be used to set the game parameters. 
/// It set the gamemode, gameTime and Time until the game start. 
/// This class uses rtos::task<>. 
class InitGameControl : public rtos::task<>{
	enum state_t = {idle,init,enterPlayTime,setGameMode,SetTimeUntilStart,sendData};

	private:
		state_t state = idle;
		rtos::channel buttonChannel;
		registerGameParametersControl& registerGameParametersControl;
		dataToIRByteControl& dataToIRByteControl;

	InitGameControl(registerGameParametersControl& registerGameParametersControl,dataToIRByteControl& dataToIRByteControl):
	registerGameParametersControl (registerGameParametersControl),
	dataToIRByteControl (dataToIRByteControl)
	{buttonHandler.addButton();}


	public:
		void buttonPressed(eButtonID buttonID){buttonChannel.write(buttonID);}

	private:
			void main(){

				for(;;){
						switch(state)
						{
							case idle:
								//entry events

								//other events
								//check deze state nog ff.
								wait(buttonPressChannel);
								bnID = buttonPressChannel.read()
								if (bnID == BUTTON_C) {
									state = init;
								}
								else {
									state = idle;
								}
								break;

							case init:
								displayControl.showMessage("command");
								displayControl.showMessage("Enter play time:");
								struct parameters para;
								para.playTime = 0;
								para.gameMode = 0;
								para.timeUntilStart = 0;
								state = enterPlayTime;
								break;

							case enterPlayTime:
								//entry events
								displayControl.showMessage("command");
								displayControl.showMessage("Enter play time:")

								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == BUTTON_#) {
									if ((para.gameTime >= 1) && (para.gameTime <= 15)) {
										state = setGameMode;
									}
									else {
										state = init;
									}
								}
								else if ((bnID == BUTTON_0) || (bnID == BUTTON_1) || (bnID == BUTTON_2) || (bnID == BUTTON_3) || (bnID == BUTTON_4) || (bnID == BUTTON_5) || (bnID == BUTTON_6) || (bnID == BUTTON_7) || (bnID == BUTTON_8) || (bnID == BUTTON_9)) {
									if (bnID == BUTTON_0) {para.gameTime = 0;}
									else if (bnID == BUTTON_1) {para.gameTime += 1;}
									else if (bnID == BUTTON_2) {para.gameTime += 2;}
									else if (bnID == BUTTON_3) {para.gameTime += 3;}
									else if (bnID == BUTTON_4) {para.gameTime += 4;}
									else if (bnID == BUTTON_5) {para.gameTime += 5;}
									else if (bnID == BUTTON_6) {para.gameTime += 6;}
									else if (bnID == BUTTON_7) {para.gameTime += 7;}
									else if (bnID == BUTTON_8) {para.gameTime += 8;}
									else if (bnID == BUTTON_9) {para.gameTime += 9;}
								}
								else {
									state = enterPlayTime
								}
								break;

							case setGameMode:
								//entry events
								displayControl.showMessage("command");
								displayControl.showMessage("Enter game Mode:");

								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == BUTTON_#) {
									if ((para.gameMode >= 1) && (para.gameMode <= 2)) {
										state = SetTimeUntilStart;
									}
									else {
										state = init;
									}
								}
								else if ((bnID == BUTTON_1) || (bnID == BUTTON_2)) {
									if (bnID == BUTTON_1) {para.gameTime = 1;}
									else if (bnID == BUTTON_2) {para.gameTime = 2;}
								}
								else {
									state = setGameMode;
								}
								break;

							case SetTimeUntilStart:
								//entry events
								displayControl.showMessage("command");
								displayControl.showMessage("Enter time until start of game:");
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == BUTTON_#) {
									state = sendData;
									//if ((gameMode >= 1) && (gameMode <= 2)) {
									//	state = SetTimeUntilStart;
									//}
									//else {
									//	state = init;
									//}
								}
								else if ((bnID == BUTTON_0) || (bnID == BUTTON_1) || (bnID == BUTTON_2) || (bnID == BUTTON_3) || (bnID == BUTTON_4) || (bnID == BUTTON_5) || (bnID == BUTTON_6) || (bnID == BUTTON_7) || (bnID == BUTTON_8) || (bnID == BUTTON_9)) {
									if (bnID == BUTTON_0) {para.timeUntilStart = 0;}
									else if (bnID == BUTTON_1) {para.timeUntilStart += 1;}
									else if (bnID == BUTTON_2) {para.timeUntilStart += 2;}
									else if (bnID == BUTTON_3) {para.timeUntilStart += 3;}
									else if (bnID == BUTTON_4) {para.timeUntilStart += 4;}
									else if (bnID == BUTTON_5) {para.timeUntilStart += 5;}
									else if (bnID == BUTTON_6) {para.timeUntilStart += 6;}
									else if (bnID == BUTTON_7) {para.timeUntilStart += 7;}
									else if (bnID == BUTTON_8) {para.timeUntilStart += 8;}
									else if (bnID == BUTTON_9) {para.timeUntilStart += 9;}
								}
								else {
									state = SetTimeUntilStart;
								}
								break;

							case sendData:
								//entry events
								dataToIRByteControl.sendGameParameters(para);
								displayControl.showmessage("Press any button exept the * button to send parameters, press the * to go to start.")

								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == BUTTON_*) {
									registerGameParametersControl.setParameters(para);
									state = idle;
								}
								else {
									state = sendData;
								}
								break;
								
							default:break;
						}
				}
			}

};