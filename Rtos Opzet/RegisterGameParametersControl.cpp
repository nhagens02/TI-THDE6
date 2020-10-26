#include "hwlib.hpp"
#include "rtos.hpp"

enum eButtonID = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5,BUTTON_6,BUTTON_7,BUTTON_8,BUTTON_9,BUTTON_0,BUTTON_*,BUTTON_#,BUTTON_A,BUTTON_B,BUTTON_C,BUTTON_D};

class RegisterGameParametersControl : public rtos::task<>{
	enum state_t = {};

	private:

	RegisterGameParametersControl():

	{}

	public:



	private:
		void main() {

			for(;;){
					switch(state)
					{
						case idle:
							//entry events


					}
			}

		}
};