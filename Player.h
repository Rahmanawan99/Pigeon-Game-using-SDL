#pragma once
#include"GameObject.h" 
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>
#include "MSound.h"

class Player : public GameObject
{
	enum STATE
	{
		IN_ROOM,
		IN_VENT
	};
	STATE state = IN_ROOM;

	int mouseX;
	int mouseY;

	int stayTime = 15;
	float countStayTime = 0;
	int numberOfDocCollected = 0; // doc collected
public:

	Player(int x, int y, int speed) 
		:GameObject(x,y,speed,new SpriteSheet("assets/bird.png",4,1,4,32,32),PLAYER,1)
	{
		spriteSheet->setCurrent(2); // right
	}

	int getStayTime() {
		return stayTime;
	}
	void setStayTime(int stayTime) {
		this->stayTime = stayTime;
	}

	void collect() {
		numberOfDocCollected++;
	}
	int getNumberOfDoc() {
		return numberOfDocCollected;
	}

	void update() {
		x += dirX * speed;
		y += dirY * speed;

		if (state == IN_ROOM) { // bird in room
			countStayTime += Util::get()->deltaTime;
			if (countStayTime >= 1000) { // countTime >= 1 second 
				countStayTime = 0;
				stayTime--; 
				if (stayTime <= 3) { // 3 seconds left
					Util::get()->sound.playSound(MSound::SOUNDNAME::THREE_SECONDS_LEFT);
				}
				if (stayTime <= 0) { // 15 to 0
					Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
					Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
					Util::get()->gameStatus = Util::GAMEOVER;
				}
			}
		}
		else { // bird in vent
			countStayTime = 0;
			stayTime = 15;
		}

		/*if (numberOfDocCollected = 2) {
			Util::get()->gameStatus = Util::GAMEWON;
		}*/

		if (y <= 52 || x >= 653 || y >= 648) { // bird in vent
			state = IN_VENT;
		}
		else { // bird in room
			state = IN_ROOM;
		}
		
	}

	void draw() {
		spriteSheet->follow(x, y);
		spriteSheet->draw();

		Util::get()->showText(365, 275, to_string(stayTime));
		Util::get()->showText(10, 50, to_string(numberOfDocCollected));
	}

	void onKeyDown(SDL_Keycode key) { 
		if (key == SDLK_a) {
			dirX = -1;
			spriteSheet->setCurrent(2);
		}
		else if (key == SDLK_d) {
			dirX = 1;
			spriteSheet->setCurrent(3);
		}
		if (key == SDLK_w) {
			dirY = -1;
			spriteSheet->setCurrent(0);
		}
		else if (key == SDLK_s) {
			dirY = 1;
			spriteSheet->setCurrent(1);
		}
	}
	void onKeyUp(SDL_Keycode key) {
		if (key == SDLK_a && dirX == -1) {
			dirX = 0;
		}
		else if (key == SDLK_d && dirX == 1) {
			dirX = 0;
		}
		if (key == SDLK_w && dirY == -1) {
			dirY = 0;
		}
		else if (key == SDLK_s && dirY == 1) {
			dirY = 0;
		}
	}
};

