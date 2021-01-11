#ifndef RECEIVEIRMESSAGECONTROL_HPP
#define RECEIVEIRMESSAGECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "BitDetector.hpp"
#include "ReceiveIrByteToDataControl.hpp"
/// @file


/// \brief
/// ReceiveIrMessageControl CLASS
/// \details
/// This class will use the class: "IrReceiver", to receive the 16 bits of data.
/// The class uses the THDE IR protocol. 
/// This class uses rtos::task<>. 
class ReceiveIrMessageControl : public rtos::task<> {
	enum state_t {idle,receivingBits};

	private:
		state_t state = idle;
		rtos::channel<bool, 128> bitValueChannel;
		ReceiveIrByteToDataControl& receiveIrByteToDataControl;
		uint_fast16_t message = 0;
		uint_fast16_t bitAmount = 0;
		rtos::timer exitTimer;
public:
	ReceiveIrMessageControl(ReceiveIrByteToDataControl& receiveIrByteToDataControl):
		task (5,"ReceiveIrMessageControl" ),
		bitValueChannel ( this, "Bits channel" ),
		receiveIrByteToDataControl ( receiveIrByteToDataControl ),
		exitTimer(this, "exit transmission timer")
		{}

	void receiveBit(bool bit) {
		message += (bit << (15 - bitAmount));
		bitAmount++;
		if (bitAmount == 16) {
			receiveIrByteToDataControl.receiveMessage(message);
			//Reset to 0 for new transmission.
			bitAmount = 0;
			message = 0;
		}
	}
	void sendBit(bool bit){ bitValueChannel.write(bit);}

	private:
		void main(){
			for(;;){
					switch(state)
					{
						case idle:
							wait(bitValueChannel);
							state = receivingBits;
							break;
						case receivingBits:
							//entry events
							for (int i = 0; i < 16;i++) {
								receiveBit(bitValueChannel.read());
								exitTimer.set(4 * rtos::ms);
								auto event = wait( bitValueChannel + exitTimer );
								if (event == exitTimer) {
									message = 0;
									bitAmount = 0;
									state = idle;
									break;
								}
							}
							state = idle;
							break;
						default:break;
					}
			}
		}
};

#endif // RECEIVEIRMESSAGECONTROL_HPP