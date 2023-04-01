#pragma once
#include"GameObject.h" 
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>
#include "MSound.h"

class Table : public GameObject
{
	enum STATE
	{
		SHOW,
		HIDE,
		DONE
	};
	STATE state = HIDE;

	SpriteSheet* tableShow;
	SpriteSheet* doc1;
	SpriteSheet* doc2;

	int timeLeft = 10;
	float countTime = 0;

	int real;

	GameObject* player = nullptr;
public:

	Table(int x, int y, int speed,string doc1Path, string doc2Path,int real)
		:GameObject(x, y, speed, new SpriteSheet("null.png", 1, 1, 1, 84, 84), TABLE, 1)
	{
		spriteSheet->setCurrent(0);
		spriteSheet->follow(x, y);

		tableShow = new SpriteSheet("assets/tableShow.png", 1, 1, 1, 700, 700); // wood background
		doc1 = new SpriteSheet(doc1Path.c_str(), 1, 1, 1, 300, 400); // doc 1
		doc2 = new SpriteSheet(doc2Path.c_str(), 1, 1, 1, 300, 400); // doc2

		this->real = real;

		tableShow->setCurrent(0);
		tableShow->follow(0, 0);
		doc1->setCurrent(0);
		doc1->follow(30, 150);
		doc2->setCurrent(0);
		doc2->follow(370, 150);
	}

	void update() {
		if (state == SHOW) {
			countTime += Util::get()->deltaTime;
			if (countTime >= 1000) {
				countTime = 0;
				timeLeft--;

				if (timeLeft <= 3) {
					Util::get()->sound.playSound(MSound::SOUNDNAME::THREE_SECONDS_LEFT);
				}
				if (timeLeft <= 0) {
					Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
					Util::get()->gameStatus = Util::GAMEOVER;
				}
			}
		}
	}

	void onCollide(GameObject* other) {
		if (state == DONE) { // become wall if done
			player = other;
			SDL_Rect playerRect = other->getRect();
			SDL_Rect wallRect = getRect();

			// stop player
			// collide like the wall

			if (playerRect.y + playerRect.h > wallRect.y && wallRect.y > playerRect.y)
			{
				float diff = playerRect.y + playerRect.h - wallRect.y;
				player->setXY(player->getX(), playerRect.y - diff);
			}
			else if (playerRect.y + playerRect.h < wallRect.y + wallRect.h && playerRect.x + playerRect.w > wallRect.x && playerRect.x < wallRect.x) {
				float diff = playerRect.x + playerRect.w - wallRect.x;
				player->setXY(playerRect.x - diff, playerRect.y);
			}
			else if (playerRect.y + playerRect.h < wallRect.y + wallRect.h && playerRect.x < wallRect.x + wallRect.w && playerRect.x + playerRect.w > wallRect.x) {
				float diff = wallRect.x + wallRect.w - playerRect.x;
				player->setXY(playerRect.x + diff, playerRect.y);
			}
			else if (wallRect.y + wallRect.h > playerRect.y && playerRect.y > wallRect.y)
			{
				float diff = wallRect.y + wallRect.h - playerRect.y;
				player->setXY(player->getX(), playerRect.y + diff);
			}
			return;
		}

		if (other->getType() == PLAYER) {
			state = SHOW;
			player = other;
			((Player*)player)->setStayTime(15);
		}
	}

	void onMouseDown(int x, int y) {
		if (state == HIDE || state == DONE) {
			return;
		}	

		if (GameObject::checkClickCollide(doc1->getRect(), x, y)) {
			player->setXY(this->x - 50, this->y + 50);
			state = HIDE;
			if (real == 0) {
				((Player*)player)->collect(); // collect real doc
				state = DONE;
			}
			else { // wrong doc - game over
				Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT); 
				Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
				Util::get()->gameStatus = Util::GAMEOVER;
				Util::get()->sound.playMenuMusic();
			}
		}
		else if (GameObject::checkClickCollide(doc2->getRect(), x, y)) {
			player->setXY(this->x - 50, this->y + 50);
			state = HIDE;
			if (real == 1) {
				((Player*)player)->collect(); // collect real doc
				state = DONE;
			}
			else { // wrong doc - game over
				Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
				Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
				Util::get()->gameStatus = Util::GAMEOVER;
				Util::get()->sound.playMenuMusic();
			}
		}
	}

	void draw() {
		if (Util::get()->DEBUG) {
			drawRectangle(getRect());
		}

		if (state == SHOW) {
			tableShow->draw();
			doc1->draw();
			doc2->draw();

			Util::get()->showText(200, 50, "Time: " + to_string(timeLeft));
		}
	}
};

