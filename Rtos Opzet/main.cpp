#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "sendIrMessageControl.cpp"
#include "playerEntity.cpp"
//#include "IRLed.cpp"

int main( void ) {
	hwlib::wait_ms(500);

	hwlib::cout << "startup\n";
	
	//namespace target = hwlib::target;

	auto pe = playerEntity();
	auto IrLed_o = hwlib::target::pin_out(hwlib::target::pins::d10);
	//auto classIrled = IRLed(IrLed_o);
	auto sendIrMessage = SendIRMessageControl(IrLed_o);

	rtos::run();
}
