#include "hwlib.hpp"
#include "soundControl.hpp"

int main(){
   auto speakerPin = hwlib::target::pin_out( hwlib::target::pins::d7 );
   auto soundClass = soundControl(speakerPin);
   soundClass.playSound(speakerPin, 20'000, 200'000, 990);
}
