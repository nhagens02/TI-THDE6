#include "hwlib.hpp"

struct KeypadButton {
	hwlib::pin_direct_from_oc_t* pinOut;
	hwlib::pin_direct_from_in_t* pinIn;
};


class Keypad {
private:
	KeypadButton buttons[16];
public:
	Keypad(hwlib::pin_direct_from_in_t* pin1, hwlib::pin_direct_from_in_t* pin2, hwlib::pin_direct_from_in_t* pin3, hwlib::pin_direct_from_in_t* pin4, hwlib::pin_direct_from_oc_t* pin5, hwlib::pin_direct_from_oc_t* pin6, hwlib::pin_direct_from_oc_t* pin7, hwlib::pin_direct_from_oc_t* pin8){
		buttons[0] = { pin8, pin2 }; // 0
		buttons[1] = { pin5, pin1 }; // 1
		buttons[2] = { pin5, pin2 }; // 2
		buttons[3] = { pin5, pin3 }; // 3
		buttons[4] = { pin6, pin1 }; // 4
		buttons[5] = { pin6, pin2 }; // 5
		buttons[6] = { pin6, pin3 }; // 6
		buttons[7] = { pin7, pin1 }; // 7
		buttons[8] = { pin7, pin2 }; // 8
		buttons[9] = { pin7, pin3 }; // 9
		buttons[10] = { pin5, pin4 }; // A
		buttons[11] = { pin6, pin4 }; // B
		buttons[12] = { pin7, pin4 }; // C
		buttons[13] = { pin8, pin4 }; // D
		buttons[14] = { pin8, pin1 }; // *
		buttons[15] = { pin8, pin3 }; // #
		pin5->write(0);
		pin6->write(0);
		pin7->write(0);
		pin8->write(0);
	}
	bool getKeyPressed(int buttonID) {
		buttons[buttonID].pinOut->write(0);
		bool returnValueInvert = buttons[buttonID].pinIn->read();
		buttons[buttonID].pinOut->write(1);
		bool returnValue = !returnValueInvert;
		return returnValue;
	}
};