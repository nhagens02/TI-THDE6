#include "hwlib.hpp"

void getBit(auto tsop_signal) {
	int timing_low = 0;
	int timing_high = 0;
	tsop_signal.refresh();
	while (tsop_signal.read() != 0) {
		tsop_signal.refresh();
	}
	if (tsop_signal.read() == 1) {
		for (int = 0; i <= 24; i++) {
			tsop_signal.refresh();
			if (tsop_signal.read() == 1) {
				timing_high++;
			}
			else {
				timing_low++;
			}
			hwlib::wait_us(100);
		}
	}
	hwlib::cout << "timing_low:" << timing_low << hwlib::endl;
	hwlib::cout << "timing_high:" << timing_high << hwlib::endl;
}



uint16_t GetReceivingBits() {
	uint16_t result = 0;
	for (int i = 0; i <= 15; i++) {
		wait(bitsChannel);
		bool bit = bitsChannel.read();
		result += (bit << i);
	}
	return result;
}



int main() {
	auto ir = hwlib::target::d2_36kHz();

	namespace target = hwlib::target;

	auto tsop_signal = target::pin_in(target::pins::d8);
	auto tsop_gnd = target::pin_out(target::pins::d9);
	auto tsop_vdd = target::pin_out(target::pins::d10);
	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();

	auto led = target::pin_out(target::pins::led);

	for (;;) {
		int result = 0;
		for (int i = 0; i < 16;i++) {
			result += (getBit(tsop_signal) << i);
		}
		hwlib::cout << result << hwlib::end;
		hwlib::wait_ms(2);
		
	}
	
}