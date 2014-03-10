// Authors: Sonja Kopf, Tomasz Zaremba
#include <GameStateDTO.h>

GameStateDTO::GameStateDTO(int length) {

	units = new BaseUnit[length]; // Allocating memory
	giveUp = false;

}

GameStateDTO::~GameStateDTO() { 
	delete units;
}