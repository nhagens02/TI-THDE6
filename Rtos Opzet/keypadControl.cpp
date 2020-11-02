#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// CLASS
/// \details
/// This class
/// This class uses rtos::task<>. 
class keypadControl : public rtos::task<>{
	enum state_t = {idle,sendInput};

	private:
		state_t = state = idle;
		InitGameControl& InitGameControl;
		registerGameParametersControl& registerGameParametersControl;

	keypadControl():

	{}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events

						//other events
						wait(keypad.getKeyPressedChannel);
						bnID = keypad.getKeyPressedChannel.read();
						state = sendInput;
						break;

					case sendInput:
						//entry events
						RegisterGameParametersControl.buttonPressedChannel.write(bnID)
						InitGameControl.buttonPressedChannel.write(bnID)

						//other events
						state = idle;
						break;
						
					default:break;
				}
			}
		}

};