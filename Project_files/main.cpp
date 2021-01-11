#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "StructData.hpp"
#include "DataToIrByteControl.hpp"
#include "playerEntity.hpp"
#include "InitGameControl.hpp"
#include "keypadControl.hpp"
#include "DisplayController.hpp"
#include "RegisterGameParametersControl.hpp"
#include "bitDetector.hpp"
#include "ReceiveIrByteToDataControl.hpp"
#include "TimerControl.hpp"
#include "soundControl.hpp"
#include "ButtonControl.hpp"
#include "RunGameControl.hpp"
#include "TransferHitsControl.hpp"


int main( void ) {
	
	hwlib::wait_ms(2000);

	auto speakerPin = hwlib::target::pin_oc( hwlib::target::pins::d7 );
   	auto soundClass = SoundControl(speakerPin);


	//keypad pins 
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
	
	auto tsop_signal = hwlib::target::pin_in(hwlib::target::pins::d8);
	auto tsop_gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto tsop_vdd = hwlib::target::pin_out(hwlib::target::pins::d10);

	tsop_gnd.write(0);
	tsop_vdd.write(1);
	tsop_gnd.flush();
	tsop_vdd.flush();

	PlayerEntity pe;

	auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
	auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);

	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

	auto oled = hwlib::glcd_oled(i2c_bus, 0x3c);

	DisplayController display(scl, sda);
	RegisterGameParametersControl regPar(pe, display);
	
	auto IrLed_output = hwlib::target::d2_36kHz();

	DataToIrbyteControl dataToIrByteControl(IrLed_output);

	TransferHitsControl transferHitsControl(pe);

	RunGameControl runGame(dataToIrByteControl, display, pe, transferHitsControl, soundClass);

	TimerControl timerControl(runGame);

	InitGameControl init(dataToIrByteControl, display, runGame, timerControl);

	keypadControl keyPad(pinOut1, pinOut2, pinOut3, pinOut4, pinIn1, pinIn2, pinIn3, pinIn4, init, regPar);

	ReceiveIrByteToDataControl receiveIrByte(regPar, runGame, timerControl);

	ReceiveIrMessageControl recIrMessage(receiveIrByte);

	BitDetector bitDet(tsop_signal, recIrMessage);

	auto triggerButton =hwlib::target::pin_in(hwlib::target::pins::d3);
	auto reloadButton = hwlib::target::pin_in(hwlib::target::pins::d4);

	ButtonControl buttonControl(runGame, triggerButton, reloadButton);

	hwlib::cout << "startup\n";
	rtos::run();
}
