#include "hwlib.hpp"

enum pulseDurations {
	shortPulseMinimumDuration = 400,
	shortPulseMaximumDuration = 1200,
	longPulseMinimumDuration = 1200,
	longPulseMaximumDuration = 2900
};

//int read(hwlib::pin_in& tsop_signal) {
//	tsop_signal.refresh();
//	if (tsop_signal.read()) {
//		// 1 puls is ontvangen
//		hwlib::wait_us(792);
//		bool bitValue = tsop_signal.read();
//		hwlib::wait_us(792);
//		if (tsop_signal.read()) {
//			
//			return -1;
//		}
//		return bitValue;
//	}
//	else {
//		return -1;
//	}
//}

int read(hwlib::pin_in& tsop_signal) {
	auto pin = hwlib::target::pin_out(hwlib::target::pins::d3);
	tsop_signal.refresh();
	if (tsop_signal.read()) {
		pin.write(1);
		pin.flush();
		uint_fast32_t beforeTime = hwlib::now_us();
		while (tsop_signal.read()) {}
		uint_fast32_t pulseDuration = hwlib::now_us() - beforeTime;
		pin.write(0);
		pin.flush();
		if (((pulseDuration) > shortPulseMinimumDuration) && ((pulseDuration) < shortPulseMaximumDuration)) {
			// short pulse detected
			return 0;
		}
		else if (((pulseDuration) > longPulseMinimumDuration) && ((pulseDuration) < longPulseMaximumDuration)) {
			// long pulse detected
			return 1;
		}
	}
	else {
		return -1;
	}
}

int main(void) {
	hwlib::wait_ms(2000);
	namespace target = hwlib::target;

	auto tsop_signal_ = target::pin_in(target::pins::d2);
	auto tsop_signal = hwlib::invert(tsop_signal_);
	auto tsop_vdd = target::pin_out(target::pins::d53);
	tsop_vdd.write(1);
	tsop_vdd.flush();

	int bits[16] = {};
	int bitsSize = 0;
	uint_fast32_t timeSinceLastReceivedBit = hwlib::now_us();
	uint_fast32_t currentLoopTime = hwlib::now_us();


	for (;;) {
		if (hwlib::now_us() > (timeSinceLastReceivedBit + 4000)) {
			bitsSize = 0;
			timeSinceLastReceivedBit = hwlib::now_us();
		}

		if (hwlib::now_us() > (currentLoopTime + 100)) {
			int bitValue = read(tsop_signal);
			if (bitValue != -1) {
				bits[bitsSize] = bitValue;
				bitsSize++;
				timeSinceLastReceivedBit = hwlib::now_us();
			}
			else {
				bitsSize = 0;
			}
			currentLoopTime = hwlib::now_us();
			if (bitsSize == 16) {
				for (int i = 0; i < 16; i++) {
					hwlib::cout << bits[i];
				}
				hwlib::cout << "\n";
				bitsSize = 0;
			}
		}
	}
}
