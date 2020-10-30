#include "hwlib.hpp"

int read(hwlib::pin_in& tsop_signal) {
	tsop_signal.refresh();
	if (tsop_signal.read()) {
		// 1 puls is ontvangen
		hwlib::wait_us(792);
		bool bitValue = tsop_signal.read();
		hwlib::wait_us(792);
		if (tsop_signal.read()) {
			return -1;
		}
		return bitValue;
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
		if ((bitsSize != 0) && (timeSinceLastReceivedBit < hwlib::now_us() - 4000)) {
			bitsSize = 0;
		}

		//hwlib::wait_us(790);
		if ((hwlib::now_us() + 800) >= currentLoopTime) {
			int bitValue = read(tsop_signal);
			if (bitValue != -1) {
				bits[bitsSize] = bitValue;
				bitsSize++;
				timeSinceLastReceivedBit = hwlib::now_us();
			}
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
