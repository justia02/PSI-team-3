#include <iostream>
#include <stdio.h>
#include <fstream>
#include <gtest/gtest.h>
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include <non-realtime-networking/NonRealtimeNetworkingException.h>
#include <string>
#include "Vector.h"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

using namespace irrlicht_nonrealtimenetworking;

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, Vector & v, const unsigned int version)
		{
			ar & v.x;
			ar & v.y;	
		}

	}
}

char* webServiceIp = "145.109.198.191";

TEST(UTILITIES, SET_BUFFER) // Check if our memory allocation works fine
{
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities();
	utilities->setBuffer("Test");
	ASSERT_STREQ("Test", utilities->getBuffer());
}

TEST(UTILITIES, SET_PORT_NUMBER) // Try to assign port number smaller than 0 - should raise an exception
{
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities();
	ASSERT_THROW(utilities->setPortNumber(-1), NonRealtimeNetworkingException);
}

TEST(WINSOCK, OPEN_SERVER_CONN) // Try to open a server socket
{
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities();
	ASSERT_NO_THROW(utilities->openServerSocket());
	utilities->closeConnection();
}

TEST(WINSOCK, OPEN_CLIENT_CONN) // Try to open a client socket
{
	NonRealtimeNetworkingUtilities* utilitiesSrv = new NonRealtimeNetworkingUtilities();
	utilitiesSrv->setPortNumber(6);
	ASSERT_NO_THROW(utilitiesSrv->openServerSocket());
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities();
	utilities->setPortNumber(6);
	ASSERT_NO_THROW(utilities->openClientSocket("127.0.0.1"));
	ASSERT_NO_THROW(utilitiesSrv->acceptClient());
	utilities->closeConnection();
	utilitiesSrv->closeConnection();
}

TEST(WINSOCK, SEND_RCV_STRING) // Try sending/receiving of an example string
{
	// Establish a connection between server and client:
	NonRealtimeNetworkingUtilities* utilitiesSrv = new NonRealtimeNetworkingUtilities();
	ASSERT_NO_THROW(utilitiesSrv->openServerSocket());
	NonRealtimeNetworkingUtilities* utilitiesClient = new NonRealtimeNetworkingUtilities();
	ASSERT_NO_THROW(utilitiesClient->openClientSocket("127.0.0.1"));
	ASSERT_NO_THROW(utilitiesSrv->acceptClient());
	
	// Set the buffer on the server side:
	ASSERT_NO_THROW(utilitiesClient->setBuffer("Test buffer"));
	// Send the buffer to the client
	ASSERT_NO_THROW(utilitiesClient->sendData());

	// Receive the buffer from the server:
	ASSERT_NO_THROW(utilitiesSrv->receiveData());

	// Close connections once we're done:
	utilitiesClient->closeConnection();
	utilitiesSrv->closeConnection();
}

TEST(WINSOCK, SEND_RCV_COMPLEX) // serialize a complex object, send via sockets, deserialize received char* 
{
	// create complex vector object
	Vector* vector = new Vector(3, 4);
	float len = vector->getLength();

	// file that serialized object will be saved to on server side
	char* filenameSrv = "serialization_file_srv";

	// create and open a character archive for serialized vector instance
    std::ofstream ofs(filenameSrv);

	// save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << *vector;
    	// archive and stream closed when destructors are called

		ofs.close();
    }

	NonRealtimeNetworkingUtilities* utilitiesSrv = new NonRealtimeNetworkingUtilities();
	ASSERT_NO_THROW(utilitiesSrv->openServerSocket());
	NonRealtimeNetworkingUtilities* utilitiesClient = new NonRealtimeNetworkingUtilities();
	ASSERT_NO_THROW(utilitiesClient->openClientSocket("127.0.0.1"));
	ASSERT_NO_THROW(utilitiesSrv->acceptClient());

	std::ifstream ifs(filenameSrv);

	std::filebuf* pbuf = ifs.rdbuf();

	// get file size using buffer's members
	size_t size = (size_t) pbuf->pubseekoff(0, ifs.end, ifs.in);
	pbuf->pubseekpos (0, ifs.in);

	// allocate memory to contain file data
	char* buffer = new char[size];

	// get file data
	pbuf->sgetn(buffer, size);
	ifs.close();

	// Set the buffer on the server side:
	ASSERT_NO_THROW(utilitiesClient->setBuffer(buffer));
	// Send the buffer to the client
	ASSERT_NO_THROW(utilitiesClient->sendData());

	// Receive the buffer from the server:
	ASSERT_NO_THROW(utilitiesSrv->receiveData());

	char* filenameCl = "serialization_file_client";

	std::ofstream ofs2(filenameCl);
	ofs2 << utilitiesClient->getBuffer();
	ofs2.close();

	Vector newVector;
	{
        // create and open an archive for input
        std::ifstream ifs(filenameCl);
		// printf("hello1");
		// HEEERE is where the shit happens
        boost::archive::text_iarchive ia(ifs);
		// printf("hello!");
        // read class state from archive
        ia >> newVector;
        // archive and stream closed when destructors are called
    }

	ASSERT_TRUE(newVector.getLength() == len);

	// Close connections once we're done:
	utilitiesClient->closeConnection();
	utilitiesSrv->closeConnection();
		
}

TEST(WEB_SERVICE, CONNECT_TWO_PLAYERS) {

	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	utilities1->setGameName("Test game");
	ASSERT_NO_THROW(utilities1->registerOnTheServer());
	utilities1->openServerSocket();

	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);
	utilities2->setGameName("Test game");
	ASSERT_NO_THROW(utilities2->registerOnTheServer());
	utilities2->openClientSocket(utilities2->getOpponentsIpAddress());
	utilities1->acceptClient();

	// Connection should now be established, try sending a string:
	ASSERT_NO_THROW(utilities1->setBuffer("Test buffer"));
	ASSERT_NO_THROW(utilities1->sendData());

	// Receive the string
	ASSERT_NO_THROW(utilities2->receiveData());

	utilities1->closeConnection();
	utilities2->closeConnection();

}

TEST(WEB_SERVICE, GET_OPPONENTS_IP_ADDRESS) {

	// Initialize the library
	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	// Set name of the game
	utilities1->setGameName("Test game");
	// Register on the master server
	ASSERT_NO_THROW(utilities1->registerOnTheServer());

	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	utilities2->setGameName("Test game");
	ASSERT_NO_THROW(utilities2->registerOnTheServer());

	// Opponent's IP should be localhost
	ASSERT_STREQ("145.109.200.21", utilities2->getOpponentsIpAddress());

}

TEST(WEB_SERVICE, GET_GAMES_LIST) {

	// Initialize the library
	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);
	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);
	NonRealtimeNetworkingUtilities* utilities3 = new NonRealtimeNetworkingUtilities(webServiceIp);

	// Set name of the game
	utilities1->setGameName("PSI Team 3");
	// Register on the master server
	utilities1->registerOnTheServer();
	utilities2->setGameName("PSI Team 3 (1)");
	utilities2->registerOnTheServer();
	utilities3->setGameName("PSI Team 3 (2)");
	utilities3->registerOnTheServer();
	// Obtain a list of games played from the server
	ASSERT_NO_THROW(utilities1->getGamesList());
}

int main(int argc, char** argv) {
	
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS();
	system("PAUSE");

}