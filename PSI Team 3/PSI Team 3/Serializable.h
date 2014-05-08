#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>
#pragma once
class Serializable
{
public:
	Serializable(void);
	virtual ~Serializable(void);

	char* serializeObject();
	friend class boost::serialization::access;
    template<class Archive>	
	void serialize(Archive & ar, const unsigned int version) {
	};
	virtual void deserializeObject(std::string serializationString) = 0;
};

