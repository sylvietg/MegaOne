#pragma once
#include "World.h"

vector<Block*> World::blocks;

World::World(){
	//set the camera here


	//populates the world with a # of blocks
	//can be changed to have a random number of blocks, or take input 
	GLuint counter = 0;
	for (int i = 0; i < WORLD_HEIGHT; i++){
		for (int j = 0; j < WORLD_LENGTH; j++){
			Block* b = (new Block(i,j));
			blocks.push_back(b);
		}
	}
}

World::~World(){
	for (int i = 0; i < blocks.size(); i++){
		delete blocks.at(i);
		blocks.at(i) = nullptr;
	}
	blocks.clear();
}


void World::Draw(){
	//gotta find a more efficient way to draw the blocks
	for (int i = 0; i < blocks.size(); i++){
		blocks.at(i)->draw();
	}
}