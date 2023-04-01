#pragma once
#include <iostream>
#include "Map.h"
#include "Table.h"
#include "Guard.h"
#include "Wall.h"
#include "GreenDoor.h"
#include "DoublyLinkedList.h"

using namespace std;

class Game {

	Map map;
	GameObject* player;
	dLinkedList* objects;

public:
	Game() {
		Util::get()->sound.playMenuMusic();
	}

	void startGame() {
		objects = new dLinkedList;
		player = new Player(100, 200, 4);
		GameObject* table1 = new Table(258, 250, 0,"assets/fake3.png", "assets/real3.png",1); // table
		GameObject* table2 = new Table(560, 57, 0, "assets/real4.png", "assets/fake4.png",0);
		GameObject* guard1 = new Guard(141, 510, 1, 3000, player,160);
		GameObject* guard2 = new Guard(424, 423, 1, 1500, player,90);

		GameObject* wall1 = new Wall(355, 51, 55, 500); // turn debug = true in utils.h to see them
		GameObject* wall2 = new Wall(121, 50, 530, 5);
		GameObject* wall3 = new Wall(0, -5, 962, 10);
		GameObject* wall4 = new Wall(697, 0, 6, 962);
		GameObject* wall5 = new Wall(650, 50, 5, 230);
		GameObject* wall6 = new Wall(0, 698, 962, 10);
		GameObject* wall7 = new Wall(189, 648, 460, 5);
		GameObject* wall8 = new Wall(650, 330, 5, 324);
		GameObject* wall9 = new Wall(0, 0, 6, 962);

		GameObject* exit = new GreenDoor(-5, 250, player);


		// add all objects to linked list

		objects->headInsert(table1);
		objects->headInsert(table2);

		objects->headInsert(player);

		objects->headInsert(guard1);
		objects->headInsert(guard2);

		objects->headInsert(wall1);
		objects->headInsert(wall2);
		objects->headInsert(wall3);
		objects->headInsert(wall4);
		objects->headInsert(wall5);
		objects->headInsert(wall6);
		objects->headInsert(wall7);
		objects->headInsert(wall8);
		objects->headInsert(wall9);

		objects->headInsert(exit);
		Util::get()->sound.playGameMusic();
	}

	void update() {
		if (Util::get()->gameStatus == Util::GAMEOVER) { // return if game over
			return;
		}
		if (Util::get()->gameStatus == Util::MENU) {
			return;
		}

		for (node* n = objects->head; n != NULL; n = n->next) {
			GameObject* obj = n->data;
			obj->update();
			if (obj->isDead()) {
				objects->deleteNode(n);
				break;
			}
		}

		checkCollide();
	}

	void onKeyDown(SDL_Keycode key) {
		if (Util::get()->gameStatus == Util::GAMEOVER) {
			if (key == SDLK_SPACE) { // press space to start the game
				Util::get()->gameStatus = Util::PLAYING;
				Mix_HaltChannel(-1);
				startGame();
			}
			return;
		}
		if (Util::get()->gameStatus == Util::MENU) {
			if (key == SDLK_SPACE) { // press space to start the game
				Util::get()->gameStatus = Util::PLAYING;
				Mix_HaltChannel(-1);
				startGame();
			}
			return;
		}

		((Player*)player)->onKeyDown(key);
	}
	void onKeyUp(SDL_Keycode key) {
		if (Util::get()->gameStatus == Util::GAMEOVER) {
			return;
		}
		if (Util::get()->gameStatus == Util::MENU) {
			return;
		}
		((Player*)player)->onKeyUp(key);
	}
	void onMouseDown(int x, int y) {
		if (Util::get()->gameStatus == Util::GAMEOVER) {
			return;
		}
		if (Util::get()->gameStatus == Util::MENU) {
			return;
		}
		for (node* n = objects->head; n != NULL; n = n->next) {
			GameObject* obj = n->data;
			obj->onMouseDown(x, y);
		}
	}

	void draw() {
		if (Util::get()->gameStatus == Util::GAMEOVER) {
			Util::get()->showText(250, 100, "Mission faled.");
			Util::get()->showText(250, 300, "No Problem Son.");
			Util::get()->showText(50, 325, "You may give it another go. May God be with you.");
			Util::get()->showText(150, 500, "Press 'Space' to retry!"); 
			return;
		}
		if (Util::get()->gameStatus == Util::MENU) {
			Util::get()->showText(225, 100, "Pigeon Impossible");
			Util::get()->showText(20, 200, "Agent! Your mission, should you choose to accept,");
			Util::get()->showText(20, 225, "is to recover classified documents from a RAW");
			Util::get()->showText(20, 250, "facility that holds secrets that could be");
			Util::get()->showText(20, 275, "detrimental to our country's safety and economy."); 
			Util::get()->showText(20, 300, "There are patrolmen on duty that may spot and"); 
			Util::get()->showText(20, 325, "shoot you in 10 seconds. They have missed the");
			Util::get()->showText(20, 350, "real document with a fake one so beware!");
			Util::get()->showText(150, 500, "Press 'Space' to accept mission");
			return;
		}
		/*if (Util::get()->gameStatus == Util::GAMEWON) {
			Util::get()->showText(250, 100, "Mission Success!");
			Util::get()->showText(250, 300, "Good Work Son.");
			Util::get()->showText(50, 325, "You just saved Pakistan!");
			Util::get()->showText(150, 500, "Press 'Space' to play again!");
			return;
		}*/

		// in game
		map.draw();
		for (node* n = objects->head; n != NULL; n = n->next) { // draw all objects
			GameObject* obj = n->data;
			obj->draw();
		}
	}

	void checkCollide() { // check collision for all objects

		for (node* n = objects->head; n != NULL; n = n->next) {
			GameObject* obj = n->data;

			for (node* m = objects->head; m != NULL; m = m->next) {
				GameObject* otherObj = m->data;
				if (obj == otherObj) {
					continue;
				}
				if (obj->checkCollide(otherObj)) { // check collide 
					obj->onCollide(otherObj); // if collide call onCollide
				}
			}
		}
	}
};
