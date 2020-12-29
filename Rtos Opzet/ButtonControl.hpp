#ifndef BUTTONCONTROL_HPP
#define BUTTONCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "StructData.hpp"

#include "RunGameControl.hpp"


class ButtonControl : public rtos::task<> {
	enum state_t {idle, checkAndSendButton};

	private:
		state_t state = idle;
		rtos::clock intervalKeyCheck;
		RunGameControl& runGameControl;
		hwlib::pin_in& triggerButtonPin;// shoot button 
		hwlib::pin_in& reloadButtonPin; // reload button
		
	public:
		ButtonControl(RunGameControl& runGameControl, hwlib::pin_in& triggerButtonPin, hwlib::pin_in& reloadButtonPin):
			task("Button controller"),
			intervalKeyCheck(this, (340 * rtos::ms), "Button interval checker"),
			runGameControl(runGameControl),
			triggerButtonPin(triggerButtonPin),
			reloadButtonPin(reloadButtonPin)
		{}

	private:
		void main() {

			for (;;) {
				switch (state)
				{
					case idle:
						//entry events

						//other events
						wait(intervalKeyCheck);
						state = checkAndSendButton;
						break;

					case checkAndSendButton:
						//entry events
						triggerButtonPin.refresh();
						if (!(triggerButtonPin.read())) {
							eButtonID b = triggerButton;
							runGameControl.buttonPressed(b);
							state = idle;
							break;
						}
						reloadButtonPin.refresh();
						if (!(reloadButtonPin.read())) {
							eButtonID b = reloadButton;
							runGameControl.buttonPressed(b);
							state = idle;
							break;
						}
						//other events

						state = idle;
						break;

					default:break;
				}
			}
		}


};



#endif // BUTTONCONTROL_HPP