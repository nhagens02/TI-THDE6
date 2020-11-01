#include "hwlib.hpp"
#include "KeypadControl.hpp"

int main() {
	due::pin_in pin1__ = hwlib::target::pin_in(hwlib::target::pins::d53);
	hwlib::pin_direct_from_in_t pin1_(pin1__);
	hwlib::pin_direct_from_in_t* pin1 = &pin1_;
	due::pin_in pin2__ = hwlib::target::pin_in(hwlib::target::pins::d51);
	hwlib::pin_direct_from_in_t pin2_(pin2__);
	hwlib::pin_direct_from_in_t* pin2 = &pin2_;
	due::pin_in pin3__ = hwlib::target::pin_in(hwlib::target::pins::d49);
	hwlib::pin_direct_from_in_t pin3_(pin3__);
	hwlib::pin_direct_from_in_t* pin3 = &pin3_;
	due::pin_in pin4__ = hwlib::target::pin_in(hwlib::target::pins::d47);
	hwlib::pin_direct_from_in_t pin4_(pin4__);
	hwlib::pin_direct_from_in_t* pin4 = &pin4_;
	due::pin_oc pin5__ = hwlib::target::pin_oc(hwlib::target::pins::d45);
	hwlib::pin_direct_from_oc_t pin5_(pin5__);
	hwlib::pin_direct_from_oc_t* pin5 = &pin5_;
	due::pin_oc pin6__ = hwlib::target::pin_oc(hwlib::target::pins::d43);
	hwlib::pin_direct_from_oc_t pin6_(pin6__);
	hwlib::pin_direct_from_oc_t* pin6 = &pin6_;
	due::pin_oc pin7__ = hwlib::target::pin_oc(hwlib::target::pins::d41);
	hwlib::pin_direct_from_oc_t pin7_(pin7__);
	hwlib::pin_direct_from_oc_t* pin7 = &pin7_;
	due::pin_oc pin8__ = hwlib::target::pin_oc(hwlib::target::pins::d39);
	hwlib::pin_direct_from_oc_t pin8_(pin8__);
	hwlib::pin_direct_from_oc_t* pin8 = &pin8_;
	Keypad test(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8);
	hwlib::wait_ms(2000);
	hwlib::cout << test.getKeyPressed(0);
}