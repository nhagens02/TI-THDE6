#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "DataToIrByteControl.cpp"
//#include "sendIrMessageControl.cpp"
#include "playerEntity.cpp"

//#include "IRLed.cpp"

class test : public rtos::task<> {
	DataToIrbyteControl DataToIrByteControl;
	struct shootdata data;
	
	void main() {
		data.playerID = 7;
		data.weaponstrenght = 4;
		for (;;) {
			hwlib::wait_ms(2000);
			DataToIrByteControl.sendTriggerfun(data);
			//hwlib::cout << data.playerID << hwlib::endl;
		}
	}

	public:
		test(hwlib::pin_out& ledpin) :
			task("test"),
			DataToIrByteControl(ledpin)
		{}

};




int main( void ) {
	hwlib::wait_ms(500);

	hwlib::cout << "startup\n";
	
	//namespace target = hwlib::target;

	auto pe = playerEntity();
	auto IrLed_output = hwlib::target::d2_36kHz();;
	//auto sendIrMessage = SendIRMessageControl(IrLed_output);
	//auto dataToIrByteControl = DataToIrbyteControl(IrLed_output);
	auto test2 = test(IrLed_output);
	rtos::run();
}
