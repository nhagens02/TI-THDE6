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
	state_t state = idle;
	rtos::channel< uint16_t, 1024 > send2ByteChannel;
	void sendZero(){
		uint_fast32_t currentTime = hwlib::now_us();
		irLed.turnOn();
		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
			hwlib::wait_ms(0);
		}

		currentTime = hwlib::now_us();
		irLed.turnOff();

		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
			hwlib::wait_ms(0);
		}
	}
	void sendOne() {
		uint_fast32_t currentTime = hwlib::now_us();
		irLed.turnOn();

		while ((currentTime + 1600) > (hwlib::now_us())) {
			//do nothing
			hwlib::wait_ms(0);
		}

		currentTime = hwlib::now_us();
		irLed.turnOff();

		while ((currentTime + 800) > (hwlib::now_us())) {
			//do nothing
			hwlib::wait_ms(0);
		}
	}
	void main() {
		for (;;) {
			switch (state)
			{
			case idle: {
				//entry event
		
				//other event
				wait(send2ByteChannel);
				state = sending;
				break;
			}
			case sending:
				//entry event
				auto information = send2ByteChannel.read();
				
				sendBytes(information);

				//other event
				state = idle;
				break;

			//default:break;
			}
		}
	}
public:
	SendIRMessageControl(hwlib::pin_out& ledPin) :
		task ("Irled"),
		irLed(ledPin),
		send2ByteChannel(this, "send2byteChannel")
		
	{}

	void sendBytes(uint16_t information) {
		//hwlib::cout << "i am here" << hwlib::endl;
		for (int i = 16; i > 0; i--) {
			if (information & (1 << (i-1))) sendOne(); else { sendZero(); }
		}
	}
};

#endif // SENDIRMESSAGECONTROL_HPP
