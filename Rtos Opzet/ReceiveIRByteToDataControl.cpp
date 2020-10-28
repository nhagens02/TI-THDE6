#include "hwlib.hpp"
#include "rtos.hpp"

class ReceiveIRByteToDataControl : public rtos::task<>{
	enum state_t = {idle, decodeData};

	private:
		state_t state = idle;
		rtos::channel messageChannel;
		RunGameControl& RunGameControl;
		registerGameParameters& registerGameParameters;
		receiveIrMessageControl& receiveIrMessageControl;
		void 

	ReceiveIRByteToDataControl():

	{}	

	public:
		

};