#include "hwlib.hpp"


void sendBit(bool bit, auto& ir) {
	if (bit == 1) {
		ir.write(1);
		ir.flush();
		hwlib::wait_us(800);
		ir.write(0);
		ir.flush();
		hwlib::wait_us(1600);
	}
	else {
		ir.write(0);
		ir.flush();
		hwlib::wait_us(800);
		ir.write(1);
		ir.flush();
		hwlib::wait_us(1600);
	}
	
}



int getBit(auto &tsop_signal, auto &ir) {
	int timing_low = 0;
	int timing_high = 0;
	tsop_signal.refresh();
	//hwlib::cout << tsop_signal.read();
	//while (tsop_signal.read() != 0) {
	//	tsop_signal.refresh();
		//sw.refresh();
		//if (!sw.read()) {
		//	sendBit(1, ir);
		//}
	//}
	//while (1) {
		if (tsop_signal.read() == 1) {
			for (int i = 0; i <= 24; i++) {
				tsop_signal.refresh();
				if (tsop_signal.read() == 1) {
					timing_high++;
				}
				else {
					timing_low++;
				}
				//if (timing_high >= ) {
				//
				//}
				hwlib::wait_us(50);
				
			}
			break;
		}
	}
	if ((timing_high == 16) && (timing_low == 32)) { return 0; }//0
	if ((timing_high == 32) && (timing_low == 16)) { return 1; }//1
	//hwlib::cout << "timing_low:" << timing_low << hwlib::endl;
	//hwlib::cout << "timing_high:" << timing_high << hwlib::endl;
	//hwlib::cout << hwlib::endl;
	return (timing_low + timing_high);
}



//uint_fast16_t GetReceivingBits() {
//	uint_fast16_t result = 0;
//	for (int i = 0; i <= 15; i++) {
	//	wait(bitsChannel);
	//	bool bit = bitsChannel.read();
//		result += (bit << i);
//	}
//	return result;
//}



int main() {
	hwlib::wait_ms(2000);
	auto ir = hwlib::target::d2_36kHz();

	namespace target = hwlib::target;

	auto tsop_signal = target::pin_in(target::pins::d8);
	auto tsop_gnd = target::pin_out(target::pins::d9);
	auto tsop_vdd = target::pin_out(target::pins::d10);
	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();
	//auto sw = hwlib::target::pin_in(hwlib::target::pins::d3);
	auto led = target::pin_out(target::pins::led);

	for (;;) {
		int result = 0;

		for (int i = 0; i < 16;i++) {
			result += getBit(tsop_signal,ir);
			//sw.refresh();
			//if (!sw.read()) {
			//	sendBit(1, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(0, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(1, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(1, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(1, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(0, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(0, ir);
			//	result += (getBit(tsop_signal, ir, sw) << i);
			//	sendBit(1, ir);
			}
		hwlib::cout << result << hwlib::endl;
		}
		
		//hwlib::wait_ms(2);
		
	}
	