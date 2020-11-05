#ifndef SOUNCONTROL.HPP
#define SOUNDCONTROL_HPP

class soundControl : public rtos::task<>{
	enum state_t state = {idle,playSound};
private:
	state_t state = idle;
	rtos::pool playSoundIDPool;
	rtos::flag playSoundFlag;
	hwlib::pin_oc& speakerPin;

public:
	soundControl(hwlib::pin_oc& speakerPin):
	task("Speaker Control"),
		speakerPin(speakerPin)
    {}

		void await( long long unsigned int t ){
		   while( t > hwlib::now_us() ){hwlib::wait_ms(0);}
		}

		void playSound( hwlib::pin_out & speakerPin, int f, int d, int fd = 1000 ){
		   auto t = hwlib::now_us();
		   auto end = t + d;
		   while( end > hwlib::now_us() ){
		       auto p = 500'000 / f;
		       f = f * fd / 1000;
		       speakerPin.write( 1 );
		       speakerPin.flush();
		       await( t += p );
		       speakerPin.write( 0 );
		       speakerPin.flush();
		       await( t += p );
		   }
		}
};

#endif
