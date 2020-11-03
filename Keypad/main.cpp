#include "hwlib.hpp"
#include "KeypadControl.hpp"



int main() {
	//pins out
	due::pin_oc pinOut1__ = hwlib::target::pin_oc(hwlib::target::pins::d53);
	hwlib::pin_direct_from_oc_t pinOut1_(pinOut1__);
	hwlib::pin_direct_from_oc_t* pinOut1 = &pinOut1_;
	due::pin_oc pinOut2__ = hwlib::target::pin_oc(hwlib::target::pins::d51);
	hwlib::pin_direct_from_oc_t pinOut2_(pinOut2__);
	hwlib::pin_direct_from_oc_t* pinOut2 = &pinOut2_;
	due::pin_oc pinOut3__ = hwlib::target::pin_oc(hwlib::target::pins::d49);
	hwlib::pin_direct_from_oc_t pinOut3_(pinOut3__);
	hwlib::pin_direct_from_oc_t* pinOut3 = &pinOut3_;
	due::pin_oc pinOut4__ = hwlib::target::pin_oc(hwlib::target::pins::d47);
	hwlib::pin_direct_from_oc_t pinOut4_(pinOut4__);
	hwlib::pin_direct_from_oc_t* pinOut4 = &pinOut4_;
	//pins in
	due::pin_in pinIn1__ = hwlib::target::pin_in(hwlib::target::pins::d45);
	hwlib::pin_direct_from_in_t pinIn1_(pinIn1__);
	hwlib::pin_direct_from_in_t* pinIn1 = &pinIn1_;
	due::pin_in pinIn2__ = hwlib::target::pin_in(hwlib::target::pins::d43);
	hwlib::pin_direct_from_in_t pinIn2_(pinIn2__);
	hwlib::pin_direct_from_in_t* pinIn2 = &pinIn2_;
	due::pin_in pinIn3__ = hwlib::target::pin_in(hwlib::target::pins::d41);
	hwlib::pin_direct_from_in_t pinIn3_(pinIn3__);
	hwlib::pin_direct_from_in_t* pinIn3 = &pinIn3_;
	due::pin_in pinIn4__ = hwlib::target::pin_in(hwlib::target::pins::d39);
	hwlib::pin_direct_from_in_t pinIn4_(pinIn4__);
	hwlib::pin_direct_from_in_t* pinIn4 = &pinIn4_;

	Keypad test(pinOut1, pinOut2, pinOut3, pinOut4, pinIn1, pinIn2, pinIn3, pinIn4);
	hwlib::wait_ms(2000);
	for (;;) {
		if (test.getKeyPressed(0)) {
			hwlib::cout << "0\n";
		}
		if (test.getKeyPressed(1)) {
			hwlib::cout << "1\n";
		}
		if (test.getKeyPressed(2)) {
			hwlib::cout << "2\n";
		}
		if (test.getKeyPressed(3)) {
			hwlib::cout << "3\n";
		}
		if (test.getKeyPressed(4)) {
			hwlib::cout << "4\n";
		}
		if (test.getKeyPressed(5)) {
			hwlib::cout << "5\n";
		}
		if (test.getKeyPressed(6)) {
			hwlib::cout << "6\n";
		}
		if (test.getKeyPressed(7)) {
			hwlib::cout << "7\n";
		}
		if (test.getKeyPressed(8)) {
			hwlib::cout << "8\n";
		}
		if (test.getKeyPressed(9)) {
			hwlib::cout << "9\n";
		}
		if (test.getKeyPressed(10)) {
			hwlib::cout << "A\n";
		}
		if (test.getKeyPressed(11)) {
			hwlib::cout << "B\n";
		}
		if (test.getKeyPressed(12)) {
			hwlib::cout << "C\n";
		}
		if (test.getKeyPressed(13)) {
			hwlib::cout << "D\n";
		}
		if (test.getKeyPressed(14)) {
			hwlib::cout << "*\n";
		}
		if (test.getKeyPressed(15)) {
			hwlib::cout << "#\n";
		}
	}
}