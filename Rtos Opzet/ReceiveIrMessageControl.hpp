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
class ReceiveIrMessageControl : public rtos::task<>{
	enum state_t {idle,receivingBits};

	private:
		state_t state = idle;
		ReceiveIrByteToDataControl receiveIrByteToDataControl;
		rtos::channel<bool, 256> bitValueChannel;
		uint_fast16_t message;
		uint_fast16_t bitAmount;
public:
	ReceiveIrMessageControl()
	{}
	void receiveBit(bool bit) {
		message += (bit << (15 - bitAmount));
		bitAmount++;
		if (bitAmount == 16) {
			receiveIrByteToDataControl.receiveMessage(message);
		}
	}
	void sendBit(bool bit){ bitValueChannel.write(bit);}

	private:
		void main(){

			for(;;){
					switch(state)
					{

						case idle:
							//entry event
							
							//other events
							wait(bitValueChannel);
							state = receivingBits;
							break;

						case receivingBits:
							//entry events
							//receiveIrByteToDataControl.getMessage(GetReceivingBits());
							for (int i = 0; i < 17;i++) {
								receiveBit(bitValueChannel.read());
								wait(bitValueChannel);
							}
							state = idle;
							break;

						default:break;
					}
			}
		}

};

#endif // RECEIVEIRMESSAGECONTROL_HPP