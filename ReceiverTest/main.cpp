#include "hwlib.hpp"

int read(hwlib::pin_in& tsop_signal) {
	tsop_signal.refresh();
	if (tsop_signal.read()) {
		hwlib::wait_us(1200);
		bool returnValue = tsop_signal.read();
		while (tsop_signal.read()) {}

		/*count+=1;*/

		return returnValue;
	}
	return -1;
}


int main() {
	namespace target = hwlib::target;

	auto _tsop_signal	= target::pin_in(target::pins::d8);
	auto tsop_signal 	= hwlib::invert(_tsop_signal);
	auto tsop_gnd    	= target::pin_out(target::pins::d9);
	auto tsop_vdd    	= target::pin_out(target::pins::d10);
   
	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();

	int bits[64] = {};
	int bitsSize = 0;

	uint_fast32_t timeSinceLastReceivedBit = hwlib::now_us();
	uint_fast32_t currentLoopTime = hwlib::now_us();

	hwlib::wait_ms(500);

	for (;;) {
		if (hwlib::now_us() > (timeSinceLastReceivedBit + 4000)) {
			if (bitsSize == 32) {
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[i];
				}
				hwlib::cout << "\n";
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[16+i];
				}
				hwlib::cout << "\n";
			}
			bitsSize = 0;
			timeSinceLastReceivedBit = hwlib::now_us();
		}

		if (hwlib::now_us() > (currentLoopTime + 100)) {
			int bitValue = read(tsop_signal);
			if (bitValue != -1) {
				
				bits[bitsSize] = bitValue;
				timeSinceLastReceivedBit = hwlib::now_us() + 1600 - (bits[bitsSize] * 800);
				bitsSize++;
			}
			
			
			currentLoopTime = hwlib::now_us();

			
			if (bitsSize == 64) {
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[i];
				}
				hwlib::cout << "\n";
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[16+i];
				}
				hwlib::cout << "\n";
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[32+i];
				}
				hwlib::cout << "\n";
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[48+i];
				}
				hwlib::cout << "\n";
				bitsSize = 0;
			}
		}
	}
}
