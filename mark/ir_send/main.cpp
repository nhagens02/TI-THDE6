#include "hwlib.hpp"

// Ouput the 38KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void IRcarrier(unsigned int IRtimemicroseconds)
{
	auto ir = hwlib::target::pin_out(hwlib::target::pins::d2);
	
	for(unsigned int i=0; i < (IRtimemicroseconds / 26); i++)
	{
		ir.write(true);				//turn on the IR LED
		ir.flush();
		
		//NOTE: hwlib::pin_out::write takes about 3.5us to execute, so we need to factor that into the timing.
		
		hwlib::wait_us(9);       	//delay for 13us (9us + hwlib::pin_out::write), half the carrier frequnecy
		
		ir.write(false);			//turn off the IR LED
		ir.flush();
		
		hwlib::wait_us(9);       	//delay for 13us (9us + hwlib::pin_out::write), half the carrier frequnecy
	}
}

void sendIRMessage(uint16_t code) {
	for(uint8_t i = 0; i < 16; i++) {
		if(code & (1 << ((sizeof(uint16_t) * 8) - 1))) {
			IRcarrier(1600);
			hwlib::wait_us(800);
		}
		else {
			IRcarrier(800);
			hwlib::wait_us(1600);
		}
		code <<= 1;
	}
}

void detectButtonPress() {
	auto sw_ = hwlib::target::pin_in(hwlib::target::pins::d43);
	auto sw = hwlib::invert(sw_);
	bool lastBtnState = false;

	while(true) {
		sw.refresh();
		bool state = sw.read();
		
		if (state != lastBtnState) {
			lastBtnState = state;
			if (!state) {
				//uint16_t code = 0b1010101010101010;
				auto start = hwlib::now_us();
				sendIRMessage(code);
				auto stop = hwlib::now_us();
				hwlib::cout << "IR Packet sent in " << (stop - start) << " microseconds!\n";
			}
		}
	}
}

int main(void){
	hwlib::wait_ms(500);
	detectButtonPress();
}
