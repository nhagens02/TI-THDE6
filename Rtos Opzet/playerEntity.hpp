#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>
#include "StructData.hpp"

/// \brief
/// PlayerEntity CLASS
/// \details
/// This class will store all the data of a specific player. 
/// This class uses rtos::pool. 
class PlayerEntity {
    private:
   		rtos::pool< int > playerIDPool;
   		rtos::pool< int > weaponIDPool;
   		rtos::pool< int > livesPool;
        rtos::pool< int > ammoPool;
   		rtos::pool < struct data_s > data_pool;
   		struct data_s data_a;

	public:
		PlayerEntity( ):
        playerIDPool( "playerID_pool" ),
        weaponIDPool( "weaponID_pool" ),
        livesPool( "lives_pool" ),
        ammoPool( "ammo_pool" ),
        data_pool( "data_pool" )
        {
            //default values
            playerIDPool.write(1);
            weaponIDPool.write(1);
            ammoPool.write(100);
            livesPool.write(10);
            data_pool.write(data_a);
        }
        //pool mag ook opslag
		void setPlayerID(int playerID){ playerIDPool.write(playerID);}
		int getPlayerID(){return (playerIDPool.read());}

		void setWeaponPower(int weaponID){weaponIDPool.write(weaponID);}
		int getWeaponPower(){return (weaponIDPool.read());}

		void setLives(int lives){ livesPool.write(lives);}
		int getlives(){return (livesPool.read());}

        void setAmmo(int ammo) { ammoPool.write(ammo); }
        int getAmmo() { return (ammoPool.read()); }

        void addData(struct shootdata sData) { if (data_a.counter < 500) { data_a.playerID[data_a.counter] = sData.playerID; data_a.weaponStrength[data_a.counter] = sData.weaponStrength; data_a.counter++; } data_pool.write(data_a); }
	    struct data_s getData(){return data_pool.read(); }

};

#endif // PLAYERENTITY_HPP
