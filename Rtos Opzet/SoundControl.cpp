#include "hwlib.hpp"
#include "rtos.hpp"
/// @file


/// \brief
/// SoundControl CLASS
/// \details
/// This class manages the playback of sounds of the lazergun. 
/// This class uses the boundary object: Speaker.
/// The class uses the soundEntity to get the sounds.
/// This class uses rtos::task<>.
class SoundControl : public rtos::task<>{
	enum state_t state = {idle,playSound};

	private:
		state_t state = idle;
		rtos::pool playSoundIDPool;
		rtos::flag playSoundFlag
		hwlib::pin_oc& speakerPin;

	public:
		soundControl(hwlib::pin_oc& speakerPin):
			task("speaker Control"),
			speakerPin(speakerPin)
		{}

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