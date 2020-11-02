#include "hwlib.hpp"
#include "rtos.hpp"
#include <array>

struct data_s{
	std::array<int,500> array;
    int counter = 0;;
};

//class must use pool with public function
class playerEntity : public rtos::task<> {
	private:
		rtos::mutex player_mutex;
   		rtos::pool< int > playerIDPool;
   		rtos::mutex weapon_mutex;
   		rtos::pool< int > weaponIDPool;
   		rtos::mutex lives_mutex;
   		rtos::pool< int > livesPool;
   		rtos::mutex data_mutex;
   		rtos::pool < struct data_s > data;
   		
   		int playerID;
   		int weaponID;
   		int lives;
   		struct data_s data_a;
   		//data_a.counter = 0;

        void main() {

        }
   		

	public:
		playerEntity( ):

		task ("entityPlayer"),
		player_mutex( "player_mutex" ),
        playerIDPool( "playerID_pool" ),
        weapon_mutex( "weapon_mutex" ),
        weaponIDPool( "weaponID_pool" ),
        lives_mutex( "lives_mutex" ),
        livesPool( "lives_pool" ),
        data_mutex( "data_mutex" ),
        data( "data_pool" )
        {}

		void setPlayerID(int playerId){ player_mutex.wait(); playerID = playerId; player_mutex.signal();}
		void getPlayerID(){player_mutex.wait(); playerIDPool.write(playerID); player_mutex.signal();}

		void setWeaponPower(int weaponId){ weapon_mutex.wait(); weaponID = weaponId; weapon_mutex.signal();}
		void getWeaponPower(){weapon_mutex.wait(); weaponIDPool.write(weaponID); weapon_mutex.signal();}

		void setLives(int lives_n){ lives_mutex.wait(); lives = lives_n; lives_mutex.signal();}
		void getlives(){lives_mutex.wait(); livesPool.write(lives); lives_mutex.signal();}

        void addData(int playerID_n) { data_mutex.wait(); if (data_a.counter < 500) { data_a.array[data_a.counter] = playerID_n; data_a.counter++; }; data_mutex.signal(); }
		void getData(){ data_mutex.wait(); data.write(data_a); data_mutex.signal(); }

};