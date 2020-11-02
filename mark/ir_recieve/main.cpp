#include "hwlib.hpp"

int main(void) {
	namespace target = hwlib::target;
   
	auto _tsop_signal	= target::pin_in(target::pins::d8);
	auto tsop_signal 	= invert(_tsop_signal);
	auto tsop_gnd    	= target::pin_out(target::pins::d9);
	auto tsop_vdd    	= target::pin_out(target::pins::d10);
	auto led         	= target::pin_out(target::pins::led);
   
	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();
	
	hwlib::wait_ms(100);
	
	auto sw = hwlib::invert(tsop_signal);
	bool lastBtnState = false;

	while(true) {
		sw.refresh();
		bool state = sw.read();
		
		if (state != lastBtnState) {
			if (state) {
				led.write(false);
				led.flush();
			} else {led.write(true); led.flush();}
			lastBtnState = state;
		}
	}
}
