#include <iostream>
#include <stdio.h>
#include <fstream>
#include <gtest/gtest.h>
#include <non-realtime-networking/NonRealtimeNetworkingUtilities.h>
#include <non-realtime-networking/NonRealtimeNetworkingException.h>
#include <string>
#include <sstream>
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

char* webServiceIp = "127.0.0.1";

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

// Register on the server without login - server should assign it automatically and raise no errors
TEST(WEB_SERVICE, REGISTER_NO_LOGIN) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game
	utilities->setGameName("Register no login test");
	// Register without login
	ASSERT_NO_THROW(utilities->registerOnTheServer());
	// Obtained login should look like this:
	std::string expectedLogin("Player");
	std::ostringstream s;
	s << "Player" << utilities->getSessionId();
	// Compare logins
	ASSERT_STREQ(s.str().c_str(), utilities->getLogin());
	
}

// Register on the server with desired login
TEST(WEB_SERVICE, REGISTER_LOGIN) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game
	utilities->setGameName("Register with login test");
	// Set login
	char* login = new char[6];
	strcpy(login, "Tomek");
	utilities->setLogin(login);
	// Register with a given login
	ASSERT_NO_THROW(utilities->registerOnTheServer());
	// Compare logins:
	ASSERT_STREQ(login, utilities->getLogin());
	
}

// Register on the server with a login which is already taken - should return error LOGIN_TAKEN
TEST(WEB_SERVICE, REGISTER_LOGIN_TAKEN) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);

	// Set the name of the game
	utilities1->setGameName("Login already taken register test");
	// Set login of the first player
	utilities1->setLogin("Tomek");
	// Register on the server
	utilities1->registerOnTheServer();

	// Set the name of the game to be exactly the same as above
	utilities2->setGameName("Login already taken register test");
	// Set the same login as the player above
	utilities2->setLogin("Tomek");
	// Try registering on the server, should return LOGIN_TAKEN error code
	ASSERT_EQ(NonRealtimeNetworkingUtilities::LOGIN_TAKEN, utilities2->registerOnTheServer());
	
}

// Remove player from the server
TEST(WEB_SERVICE, REMOVE_PLAYER) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set the name of the game to play
	utilities->setGameName("Remove player test");
	// Register player on the server
	utilities->registerOnTheServer();
	// Remove player from the server
	ASSERT_NO_THROW(utilities->removePlayer());

}

// Obtain opponent's IP address from the server
TEST(WEB_SERVICE, GET_OPPONENTS_IP_ADDRESS) {

	// Initialize the library
	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	// Set name of the game
	utilities1->setGameName("Opponent's IP test");
	// Register on the master server
	ASSERT_NO_THROW(utilities1->registerOnTheServer());

	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	utilities2->setGameName("Opponent's IP test");
	ASSERT_NO_THROW(utilities2->registerOnTheServer());

	// Opponent's IP should be localhost
	ASSERT_STREQ("127.0.0.1", utilities2->getOpponentsIP());

}

// Create a socket connection between two players using the server
TEST(WEB_SERVICE, CONNECT_TWO_PLAYERS) {

	NonRealtimeNetworkingUtilities* utilities1 = new NonRealtimeNetworkingUtilities(webServiceIp);	
	utilities1->setGameName("Connect 2 players test");
	ASSERT_NO_THROW(utilities1->registerOnTheServer());
	utilities1->openServerSocket();

	NonRealtimeNetworkingUtilities* utilities2 = new NonRealtimeNetworkingUtilities(webServiceIp);
	utilities2->setGameName("Connect 2 players test");
	ASSERT_NO_THROW(utilities2->registerOnTheServer());
	utilities2->openClientSocket(utilities2->getOpponentsIP());
	utilities1->acceptClient();

	// Connection should now be established, try sending a string:
	ASSERT_NO_THROW(utilities1->setBuffer("Test buffer"));
	ASSERT_NO_THROW(utilities1->sendData());

	// Receive the string
	ASSERT_NO_THROW(utilities2->receiveData());

	utilities1->closeConnection();
	utilities2->closeConnection();

}

// Retrieve a list of games registered on the server
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

// Check in on the server
TEST(WEB_SERVICE, CHECK_IN) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game
	utilities->setGameName("Check in test");
	// Register
	utilities->registerOnTheServer();
	// Check in
	ASSERT_EQ(0, utilities->checkIn());

}

// Register player's score on the server
TEST(WEB_SERVICE, ADD_HIGH_SCORE) {

// Create library instance to work with the server
NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
// Set name of the game
utilities->setGameName("Add high score test");
// Register on the server
utilities->registerOnTheServer();
// Add score, should return 0 - no error
ASSERT_EQ(0, utilities->addPlayerScore(100));

}

// Retrieve a list of best scores for a game
TEST(WEB_SERVICE, GET_HIGH_SCORES) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game
	utilities->setGameName("Get high scores test");
	// Get a list of scores for a given game
	ASSERT_NO_THROW(utilities->getHighScores());

}

// Retrieve a list of scores for a game which was not registered on the server
TEST(WEB_SERVICE, GET_HIGH_SCORES_NO_SUCH_GAME) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game which does not exist on the server
	utilities->setGameName("Game which does not exist");
	// Obtain a list of scores for that game
	ASSERT_NO_THROW(utilities->getHighScores());

}

// Play a given game again
TEST(WEB_SERVICE, PLAY_AGAIN) {

	// Create library instance to work with the server
	NonRealtimeNetworkingUtilities* utilities = new NonRealtimeNetworkingUtilities(webServiceIp);
	// Set name of the game which does not exist on the server
	utilities->setGameName("Game which does not exist");
	// Register
	utilities->registerOnTheServer();
	// Try to play again
	ASSERT_EQ(0, utilities->playAgain());

}

int main(int argc, char** argv) {
	
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS();
	system("PAUSE");

}