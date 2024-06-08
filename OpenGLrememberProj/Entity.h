#pragma once
#include <ctime>
#include "Primitives.h"
#include "EntityContainer.h"

// Маленькая величина для создания пуль чуть дальше от тела моба/игрока
const double epsilon = 0.2;

// Своего рода "душа" сущности.
class Entity
{
protected:
	int health;
	EntityContainer* host;
	Entity(int hp, EntityContainer* hst) {
		health = hp;
		host = hst;
	}
public:
	virtual char Update() = 0;
};


class Projectile : public Entity {
	friend class Mob;
	friend class Player;
	float speed;
	int time;
	Sphere body;
public:
	Projectile(int hp, float spd, Vector3 pos, float angle, EntityContainer* hst, int t = 100) : Entity(hp, hst) {
		body.angle = angle;
		body.pos = pos;
		speed = spd;
		body.move(0, 0, body.scale.Z());
		body.scale = body.scale * 0.18;
		time = 100;
		host->projs.push_back(*this);
	}
	char Update() {
		if (!(--time)) {
			return 1;
		}
		body.Show();
		body.pos = body.pos + Vector3(cos(body.angle) * speed, sin(body.angle) * speed, 0);
		return 0;
	};
};


class Mob : public Entity {
protected:
	int damage;
	float speed;
	clock_t last, delay;
public:
	Sphere body;
	Mob(int hp, int dmg, float spd, Vector3 pos, float phi, EntityContainer* hst, clock_t dlay=500) : Entity(hp, hst) {
		damage = dmg;
		body.pos = pos + Vector3(0, 0, body.scale.Z());
		body.angle = phi;
		//body.RenderObject();
		speed = spd;
		delay = dlay;
		last = clock();
		host->mobs.push_back(*this);
	}

	void Shoot() {
		clock_t now = clock();
		if (now - last >= delay) {
			last = now;
			Projectile(damage, .6, body.pos + Vector3(cos(body.angle) * (body.scale.X() + epsilon), sin(body.angle) * (body.scale.Y() + epsilon), -body.scale.Z()), body.angle, host, 100);
		}
	}

	char DetectHit() {
		for (auto p = host->projs.begin(); p != host->projs.end(); ) {
			Projectile pr = *p;
			if ((body.pos - pr.body.pos).length() > body.scale.X() + pr.body.scale.X()) {
				p++;
				continue;
			}
			health -= pr.health;
			host->projs.erase(p++);
			if (health <= 0) {
				return 1;
			}
		}
		return 0;
	}

	char Update() {
		if (DetectHit()) {
			return 1;
		}
		body.Show();

		body.pos = body.pos + Vector3(cos(body.angle), sin(body.angle), 0);
		body.angle += 0.2;
		return 0;
	};
};

