#include "hwlib.hpp"
#include "rtos.hpp"
#include "IRLed.cpp"
/// @file


/// \brief
/// SendIRMessageControl CLASS
/// \details
/// This class wil be used to send ir messsages with the boundary object: "IrLed".
/// In the main function will be waited until it received 16 bits(2bytes) to send it.
/// This class uses rtos::task<>.
class SendIRMessageControl : public rtos::task<> {
private:
	IRLed irLed;
	void sendZero(){
		uint_fast32_t currentTime = hwlib::now_us();
		irLed.turnOn();
		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
		}

		currentTime = hwlib::now_us();
		irLed.turnOff();

		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
		}
	}
	void sendOne() {
		uint_fast32_t currentTime = hwlib::now_us();
		irLed.turnOn();

		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
		}

		currentTime = hwlib::now_us();
		irLed.turnOff();

		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
		}
	}
public:
	SendIRMessageControl(hwlib::pin_out& ledPin) :
		irLed(ledPin)
	{}

	void sendBytes(uint16_t information) {
		for (int i = 16; i > 0; i--) {
			if (information & (1 << (i-1))) sendOne(); else { sendZero(); }
		}
	}
};

#endif // SENDIRMESSAGECONTROL_HPP