#include "Serializable.h"
#define GAME_STATE_FILE_NAME "serializationFileGameState"

Serializable::Serializable(void)
{
}


Serializable::~Serializable(void)
{
}

char* Serializable::serializeObject() {
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
