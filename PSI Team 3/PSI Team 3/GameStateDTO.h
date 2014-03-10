// Authors: Sonja Kopf, Tomasz Zaremba
#include <BaseUnit.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class GameStateDTO { // Game state Data Transfer Object

private:
	BaseUnit* units; // Array of units
	bool giveUp;
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
	void setUnits(BaseUnit* units) { this->units = units; }
	BaseUnit* getUnits() { return units; }
	void setGiveUp(bool giveUp) { this->giveUp = giveUp; }
	bool getGiveUp() { return giveUp; }

}