#include "hwlib.hpp"

int main(void) {
   
   namespace target = hwlib::target;
   
   auto tsop_signal = target::pin_in(target::pins::d8);
   auto tsop_gnd    = target::pin_out(target::pins::d9);
   auto tsop_vdd    = target::pin_out(target::pins::d10);
   auto led         = target::pin_out(target::pins::led);
   
   tsop_gnd.write(0);
   tsop_vdd.write(1);
   tsop_gnd.flush();
   tsop_vdd.flush();
   
   auto const active = 100'000;
   auto last_signal = hwlib::now_us() - active;
   
   for(;;){
      tsop_signal.refresh();
      if(tsop_signal.read() == 0) {
         last_signal = hwlib::now_us();
      }
      led.write((last_signal + active) > hwlib::now_us());
      led.flush();
   }
}
