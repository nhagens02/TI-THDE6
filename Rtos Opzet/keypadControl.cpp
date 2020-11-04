#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypad.hpp"
#include "InitGameControl.hpp"
#include "StructData.hpp"
/// @file


/// \brief
/// keypadControl CLASS
/// \details
/// This class is used to send the keypad presses to the RegisterGameParametersControl and InitGameControl.
/// This class uses the boundary object:"keypad".
/// This class only support a 4x4 matrix keypad.
/// This class uses rtos::task<>. 
class keypadControl : public rtos::task<>{
	enum state_t {idle,sendInput};

	private:
		state_t state = idle;
		Keypad keypad;
		int key = -1;
		InitGameControl& initGameControl;
		//registerGameParametersControl& registerGameParametersControl;
		rtos::clock intervalKeyCheck;
		

		int checkIfAnKeyIsPressed() {
			for (unsigned int i = 0; i <= 15; i++) {
				if (keypad.getKeyPressed(i)) {
					return i;
				}
			}
			return -1;
		}

	public:
	keypadControl(hwlib::pin_direct_from_oc_t* pinOut1, hwlib::pin_direct_from_oc_t* pinOut2, hwlib::pin_direct_from_oc_t* pinOut3, hwlib::pin_direct_from_oc_t* pinOut4, hwlib::pin_direct_from_in_t* pinIn1, hwlib::pin_direct_from_in_t* pinIn2, hwlib::pin_direct_from_in_t* pinIn3, hwlib::pin_direct_from_in_t* pinIn4, InitGameControl& initGameControl):
		task("keypad controller"),
		keypad(pinOut1, pinOut2, pinOut3, pinOut4, pinIn1, pinIn2, pinIn3, pinIn4),
		initGameControl ( initGameControl ),
		intervalKeyCheck(this, (200* rtos::ms), "keypad interval checker")
		//initGameControl(initGameControl)

	{}

	private:
		void main(){

			for(;;){
				switch(state)
				{
				case idle: {
					//entry events

					//other events
					wait(intervalKeyCheck);
					//wait(keypad.getKeyPressedChannel);
					//bnID = keypad.getKeyPressedChannel.read();
					key = checkIfAnKeyIsPressed();
					if (key == -1) {
						state = idle;
						break;
					}
					else {
						state = sendInput;
						break;
					}
					break;
				}
					case sendInput:
						//entry events
						hwlib::cout << "keytest: " << key << hwlib::endl;
						//RegisterGameParametersControl.buttonPressedChannel.write(bnID)
						initGameControl.buttonPressed(key);

						//other events
						state = idle;
						break;
						
					default:break;
				}
			}
		}

};