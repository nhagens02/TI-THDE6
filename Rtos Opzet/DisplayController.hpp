#ifndef DISPLAYCONTROLLER_HPP
#define DISPLAYCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include <string>


class DisplayController : public rtos::task<> {
	enum state_t {idle, };
	private:
		hwlib::pin_oc& scl;
		hwlib::pin_oc& sda;
		hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
		hwlib::glcd_oled oled;
		hwlib::window_part_t w1;
		hwlib::window_part_t w2;
		hwlib::font_default_16x16 f1;
		hwlib::terminal_from d1;
		hwlib::font_default_8x8 f2;
		hwlib::terminal_from d2;
		rtos::pool< int > time;
		rtos::flag timeFlag;

	public:
		DisplayController(hwlib::pin_oc& scl, hwlib::pin_oc& sda):
			task("Display controller"),
			scl(scl),
			sda(sda),
			i2c_bus(scl, sda),
			oled(i2c_bus, 0x3c),
			w1(oled, hwlib::xy(0, 0), hwlib::xy(128, 32)),
			w2(oled, hwlib::xy(0, 32), hwlib::xy(128, 32)),
			d1(w1, f1),
			d2(w2, f2),
			time( "display Time Pool" ),
			timeFlag(this, "display time Flag")
		{
			w1.clear();
			w2.clear();
		}

		void showTime(const int & seconds){
			//hwlib::cout << "t" << hwlib::endl;
			d1 << "\f" << "T:" << seconds;
			//oled.flush();
		}

		//void showMessage(std::string str) {
			//d2 << "\f"
			//	<< "==============\n"
			//	<< "test\n"
			//	<< "score: " << 42 << "\n"
			//	<< "==============\n";
			//oled.flush();
	//	}


		void main() {
			for (;;) {
				hwlib::wait_ms(3000);
				showTime(50);
		//		showMessage("test");
				oled.flush();
			}
		}
};


#endif // DISPLAYCONTROLLER_HPP