#include "hwlib.hpp"
#include "soundControl.hpp"

int main(){
   auto speakerPin = hwlib::target::pin_out( hwlib::target::pins::d7 );
   playSound(speakerPin);
}
