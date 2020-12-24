#include "hwlib.hpp"
#include "soundControl.hpp"

int main(){
   auto speakerPin = hwlib::target::pin_oc( hwlib::target::pins::d7 );
   auto soundClass = soundControl(speakerPin);
}
