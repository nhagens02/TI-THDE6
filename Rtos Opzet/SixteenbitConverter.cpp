#include "hwlib.hpp"
#include "rtos.hpp"

class sixteenBitConverter : public rtos::task<>{
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

	sixteenBitConverter():

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
							receiveIrMessageControl.sendByte(GetReceivingBits());
							state = idle;
							break;


						default:break;
					}
			}
		}

};