#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "playerEntity.cpp"

int main( void ) {
	hwlib::wait_ms(500);

	hwlib::cout << "startup\n";
	
	namespace target = hwlib::target;

	playerEntity();

	rtos::run();
}
