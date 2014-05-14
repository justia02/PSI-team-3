#pragma once
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#define SERIALIZATION_FILE_NAME "serializationFile"

class Serializer
{
public:
	Serializer(void);
	~Serializer(void);
	char* serializeObj(void* anyobj);
};

