#include "hwlib.hpp"

bool getbit(const uint8_t byte, const int index) {
	int x = 0;
	if (byte & (1 << index)) {
		x = 1;
	}
	return x;
}

//void writeBit(bool bit, auto& ir) {
//	if (bit) {
//
//	}
//}


void write_0(bool bit, auto& ir) {
	ir.write(1);
	ir.flush();
	hwlib::wait_us(560);
	
	ir.write(0);
	ir.flush();
	hwlib::wait_us(560);
}

void write_1(bool bit, auto& ir) {
	ir.write(1);
	ir.flush();
	hwlib::wait_us(560);
	ir.write(0);
	ir.flush();
	hwlib::wait_us(1600);
}


void writeByte(uint8_t byte, auto& ir) {
	ir.write(1);
	ir.flush();
	//uint_fast64_t now = hwlib::now_us();

	//for (;;) {
	//	if (hwlib::now_us() >= (now + 9000)) {
	//		break;
	//	}
	//}
	hwlib::wait_ms(9);
	ir.write(0);
	ir.flush();
	hwlib::wait_us(4500);
	for (int i = 0; i <= 8; i++) {
		if (getbit(byte, i)) {
			write_1(1, ir);
		}
		else {
			write_0(0, ir);
		}
	}
}

int main( void ) {
	auto ir = hwlib::target::d2_36kHz();

    auto sw = hwlib::target::pin_in(hwlib::target::pins::d4);


	auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
	auto tsop_gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto tsop_vdd = hwlib::target::pin_out(hwlib::target::pins::d10);
	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();

	auto led = hwlib::target::pin_out(hwlib::target::pins::led);

	auto const active = 100'000;
	auto last_signal = hwlib::now_us() - active;

	for (;;) {
		tsop_signal.refresh();
		if (tsop_signal.read() == 0) {
			last_signal = hwlib::now_us();
		}
		else {
			last_signal = hwlib::now_us();
			for (;;) {
				tsop_signal.refresh();
				if () {

				}
			}
		}
		//bool data[8] = {};
		tsop_signal.refresh();
		if (tsop_signal.read() == 1) {
			//hwlib::wait_ms(9);
			now = hwlib::now_us();
			while (!(hwlib::now_us() >= (now + 9000))) {
				tsop_signal.refresh();
				if (tsop_signal.read() == 0) {
					break;
				}
			}
			tsop_signal.refresh();
			if (tsop_signal.read() == 0) {
				hwlib::wait_ms(4000);
				if (tsop_signal.read() == 1) {

				}
			}
		}

		//led.write((last_signal + active) > hwlib::now_us());
		//led.flush();
		


		//hwlib::wait_ms(1);
		sw.refresh();
		if (!sw.read()) {
			writeByte(0b11010101, ir);
			hwlib::wait_ms(1000);
		}


		//sw.refresh();
		//ir.write(!sw.read());
		//ir.flush();

		//hwlib::wait_ms(1);
		//ir.write(0);
		//ir.flush();
	}
}