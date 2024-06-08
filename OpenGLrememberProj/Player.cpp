#include "Player.h"
#include "MyOGL.h"

void Player::Control() {
	if (OpenGL::isKeyPressed('W')) {
		body.move(cos(body.angle) * speed, sin(body.angle) * speed, 0);
	}
	// Move backward
	if (OpenGL::isKeyPressed('S')) {
		body.move(-cos(body.angle) * speed, -sin(body.angle) * speed, 0);
	}
	// Strafe right
	if (OpenGL::isKeyPressed('A')) {
		body.move(cos(body.angle + pi05) * speed, sin(body.angle + pi05) * speed, 0);
	}
	// Strafe left
	if (OpenGL::isKeyPressed('D')) {
		body.move(cos(body.angle - pi05) * speed, sin(body.angle - pi05) * speed, 0);
	}

	if (OpenGL::isKeyPressed(VK_LBUTTON)) {
		Shoot();
	}
}