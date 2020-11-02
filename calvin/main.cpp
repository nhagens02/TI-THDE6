#include "hwlib.hpp"
#include "IRLed.hpp"

void test_dump( hwlib::istream & kbd ){
   int n = 0;
   for(;;){
      auto c = kbd.getc();
      hwlib::cout << ++n << " [" << c << "]\n";
   }
}

int main( void ){

   hwlib::wait_ms( 500 );
   hwlib::cout << "Keypad demo\n";

   auto out0 = hwlib::target::pin_oc( hwlib::target::pins::a0 );
   auto out1 = hwlib::target::pin_oc( hwlib::target::pins::a1 );
   auto out2 = hwlib::target::pin_oc( hwlib::target::pins::a2 );
   auto out3 = hwlib::target::pin_oc( hwlib::target::pins::a3 );

   auto in0  = hwlib::target::pin_in( hwlib::target::pins::a4 );
   auto in1  = hwlib::target::pin_in( hwlib::target::pins::a5 );
   auto in2  = hwlib::target::pin_in( hwlib::target::pins::a6 );
   auto in3  = hwlib::target::pin_in( hwlib::target::pins::a7 );

   auto out_port = hwlib::port_oc_from( out3, out2, out1, out0 );
   auto in_port  = hwlib::port_in_from( in3,  in2,  in1,  in0  );
   auto matrix   = hwlib::matrix_of_switches( out_port, in_port );
   auto keypad   = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );

   test_dump( keypad );
}
