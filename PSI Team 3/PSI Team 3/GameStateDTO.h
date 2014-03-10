// Authors: Sonja Kopf, Tomasz Zaremba
/*
#include <BaseUnit.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#define FILE_NAME "serializationFile"

class GameStateDTO { // Game state Data Transfer Object

private:
	BaseUnit* units; // Array of units
	bool giveUp;
	bool player1Turn;
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & units;
        ar & giveUp;
    }
public:
	GameStateDTO(int length); // Length of the units array
	~GameStateDTO();
	char* serialize();
	void deserialize(char* serializationString);
	void setUnits(BaseUnit* units) { this->units = units; }
	BaseUnit* getUnits() { return units; }
	void setGiveUp(bool giveUp) { this->giveUp = giveUp; }
	bool getGiveUp() { return giveUp; }
	void setPlayer1Turn(bool player1Turn) { this->player1Turn = player1Turn; }
	bool getPlayer1Turn() { return player1Turn; }

};
*/