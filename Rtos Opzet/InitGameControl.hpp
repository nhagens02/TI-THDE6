#ifndef INITGAMECONTROL_HPP
#define INITGAMECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"
#include "DataToIrByteControl.hpp"
#include "DisplayController.hpp"
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
		DisplayController& displayControl;

	


	public: //registerGameparametersControl& registerGameparametersControl, keypadControl& KeypadControl,
		InitGameControl(DataToIrbyteControl& dataToIrByteControl, DisplayController& displayControl):
			task("init game controller"),
			buttonChannel(this, "button press Channel"),
			//registerGameparametersControl(registerGameparametersControl),
			//dataToIRByteControl(dataToIRByteControl),
			//KeypadControl( KeypadControl ),
			dataToIrByteControl(dataToIrByteControl),
			displayControl( displayControl )
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
								//displayControl.showMessage("\fcommand\nEnter play time: ");
								//displayControl.showMessage("");
								para.gameTime = 0;
								para.gameMode = 0;
								para.timeUntilStart = 0;
								state = enterPlayTime;
								break;

							case enterPlayTime:
								//entry events
								displayControl.showMessage("\fcommand\nEnter play time:\n");
								displayControl.showMessage(para.gameTime);
								//hwlib::cout << "playtimeTest" << hwlib::endl;
								//hwlib::cout << "Time: " << para.gameTime << hwlib::endl;
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
								displayControl.showMessage("\fcommand\nEnter game Mode:\n");
								displayControl.showMessage(para.gameMode);
								//hwlib::cout << "GameMode" << hwlib::endl;
								//hwlib::cout << "gameMode: " << para.gameMode << hwlib::endl;
								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								//hwlib::cout << bnID << hwlib::endl;
								if (bnID == 15) { //15 = #
									if ((para.gameMode >= 0) && (para.gameMode <= 1)) {
										state = SetTimeUntilStart;
										break;
									}
									else {
										state = init;
										break;
									}
								}
								else if ((bnID >= 0) || (bnID <= 1)) {
									para.gameMode = bnID;
									state = setGameMode;
									break;
								}
								else {
									state = setGameMode;
									break;
								}
								break;

							case SetTimeUntilStart:
								//entry events
								displayControl.showMessage("\fcommand\nEnter time until\nstart:");
								displayControl.showMessage(para.timeUntilStart);
								//displayControl.showMessage("");
								//hwlib::cout << "Time until start" << hwlib::endl;
								//hwlib::cout << "time: " << para.timeUntilStart << hwlib::endl;
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
								else if ((bnID >= 0) || (bnID <= 9)) {
									para.timeUntilStart = (para.timeUntilStart * 10);
									para.timeUntilStart += bnID;
									state = SetTimeUntilStart;
									break;
								}
								else {
									state = SetTimeUntilStart;
									break;
								}
								break;

							case sendData:
								//entry events
								displayControl.showMessage("\fPress * to send para\nPress # to go to\nStart.\n");
								//other events
								wait(buttonChannel);
								bnID = buttonChannel.read();
								if (bnID == 15) { // # = 15
									//registerGameparametersControl.setparameters(para);
									state = idle;
									break;
								}
								else if (bnID == 14) { // * = 14
									dataToIrByteControl.sendingGameParametersChannel(para);
									state = sendData;
									break;
								}
								else {
									state = sendData;
									break;
								}
								break;
								
							default:break;
						}
				}
			}

};


#endif // INITGAMECONTROL_HPP