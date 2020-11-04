#ifndef STRUCTDATA_HPP
#define STRUCTDATA_HPP

/// \brief
/// Game parameters struct. 
/// \details
/// This struct contains the GameMode, gameTime and timeUntil start of a game.
/// This is in seconds
/// All variables are of the type int. 
struct parameters {
	int gameMode;
	int gameTime;
	int timeUntilStart;
};


struct data_s {
	std::array<int, 500> playerID;
	std::array<int, 500> weaponStrength;
	int counter = 0;
};


/// \brief
/// Timer Struct
/// \details
/// Struct used to save data about the timer used.
/// Option 1: If runGameTime is true, the time in timer used is the time how long the game is.
/// Option 2: If runGameTime is false, the time in timer used is not the time how long the game is.
/// The time used will be in minutes.
struct timer_struct {
	int time;
	bool runGameTime;
};


/// \brief
/// Shoot data struct
/// \details
/// This struct contains a playerID and weaponStrength. 
/// This struct will be used to send data of a shot. 
struct shootdata {
	int playerID;
	int weaponStrength;
};

#endif // STRUCTDATA_HPP