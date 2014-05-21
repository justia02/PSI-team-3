#include "Serializer.h"

Serializer::Serializer(void)
{
}

Serializer::~Serializer(void)
{
}

// wth ... can't i make the parameter to be just ANY object? -> consultancy?! 
char* Serializer::serializeObj(void* anyobj)
{
// create and open a character archive for serialized vector instance
    std::ofstream ofs(SERIALIZATION_FILE_NAME);

	// save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        // oa << anyobj;
		// oa << *	this;
    	// archive and stream closed when destructors are called

		ofs.close();
    }

	std::ifstream ifs(SERIALIZATION_FILE_NAME);

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
