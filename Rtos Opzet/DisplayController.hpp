#ifndef DISPLAYCONTROLLER_HPP
#define DISPLAYCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"


class DisplayController : public rtos::task<> {
	private:
		hwlib::pin_oc& scl;
		hwlib::pin_oc& sda;
		hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
		hwlib::glcd_oled oled;
		hwlib::part w1;
		hwlib::part w2;
		hwlib::font_default_16x16 f1;
		hwlib::terminal_from d1;
		hwlib::font_default_8x8 f2;
		hwlib::terminal_from d2;





	public:
		DisplayController(hwlib::pin_oc& scl, hwlib::pin_oc& sda):
			task("Display controller"),
			scl ( scl ),
			sda ( sda ),
			i2c_bus ( scl, sda ),
			oled ( i2c_bus, 0x3c ),
			w1 ( oled, hwlib::xy(0, 0), hwlib::xy(128, 32)) ),
			w2 ( oled, hwlib::xy(0, 32), hwlib::xy(128, 32)) ),
			f1 ( f1 ),
			d1 ( w1, f1 ),
			f2 ( f2 ),
			d2 ( w2, f2 )




};


#endif // DISPLAYCONTROLLER_HPP