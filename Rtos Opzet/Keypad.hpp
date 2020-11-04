#include "hwlib.hpp"
#include "rtos.hpp"

struct KeypadButton {
	hwlib::pin_direct_from_oc_t* pinOut;
	hwlib::pin_direct_from_in_t* pinIn;
};

/// \brief
/// Keypad CLASS
/// \details
/// This class will read the buttons that are pressed on the keypad. 
/// This class uses hwlib::pin_direct_from_oc_t, hwlib::pin_direct_from_in_t. 
class Keypad {
private:
	KeypadButton buttons[16];
public:
	Keypad(hwlib::pin_direct_from_oc_t* pinOut1, hwlib::pin_direct_from_oc_t* pinOut2, hwlib::pin_direct_from_oc_t* pinOut3, hwlib::pin_direct_from_oc_t* pinOut4, hwlib::pin_direct_from_in_t* pinIn1, hwlib::pin_direct_from_in_t* pinIn2, hwlib::pin_direct_from_in_t* pinIn3, hwlib::pin_direct_from_in_t* pinIn4){
		buttons[0] = { pinOut4, pinIn2 }; // 0
		buttons[1] = { pinOut1, pinIn1 }; // 1
		buttons[2] = { pinOut1, pinIn2 }; // 2
		buttons[3] = { pinOut1, pinIn3 }; // 3
		buttons[4] = { pinOut2, pinIn1 }; // 4
		buttons[5] = { pinOut2, pinIn2 }; // 5
		buttons[6] = { pinOut2, pinIn3 }; // 6
		buttons[7] = { pinOut3, pinIn1 }; // 7
		buttons[8] = { pinOut3, pinIn2 }; // 8
		buttons[9] = { pinOut3, pinIn3 }; // 9
		buttons[10] = { pinOut1, pinIn4 }; // A
		buttons[11] = { pinOut2, pinIn4 }; // B
		buttons[12] = { pinOut3, pinIn4 }; // C
		buttons[13] = { pinOut4, pinIn4 }; // D
		buttons[14] = { pinOut4, pinIn1 }; // *
		buttons[15] = { pinOut4, pinIn3 }; // #
		pinOut1->write(0);
		pinOut2->write(0);
		pinOut3->write(0);
		pinOut4->write(0);
	}
	bool getKeyPressed(int buttonID) {
		buttons[buttonID].pinOut->write(0);
		bool returnValueInvert = buttons[buttonID].pinIn->read();
		buttons[buttonID].pinOut->write(1);
		bool returnValue = !returnValueInvert;
		return returnValue;
	}
};
