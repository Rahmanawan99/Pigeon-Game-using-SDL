#pragma once
#include"GameObject.h" 
#include "DoublyLinkedList.h"
#include <iostream>
#include <string>
#include "MSound.h"

class Guard : public GameObject
{
	enum STATE
	{
		REST,

		MOVING
	};
	STATE state = REST;

	int walkTime;
	float countTime = 0;

	GameObject* player = nullptr;
	int flip = 1;
	int distance;
public:

	Guard(int x, int y, int speed,int walkTime, GameObject* player,int distance)
		:GameObject(x, y, speed, new SpriteSheet("assets/guard.png", 1, 1, 1, 43, 84), ENEMY, 1)
	{
		spriteSheet->setCurrent(0); 
		spriteSheet->follow(x, y);
		this->walkTime = walkTime;
		this->player = player;
		this->distance = distance;
	}

	void update() {
		
		x += flip * speed;

		countTime += Util::get()->deltaTime; // move left and right by walkTime
		if (countTime >= walkTime) {
			countTime = 0;
			flip *= -1;
		}

		spriteSheet->follow(x, y);


		// guard see bird
		SDL_Rect playerRect = player->getRect();
		SDL_Rect rect = getRect();
		if (playerRect.y >= rect.y && playerRect.y <= rect.y + rect.h && Util::get()->distance(playerRect.x,playerRect.y,rect.x,rect.y) <= distance) {
			
			if (flip != 1 && playerRect.x < rect.x) // shot player if collide //Sees the player
			{
				Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
				Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
				Util::get()->gameStatus = Util::GAMEOVER;
				Util::get()->sound.playMenuMusic();
			}
			else if (flip == 1 && playerRect.x > rect.x) { // shot player if collide
				Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
				Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
				Util::get()->gameStatus = Util::GAMEOVER;
				Util::get()->sound.playMenuMusic();
			}
			
			
		}

	}

	void onCollide(GameObject* other) {
		if (other->getType() == PLAYER) { // shot player if collide and game ends
			Util::get()->sound.playSound(MSound::SOUNDNAME::SHOT);
			Util::get()->sound.playSound(MSound::SOUNDNAME::SIREN);
			Util::get()->gameStatus = Util::GAMEOVER;
			Util::get()->sound.playMenuMusic();
		}
	}

	void draw() {
		SDL_Rect src = spriteSheet->getRect();
		if (flip != 1) { // draw with flip image
			SDL_RenderCopyEx(Util::get()->renderer, spriteSheet->getTexture(), NULL, &src, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		else { // draw no flip
			SDL_RenderCopyEx(Util::get()->renderer, spriteSheet->getTexture(), NULL, &src, 0, NULL, SDL_FLIP_NONE);
		}
	}
};

