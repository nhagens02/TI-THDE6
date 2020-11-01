#include "Keypad.hpp"

class KeypadControl {
private:
	Keypad keypad;
public:
	KeypadControl(hwlib::pin_direct_from_in_t* pin1, hwlib::pin_direct_from_in_t* pin2, hwlib::pin_direct_from_in_t* pin3, hwlib::pin_direct_from_in_t* pin4, hwlib::pin_direct_from_oc_t* pin5, hwlib::pin_direct_from_oc_t* pin6, hwlib::pin_direct_from_oc_t* pin7, hwlib::pin_direct_from_oc_t* pin8) :
		keypad(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8)
	{}

};