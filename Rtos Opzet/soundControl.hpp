#ifndef SOUNCONTROL.HPP
#define SOUNDCONTROL_HPP

/// \brief
/// soundControl CLASS
/// \details
/// This class will manage wich sound needs to be played at a specific time. 
/// This class needs a speaker pin of type hwlib::pin_oc. 
class soundControl : public rtos::task<> {
	enum state_t { idle, playSound };
private:
	hwlib::pin_oc& speakerPin;
	rtos::pool < int > soundIDPool;
	rtos::flag playSoundFlag;

public:
	soundControl(hwlib::pin_oc& speakerPin):
		task("speaker"),
		speakerPin(speakerPin),
		soundIDPool("sound id Pool"),
		playSoundFlag(this, "playSoundFlag")
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

	private:
		void main() {
			for (;;) {
				switch (state)
				{
					case idle:


						
				}
			}
		}
};

#endif // SOUNDCONTROL_HPP
