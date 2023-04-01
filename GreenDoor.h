#pragma once
#include"GameObject.h" 
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>
#include "MSound.h"
#include "Player.h"

class GreenDoor : public GameObject
{
	enum STATE
	{
		SHOW,
		HIDE
	};
	STATE state = HIDE;

	GameObject* player = nullptr;
public:

	GreenDoor(int x, int y, GameObject* player)
		:GameObject(x, y, speed, new SpriteSheet("assets/exitVent.png", 1, 1, 1, 70, 120), TABLE, 1)
	{
		spriteSheet->setCurrent(0);
		spriteSheet->follow(x, y);
		this->player = player;
	}

	void update() {
		if (((Player*)player)->getNumberOfDoc() == 2) { // check player collect all
			state = SHOW;
		}
	}

	void onCollide(GameObject* other) {
		if (state == HIDE && other->getType() == PLAYER) { // is wall
			player = other;
			SDL_Rect playerRect = other->getRect();
			SDL_Rect wallRect = getRect();
			// 2 box collide

			// stop player
			if (playerRect.y + playerRect.h > wallRect.y && wallRect.y > playerRect.y) // collide top
			{
				float diff = playerRect.y + playerRect.h - wallRect.y;
				player->setXY(player->getX(), playerRect.y - diff);
			}
			// colllide left
			else if (playerRect.y + playerRect.h < wallRect.y + wallRect.h && playerRect.x + playerRect.w > wallRect.x && playerRect.x < wallRect.x) {
				float diff = playerRect.x + playerRect.w - wallRect.x;
				player->setXY(playerRect.x - diff, playerRect.y);
			}
			// collide right
			else if (playerRect.y + playerRect.h < wallRect.y + wallRect.h && playerRect.x < wallRect.x + wallRect.w && playerRect.x + playerRect.w > wallRect.x) {
				float diff = wallRect.x + wallRect.w - playerRect.x;
				player->setXY(playerRect.x + diff, playerRect.y);
			}
			// collide bottom
			else if (wallRect.y + wallRect.h > playerRect.y && playerRect.y > wallRect.y)
			{
				float diff = wallRect.y + wallRect.h - playerRect.y;
				player->setXY(player->getX(), playerRect.y + diff);
			}
			return;
		}

		if (other->getType() == PLAYER) {
			Util::get()->gameStatus = Util::MENU;
		}
	}

	void draw() {
		if (state == SHOW) { // door turn green
			spriteSheet->draw();
		}
	}
};

