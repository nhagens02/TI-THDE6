#ifndef SENDIRMESSAGECONTROL_HPP
#define SENDIRMESSAGECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "IRLed.hpp"
#include "StructData.hpp"
/// @file


/// \brief
/// SendIRMessageControl CLASS
/// \details
/// This class wil be used to send ir messsages with the boundary object: "IrLed".
/// In the main function will be waited until it received 16 bits(2bytes) to send it.
/// This class uses rtos::task<>.
class SendIRMessageControl : public rtos::task<> {
	enum state_t {idle, sending};

private:
	IRLed irLed;
	int index = 0;
	uint16_t information;
	state_t state = idle;
	rtos::channel< uint16_t, 64 > send2ByteChannel;
	void sendZero(){
		irLed.turnOn();
		hwlib::wait_us_busy(800);
		irLed.turnOff();
		hwlib::wait_us_busy(1600);
	}
	void sendOne() {
		irLed.turnOn();
		hwlib::wait_us_busy(1600);
		irLed.turnOff();
		hwlib::wait_us_busy(800);
	}
	void main() {
		for (;;) {
			switch (state)
			{
			case idle: {
				wait(send2ByteChannel);
				state = sending;
				break;
			}
			case sending: {
				//entry event
				information = send2ByteChannel.read();
				sendBytes(information);

				//other event
				state = idle;
				break;
			}
			default:break;
			}
		}
	}
public:
	SendIRMessageControl(hwlib::pin_out& ledPin) :
		task (6, "sendIrMessageControl"),
		irLed(ledPin),
		send2ByteChannel(this, "send2byteChannel")	
	{}

	void sendBytes(uint_fast16_t information) {
		for (int i = 16; i > 0; i--) {
			if (information & (1 << (i-1))) sendOne(); else { sendZero(); }
		}
	}
};

#endif // SENDIRMESSAGECONTROL_HPP
