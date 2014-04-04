/*
 * Author: Sonja Kopf
 * Data Transfer Object for units
*/
#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#define FILE_NAME "serializationFileBaseUnit"

class BaseUnitDTO
{

private:
	// relevant attributes for data transfer
	int id;

	float x;
	float y;
	float z;
	bool player1;

	// serialization stuff
	friend class boost::serialization::access;
    template<class Archive>	
	void serialize(Archive & ar, const unsigned int version)
    {
        ar & id;
		ar & x;
		ar & y;
		ar & z; 
		ar & player1;
    };


public:
	BaseUnitDTO() { };
	~BaseUnitDTO() { };
	BaseUnitDTO(float x, float y, float z, bool player1) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->player1 = player1;
	};
	
	int getId() {return this->id; };
	float getX() { return this->x; };
	float getY() { return this->y; };
	float getZ() { return this->z; };
	bool  getPlayer() { return this->player1; };

	void setId(int id) {this->id = id; };
	void setX(float x) { this->x = x; };
	void setY(float y) {this->y = y; };
	void setZ(float z) {this->z = z;};
	void setPlayer(bool player1) { this->player1 = player1; };

	// serialization
	char* serialize();
	void deserialize(std::string serializationString);
};

