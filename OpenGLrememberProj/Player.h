#pragma once
#include "Entity.h"

const double pi05 = 1.57079632679;

class Player : public Mob {
	friend class Projectile;
	friend class CustomCamera;
	char flag = 1;
public:
	Player(int hp, int dmg, float spd, Vector3 pos, float phi, EntityContainer* host, clock_t dlay = 500) : Mob(hp, dmg, spd, pos, phi, host, dlay) {
		host->mobs.pop_back(); //���������� ������� ����, ������������� �� ����� �� �����.
	}
	void Control();
	char Update() {
		if (DetectHit()) {
			return 1;
		}
		body.Show();
		Control();
		return 0;
	}
};
