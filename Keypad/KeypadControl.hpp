#include "Keypad.hpp"

class KeypadControl {
private:
	Keypad keypad;
public:
	KeypadControl(hwlib::pin_direct_from_oc_t* pinOut1, hwlib::pin_direct_from_oc_t* pinOut2, hwlib::pin_direct_from_oc_t* pinOut3, hwlib::pin_direct_from_oc_t* pinOut4, hwlib::pin_direct_from_in_t* pinIn1, hwlib::pin_direct_from_in_t* pinIn2, hwlib::pin_direct_from_in_t* pinIn3, hwlib::pin_direct_from_in_t* pinIn4) :
		keypad(pinOut1, pinOut2, pinOut3, pinOut4, pinIn1, pinIn2, pinIn3, pinIn4)
	{}

};