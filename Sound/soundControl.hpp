#ifndef SOUNCONTROL.HPP
#define SOUNDCONTROL_HPP

class soundControl {
private:
	hwlib::pin_oc& speakerPin;

public:
	soundControl(hwlib::pin_oc& speakerPin):
		speakerPin(speakerPin)
    {}

		void await( long long unsigned int t ){
		   while( t > hwlib::now_us() ){}
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
