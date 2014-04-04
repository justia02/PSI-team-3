#include "BaseUnitDTO.h"
#include <iostream>
#include <fstream>

char* BaseUnitDTO::serialize() {
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
	ifs.close();

	return buffer;
}

void BaseUnitDTO::deserialize(std::string serializationString) {
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