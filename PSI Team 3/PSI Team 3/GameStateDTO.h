/*
 * Authors: Sonja Kopf, Tomasz Zaremba
 * Data Transfer Object 
 * to exchange the game state between two players
 * can be serialized
 * contains information about units (of both players)
 * and whose turn it is
 * --> just a container!
 */
#include <BaseUnitDTO.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#define GAME_STATE_FILE_NAME "serializationFileGameState"

class GameStateDTO {

private:

	BaseUnitDTO* units; // Array of units
	bool giveUp; // part of the victory/defeat condition
	bool player1Turn; // true - its player1's turn, false - it's player2's turn

public:
	GameStateDTO() { 
		giveUp = false;
		player1Turn = true;
	};
	GameStateDTO(int length); // amount of units, needed to allocate memory
	~GameStateDTO();

	// getters + setters
	void setUnits(BaseUnitDTO* units) { this->units = units; }
	BaseUnitDTO* getUnits() { return units; }
	void setGiveUp(bool giveUp) { this->giveUp = giveUp; }
	bool getGiveUp() { return giveUp; }
	void setPlayer1Turn(bool player1Turn) { this->player1Turn = player1Turn; }
	bool getPlayer1Turn() { return player1Turn; }

	bool victory;

	// serialization
	friend class boost::serialization::access;
    template<class Archive>	
    void serialize(Archive & ar, const unsigned int version)
    {
        for(int i = 0; i < unitLength; i++) {
			ar & units[i];
		}
        ar & giveUp;
		ar& victory;
    }

	char* serializeGameState();
	void deserialize(std::string serializationString);
	int unitLength;


};