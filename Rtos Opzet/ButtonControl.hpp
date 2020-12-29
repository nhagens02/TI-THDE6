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
		hwlib::pin_in& but_1;// shoot button 
		hwlib::pin_in& but_2;// reload button
		
	public:
		ButtonControl(RunGameControl& runGameControl, hwlib::pin_in& but_1, hwlib::pin_in& but_2):
			task("Button controller"),
			intervalKeyCheck(this, (340 * rtos::ms), "Button interval checker"),
			runGameControl(runGameControl),
			but_1(but_1),
			but_2(but_2)
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
						but_1.refresh();
						if (!(but_1.read())) {
							eButtonID b = triggerButton;
							runGameControl.buttonPressed(b);
							state = idle;
							break;
						}
						but_2.refresh();
						if (!(but_2.read())) {
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