// Authors: Sonja Kopf, Tomasz Zaremba
/*
#include <iostream>
#include <fstream>
#include <GameStateDTO.h>

GameStateDTO::GameStateDTO(int length) {

	units = new BaseUnit[length]; // Allocating memory
	giveUp = false;
	player1Turn = true;

}

GameStateDTO::~GameStateDTO() { 
	delete units;
}

void GameStateDTO::deserialize(char* serializationString) {

	std::ofstream ofs(FILE_NAME);
	ofs << serializationString;
	ofs.close();

	{
        // create and open an archive for input
        std::ifstream ifs(FILE_NAME);
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> *this;
        // archive and stream closed when destructors are called
    }

}

char* GameStateDTO::serialize() {

	// create and open a character archive for serialized vector instance
    std::ofstream ofs(FILE_NAME);

	// save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << *this;
    	// archive and stream closed when destructors are called

		ofs.close();
    }

	std::ifstream ifs(FILE_NAME);

	std::filebuf* pbuf = ifs.rdbuf();

	// get file size using buffer's members
	size_t size = pbuf->pubseekoff (0, ifs.end, ifs.in);
	pbuf->pubseekpos (0, ifs.in);

	// allocate memory to contain file data
	char* buffer = new char[size];

	// get file data
	pbuf->sgetn(buffer, size);
	// std::cout << "Buffer to be send: " << buffer << std::endl;
	ifs.close();

	return buffer;

}
*/