#include "hwlib.hpp"

// Ouput the 38KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void IRcarrier(unsigned int IRtimemicroseconds)
{
	auto ir = hwlib::target::d2_36kHz();
	
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

void testIR() {
	auto ir = hwlib::target::d2_36kHz();
	
	ir.write(true);
	ir.flush();
	
	hwlib::wait_us(50);
	
	ir.write(false);
	ir.flush();
	
	hwlib::wait_us(50);
	
	ir.write(true);
	ir.flush();
	
	hwlib::wait_us(50);
	
	ir.write(false);
	ir.flush();
	
}

void detectButtonPress() {
	auto sw = hwlib::target::pin_in(hwlib::target::pins::d43);
	
	while(true) {
		static bool lastBtnState = false;
		
		sw.refresh();
		bool state = sw.read();
		
		if (state != lastBtnState) {
			lastBtnState = state;
			if (state) {
				IRcarrier(9000);
				hwlib::wait_us(4500);
				
				// broeder
				
				hwlib::cout << "Button pressed!\n";
			}
		}
	}
}

int main(void){
	hwlib::wait_ms(500);
	detectButtonPress();
}
