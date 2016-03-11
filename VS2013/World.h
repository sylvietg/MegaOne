#pragma once
#include <vector>

class Camera;
class Model;
/*
class WaterModel;
class ObjectModel;
class Animation;
class AnimationKey;
class ParticleSystem;
class BSpline;
*/
class World
{
public:
	World();
	~World();

	static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);

	const Camera* GetCurrentCamera() const;

private:
	static World* instance;

	std::vector<Model*> mModel;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;

};