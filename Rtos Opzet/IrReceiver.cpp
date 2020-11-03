#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// IrReceiver CLASS
/// \details
/// This class is be used to read out the ir receiver. 
/// This class uses rtos::task<>. 
class IrReceiver{
	private:
		hwlib::pin_in& IrReceiverPin;
	public: 
		IrReceiver(hwlib::pin_in& IrReceiverPin):
			IrReceiverPin(IrReceiverPin)
		{}
		bool getCurrentSignal() { IrReceiverPin.refresh(); return (IrReceiverPin.read()); }

};