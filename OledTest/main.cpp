#include "hwlib.hpp"

void filledScreenPercent(hwlib::window& w, int percent) {
	hwlib::xy window_size = w.size;
	hwlib::cout << window_size.x << " " << window_size.y << "\n";
	float rectangleWidthFloat = ((window_size.x / 100) * percent);
	int rectangleWidth = rectangleWidthFloat;
	hwlib::cout << rectangleWidth << "\n";
	hwlib::xy rectangleEnd(rectangleWidth, window_size.y);
	for (int y = 0; y < rectangleEnd.y; y++) {
		for (int x = 0; x < rectangleEnd.x; x++) {
			w.write(hwlib::xy(x, y));
		}
	}
	w.flush();
}

int main() {
	auto oledVcc = hwlib::target::pin_out(hwlib::target::pins::d13);
	auto scl = hwlib::target::pin_oc(hwlib::target::pins::d12);
	auto sda = hwlib::target::pin_oc(hwlib::target::pins::d11);

	oledVcc.write(1);
	oledVcc.flush();

	auto i2c = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
	auto oled = hwlib::glcd_oled_i2c_128x64_fast_buffered(i2c);
	oled.clear();
	oled.flush();

	auto button_ = hwlib::target::pin_in(hwlib::target::pins::d44);
	auto button = hwlib::invert(button_);

	auto cooldownDisplay = hwlib::part(oled, hwlib::xy(0, 0), hwlib::xy(64, 16));
	cooldownDisplay.clear();
	for (;;) {
		if (button.read()) {
			//for (int i = 0; i < 100; i++) {
				filledScreenPercent(oled, 100);
				cooldownDisplay.flush();
				oled.flush();
			//}
		}
	}


}