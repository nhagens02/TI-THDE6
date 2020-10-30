#include "hwlib.hpp"
#include "rtos.hpp"


class SoundControl : public rtos::task<>{
	enum state_t state = {idle,playSound};

	private:
		state_t = idle;
		rtos::pool playSoundIDPool;
		rtos::flag playSoundFlag;


	SoundControl():

	{}

	public:
		void playSound(int soundID){playSoundID.write(soundID);playSoundFlag.set();}

	private:
		void main(){

			for(;;){
				switch(state)
				{
					case idle:
						//entry events

						//other events
						hwlib::wait(playSoundFlag);
						int soundID = playSoundIDPool.read();
						state = playSound;
						break;

					case playSound:
						//entry events
						auto sound = soundFiles.getSoundFiles(soundID);
						playSound(sound);
						
						//other events
						state = idle;
						break;

					default:break;

				}	
			}
		}
};