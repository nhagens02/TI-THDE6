#include "hwlib.hpp"

int main(void){
	namespace target = hwlib::target;
	
	auto lcd_gnd    	= target::pin_out(target::pins::d8);
	auto lcd_vdd    	= target::pin_out(target::pins::d9);
	auto lcd_scl    	= target::pin_oc(target::pins::d10);
	auto lcd_sda    	= target::pin_oc(target::pins::d11);
	
	lcd_gnd.write(0);
	lcd_vdd.write(1);
	
	lcd_gnd.flush();
	lcd_vdd.flush();
	
	hwlib::wait_ms(500);
	
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(lcd_scl, lcd_sda);
	auto oled = hwlib::glcd_oled(i2c_bus, 0x3C);

	auto w1 = hwlib::part(oled, hwlib::xy(0, 0), hwlib::xy(128, 16));
	auto w2 = hwlib::part(oled, hwlib::xy(0, 16), hwlib::xy(128, 48));
	
	auto f1 = hwlib::font_default_16x16();
	auto d1 = hwlib::terminal_from(w1, f1);

	auto f2 = hwlib::font_default_8x8();
	auto d2 = hwlib::terminal_from(w2, f2);

	oled.clear();

	d1 << "Darren";
	d2 << "is super cute";

	oled.flush();

	/*
	auto t1 = hwlib::now_us(); 
	oled.flush();
	auto t2 = hwlib::now_us();
	HWLIB_TRACE << "oled.fluhs() took " << t2 - t1 << " us";

	int n = 0;   
	for(;;){
		hwlib::wait_ms( 1000 );
		d1 << "\f" << ++n;
		d2 << "\t0502" << ": " << n + 42;
		oled.flush();
	}
	*/
}
