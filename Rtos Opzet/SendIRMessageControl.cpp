#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// SendIRMessageControl CLASS
/// \details
/// This class wil be used to send ir messsages with the boundary object: "IrLed".
/// In the main function will be waited until it received 16 bits(2bytes) to send it.
/// This class uses rtos::task<>.
class SendIRMessageControl : public rtos::task<>{

};