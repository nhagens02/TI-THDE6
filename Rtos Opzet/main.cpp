#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "StructData.hpp"
#include "DataToIrByteControl.hpp"

//#include "sendIrMessageControl.cpp"
#include "playerEntity.hpp"
#include "InitGameControl.hpp"
#include "keypadControl.cpp"
#include "DisplayController.hpp"
#include "RegisterGameParametersControl.hpp"
#include "bitDetector.hpp"
#include "ReceiveIrByteToDataControl.hpp"

//#include "IRLed.cpp"
#include "RunGameControl.hpp"

class test : public rtos::task<> {
	DataToIrbyteControl& dataToIrByteControl;
	struct shootdata data;
	struct parameters para;
	
	void main() {
		data.playerID = 7;
		data.weaponStrength = 4;
		para.gameMode = 1;
		para.gameTime = 5;
		para.timeUntilStart = 10;
		for (;;) {
			hwlib::wait_ms(2000);
			dataToIrByteControl.sendTriggerChannel(data);
			hwlib::wait_ms(2000);
			//dataToIrByteControl.sendingGameParametersChannel(para);
			//hwlib::cout << data.playerID << hwlib::endl;
		}
	}

	public:
		test(DataToIrbyteControl& dataToIrByteControl) :
			task("test"),
			dataToIrByteControl(dataToIrByteControl)
		{}

};




int main( void ) {
	hwlib::wait_ms(600);

	hwlib::cout << "startup\n";
	
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


	auto pe = PlayerEntity();

	auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
	auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);

	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

	auto oled = hwlib::glcd_oled(i2c_bus, 0x3c);

	auto display = DisplayController(scl, sda);
	
	auto regPar = RegisterGameParametersControl(pe, display);

	
	hwlib::cout << "test1" << hwlib::endl;
	auto IrLed_output = hwlib::target::d2_36kHz();;

	auto dataToIrByteControl = DataToIrbyteControl(IrLed_output);
	


	//auto test2 = test(dataToIrByteControl);

	auto init = InitGameControl(dataToIrByteControl, display);

	

	auto keyPad = keypadControl(pinOut1, pinOut2, pinOut3, pinOut4, pinIn1, pinIn2, pinIn3, pinIn4, init, regPar);

	auto runGame = RunGameControl(dataToIrByteControl, display, pe);

	auto receiveIrByte = ReceiveIrByteToDataControl(regPar, runGame);

	auto recIrMessage = ReceiveIrMessageControl(receiveIrByte);

	auto bitDet = BitDetector(tsop_signal, recIrMessage);

	hwlib::cout << "before start" << hwlib::endl;
	rtos::run();
}
