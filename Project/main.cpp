#include "hwlib.hpp"
#include "rtos.hpp"

class LightControl : public rtos::task<> {
	private:
		void main() {
			auto led = hwlib::target::pin_out(hwlib::target::pins::d2);
			while(true) {
				led.write(true);
				led.flush();
				hwlib::wait_ms(1000);
				led.write(false);
				led.flush();
				hwlib::wait_ms(1000);
			}
		}
};

class ButtonControl : public rtos::task<> {
	private:
		void main() {
			auto button = hwlib::target::pin_in(hwlib::target::pins::d3);
			auto led = hwlib::target::pin_out(hwlib::target::pins::d2);
			while(true) {
				button.refresh();
				if(!button.read()) {
					led.write(true);
					led.flush();
					hwlib::cout << "ON!\n";
				}
				else {
					led.write(false);
					led.flush();
					hwlib::cout << "OFF!\n";
				}
			}
		}
};

void singleButtonPress() {
	auto button = hwlib::target::pin_in(hwlib::target::pins::d3);

	button.refresh();
	bool buttonPrevState = button.read();

	while(true) {
		button.refresh();
		bool currentState = button.read();
		
		if(currentState != buttonPrevState) {
			if(!currentState) {
				hwlib::cout << "test" << hwlib::endl;
			}
		}
		buttonPrevState = currentState;
	}
}

int main() {
	hwlib::wait_ms(500);
	
	//auto blinkLED = LightControl();
	//auto readButton = ButtonControl();
	//rtos::run();
	
	singleButtonPress();
	
	return 0;
}
