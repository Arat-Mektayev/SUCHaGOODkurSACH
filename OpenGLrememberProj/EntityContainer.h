#pragma once
#include <list>

class Mob;
class Projectile;

class EntityContainer {
public:
	std::list<Projectile> projs;
	std::list<Mob> mobs;
	void Update();
};
