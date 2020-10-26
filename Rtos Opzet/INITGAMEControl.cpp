#include "hwlib.hpp"
#include "rtos.hpp"

enum eButtonID = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5,BUTTON_6,BUTTON_7,BUTTON_8,BUTTON_9,BUTTON_0,BUTTON_*,BUTTON_#,BUTTON_A,BUTTON_B,BUTTON_C,BUTTON_D};

class INITGAMEControl : public rtos::task<>{
	enum state_t = {idle,init,enterPlayTime,setGameMode,SetTimeUntilStart,sendData};

	private:
		state_t state = idle;
		registerGameParametersControl& registerGameParametersControl;
		dataToIRByteControl& dataToIRByteControl;

	INITGAMEControl(registerGameParametersControl& registerGameParametersControl,dataToIRByteControl& dataToIRByteControl):

	{buttonHandler.addButton();}


	public:
		void buttonPressed(eButtonID buttonID){buttonChannel.write(buttonID);}

	private:

};