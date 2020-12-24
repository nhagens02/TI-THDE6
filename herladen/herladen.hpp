#ifndef HERLADEN_HPP
#define HERLADEN_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "../Rtos Opzet/playerEntity.hpp"

class reloadControl : public rtos::task<> {
	enum state_t { idle, reload };
private:
	state_t state = idle;
	rtos::flag reloadFlag;
	PlayerEntity& playerEntity;

public:
	reloadControl(PlayerEntity& playerEntity ) :
		task("reloading"),
		reloadFlag(this, "reloadFlag"),
		playerEntity(playerEntity)
	{}
		
	private:
		void main() {
			for (;;) {
				switch (state)
				{
				case idle:
					wait(reloadFlag);
					state = reload;
					break;

				case reload:
					if (playerEntity.getWeaponPower() == 1) {
						playerEntity.setAmmo(100);
					}
					if (playerEntity.getWeaponPower() == 2) {
						playerEntity.setAmmo(100);
					}
					break;

				default:break;
				}
			}
		}
};






#endif // HERLADEN_HPP