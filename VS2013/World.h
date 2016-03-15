#pragma once
#include "Block.h"
#include <vector>

//size of the world in blocks, the world is currently a 2x2 world (4 blocks)
#define WORLD_HEIGHT 2
#define WORLD_LENGTH 2

class World
{
public:
	World();
	~World();
	void Draw();

private:
	//hold the blocks to be drawn;
	static vector<Block*> blocks;
};