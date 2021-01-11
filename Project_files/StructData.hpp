#ifndef STRUCTDATA_HPP
#define STRUCTDATA_HPP
/// @file


/// \brief
/// GameTime options
/// \details
/// This enum contains all possible times for a game of lazergun.
enum gameTimes {
	veryShortTime = 0, shortTime = 1, mediumTime = 2, longTime = 3, veryLongTime = 4, testing = 7
};


/// \brief
/// gameModes enum
/// \details
/// This enum contains the gamemodes in the game.
/// This class uses rtos::task<>. 
enum gameModes {
	regular = 0, goldenGun = 1
};


/// \brief
/// enum eButtonID
/// \details
/// This enum will define the possible states for the button's.
/// The Buttons in the class RunGameControl are: reloadButton, TriggerButton.
enum eButtonID { triggerButton, reloadButton };


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
	std::array<int, 50> playerID;
	std::array<int, 50> weaponStrength;
	unsigned int counter = 0;
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