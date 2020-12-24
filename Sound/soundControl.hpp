#ifndef SOUNDCONTROL_HPP
#define SOUNDCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
/// \brief
/// soundControl CLASS
/// \details
/// This class will manage wich sound needs to be played at a specific time.
/// This class needs a speaker pin of type hwlib::pin_oc.
class soundControl : public rtos::task<> {
	enum state_t { idle, playSound };
private:
	state_t state = idle;
	hwlib::pin_oc& speakerPin;
	rtos::pool< int > soundIDPool;
	rtos::flag playSoundFlag;
	int soundID = 0;

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

		void makeSound( hwlib::pin_oc & speakerPin, int f, int d, int fd = 1000 ){
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

		void Pew(hwlib::pin_oc& speakerPin) {
			makeSound(speakerPin, 20'000, 200'000, 990);
		}

		void startTune(hwlib::pin_oc& speakerPin) {
			makeSound(speakerPin, 500, 600'000, 1000);
			hwlib::wait_us(400'000);
			makeSound(speakerPin, 500, 600'000, 1000);
			hwlib::wait_us(400'000);
			makeSound(speakerPin, 500, 600'000, 1000);
			hwlib::wait_us(400'000);
			makeSound(speakerPin, 1000, 500'000, 1000);
		}


	private:
		void main() {
			for (;;) {
				switch (state)
				{
					case idle:
						wait(playSoundFlag);
						state = playSound;
						break;

					case playSound:
						soundID = soundIDPool.read();
						if (soundID == 1) {
							startTune(speakerPin);
						}
						else if (soundID == 2) {
							pew(speakerPin);
						}
						break;
					
					default:break;
				}
			}
		}
};

#endif // SOUNDCONTROL_HPP
