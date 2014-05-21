/*
 * Authors: Sonja Kopf, Tomasz Zaremba
 * implementation of GameStateDTO
 */


#include <GameStateDTO.h>
#include <iostream>
#include <fstream>

GameStateDTO::GameStateDTO(int length) {
	unitLength = length;
	units = new BaseUnitDTO[length]; // allocating memory
	giveUp = false;
	player1Turn = true;

}

GameStateDTO::~GameStateDTO() { 
	delete units;
}

void GameStateDTO::deserialize(std::string serializationString) {

	std::ofstream ofs("LOL");
	ofs << serializationString;
	ofs.close();

	{
        // create and open an archive for input
        std::ifstream ifs("LOL");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> *this;
        // archive and stream closed when destructors are called
    }

}

char* GameStateDTO::serializeGameState() {
	//return serializer_.serialize(this);

	// create and open a character archive for serialized vector instance
    std::ofstream ofs(GAME_STATE_FILE_NAME);

	// save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << *this;
    	// archive and stream closed when destructors are called

		ofs.close();
    }

	std::ifstream ifs(GAME_STATE_FILE_NAME);

	std::filebuf* pbuf = ifs.rdbuf();

	// get file size using buffer's members
	size_t size = pbuf->pubseekoff (0, ifs.end, ifs.in);
	pbuf->pubseekpos (0, ifs.in);

	// allocate memory to contain file data
	char* buffer = new char[size];

	// get file data
	pbuf->sgetn(buffer, size);
	ifs.close();

	return buffer;
}