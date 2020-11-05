#ifndef DISPLAYCONTROLLER_HPP
#define DISPLAYCONTROLLER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include <string>

/// \brief
/// DisplayController CLASS
/// \details
/// This class will show the right information on the screen. 
/// This class uses rtos::task<>, hwlib::pin_oc, hwlib::i2c_bus_bit_banged_scl_sda i2c_bus,
/// hwlib::glcd_oled, hwlib::window_part_t, hwlib::font_default_16x16, hwlib::terminal_from, 
/// hwlib::font_default_8x8, hwlib::terminal_from, rtos::pool, rtos::flag, hwlib::wait_ms. 
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
			d1 << "\f" << "Time:" << seconds;
			//oled.flush();
			oled.flush();
		}

		void showMessage(auto str) {
			d2 << str;
				//<< "\f"
				//<< "==============\n"
				//<< str
				//<< "score: " << 42 << "\n"
				//<< "==============\n";
			//oled.flush();
			oled.flush();
		}


		void main() {
			//auto test = "test";
			for (;;) {
				hwlib::wait_ms(4000);
				//showTime(2);
				//showMessage(test);
		//		showMessage("test");
				
			}
		}
};


#endif // DISPLAYCONTROLLER_HPP
