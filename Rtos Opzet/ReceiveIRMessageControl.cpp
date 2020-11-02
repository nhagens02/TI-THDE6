#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// ReceiveIrMessageControl CLASS
/// \details
/// This class will use the class: "IrReceiver", to receive the 16 bits of data.
/// The class uses the THDE IR protocol. 
/// This class uses rtos::task<>. 
class ReceiveIrMessageControl : public rtos::task<>{
	enum state_t ={idle,receivingBits};

	private:
		rtos::channel bitsChannel;
		uint16_t GetReceivingBits(){
			uint16_t result = 0;
			for (int i=0; i<=15; i++) {
				wait(bitsChannel);
				bool bit = bitsChannel.read();
				result += (bit << i);
			}
			return result;
		}

	ReceiveIrMessageControl():

	{}


	public:
		void sendBit(bool bit){bitsChannel.write(bit bool);}
		void update(){updateFlag.set();}

	private:
		void main(){

			for(;;){
					switch(state)
					{

						case idle:
							//entry event
							
							//other events
							wait(updateFlag);
							state = receivingBits;
							break;


						case receivingBits:
							//entry events
							receiveIrByteToDataControl.getMessage(GetReceivingBits());
							state = idle;
							break;


						default:break;
					}
			}
		}

};