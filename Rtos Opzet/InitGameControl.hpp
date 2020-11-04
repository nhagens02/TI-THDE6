#ifndef INITGAMECONTROL_HPP
#define INITGAMECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "DataToIrByteControl.hpp"
/// @file


/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// all variables are of the type int. 
//struct parameters{
//	int gameMode,
//	int gameTime,
//	int timeUntilStart;
//};


/// \brief
/// Enum buttons keypad.
/// \details
/// This enum contains all possable buttons from the keypad to be pressed.
//enum eButtonID = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5,BUTTON_6,BUTTON_7,BUTTON_8,BUTTON_9,BUTTON_0,BUTTON_*,BUTTON_#,BUTTON_A,BUTTON_B,BUTTON_C,BUTTON_D};


/// \brief
/// InitGameControl CLASS
/// \details
/// This class will be used by the game leader. 
/// This class will be used to set the game parameters. 
/// It set the gamemode, gameTime and Time until the game start. 
/// This class uses rtos::task<>. 
class InitGameControl : public rtos::task<>{
	enum state_t {idle, init, enterPlayTime, setGameMode, SetTimeUntilStart, sendData};

	private:
		state_t state = idle;
		struct parameters para;
		int bnID;
		rtos::channel< int, 1024 > buttonChannel;
		//keypadControl& KeypadControl;
		//registerGameparametersControl& registerGameparametersControl;
		DataToIrbyteControl& dataToIrByteControl;

	


	public: //registerGameparametersControl& registerGameparametersControl, keypadControl& KeypadControl,
		InitGameControl(DataToIrbyteControl& dataToIrByteControl):
			task("init game controller"),
			buttonChannel(this, "button press Channel"),
			//registerGameparametersControl(registerGameparametersControl),
			//dataToIRByteControl(dataToIRByteControl),
			//KeypadControl( KeypadControl ),
			dataToIrByteControl(dataToIrByteControl)
		{}

		void buttonPressed(int buttonID){buttonChannel.write(buttonID);}

	private:
			void main(){

				for(;;){
						switch(state)
						{
							case idle:
								//entry events

								//other events
								//check deze state nog ff.
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == 12) { //bnID =12 = BUTTON C
									state = init;
									break;
								}
								else {
									state = idle;
									break;
								}
								break;

							case init:
								//displayControl.showMessage("command");
								//displayControl.showMessage("Enter play time:");
								para.gameTime = 0;
								para.gameMode = 0;
								para.timeUntilStart = 0;
								state = enterPlayTime;
								break;

							case enterPlayTime:
								//entry events
								//displayControl.showMessage("command");
								//displayControl.showMessage("Enter play time:")
								hwlib::cout << "playtimeTest" << hwlib::endl;
								hwlib::cout << "Time: " << para.gameTime << hwlib::endl;
								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == 15) { //15 = #
									if ((para.gameTime >= 1) && (para.gameTime <= 15)) {
										state = setGameMode;
										break;
									}
									else {
										state = init;
										break;
									}
								}
								else if (bnID == 0) {

								}
								else if ((bnID >=1) && (bnID <=9)) {
									para.gameTime += bnID;
								}
								else {
									state = enterPlayTime;
									break;
								}
								break;

							case setGameMode:
								//entry events
								//displayControl.showMessage("command");
								//displayControl.showMessage("Enter game Mode:");
								hwlib::cout << "GameMode" << hwlib::endl;
								hwlib::cout << "gameMode: " << para.gameMode << hwlib::endl;
								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								hwlib::cout << bnID << hwlib::endl;
								if (bnID == 15) { //15 = #
									if ((para.gameMode >= 1) && (para.gameMode <= 2)) {
										state = SetTimeUntilStart;
										break;
									}
									else {
										state = init;
										break;
									}
								}
								else if ((bnID >= 1) || (bnID <= 2)) {
									para.gameTime = bnID;
									state = SetTimeUntilStart;
									break;
								}
								else {
									state = setGameMode;
									break;
								}
								break;

							case SetTimeUntilStart:
								//entry events
								//displayControl.showMessage("command");
								//displayControl.showMessage("Enter time until start of game:");
								hwlib::cout << "Time until start" << hwlib::endl;
								hwlib::cout << "time: " << para.timeUntilStart << hwlib::endl;
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == 15) { //15 = #
									if ((para.timeUntilStart >= 1) && (para.timeUntilStart <= 31)) {
										state = sendData;
										break;
									}
									else {
										state = init;
										break;
									}	
								}
								//else if () {

								//}
								else {
									state = SetTimeUntilStart;
									break;
								}
								break;

							case sendData:
								//entry events
								dataToIrByteControl.sendingGameParametersfun(para);
								//displayControl.showmessage("Press any button exept the * button to send parameters, press the * to go to start.")

								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID != 14) { // * = 14
									//registerGameparametersControl.setparameters(para);
									state = sendData;
									break;
								}
								else {
									state = idle;
									break;
								}
								break;
								
							default:break;
						}
				}
			}

};


#endif // INITGAMECONTROL_HPP