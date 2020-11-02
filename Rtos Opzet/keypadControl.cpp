#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// keypadControl CLASS
/// \details
/// This class is used to send the keypad presses to the RegisterGameParametersControl and InitGameControl.
/// This class uses the boundary object:"keypad".
/// This class only support a 4x4 matrix keypad.
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