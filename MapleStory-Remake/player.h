#pragma once

#include "hitbox.h"
#include "input.h"

enum PLAYER_STATE {
	idle = 1, walking = 2
};

class ENTITY {

protected:
	SDL_Rect animclips[10];
	float max_frames = 0;
	float current_frame = 0.0f;
	PLAYER_STATE state = idle;

	void addAnimation(SDL_Rect* clip, int row, int cnt, int w, int h)
	{
		for (int i = 0; i < cnt-1; i += 1) {
			clip[i].x = 0 + i * w;
			clip[i].y = row * h;
			clip[i].w = w;
			clip[i].h = h;
		}
	}

public:

	KeyInput* KeyboardInput = new KeyInput();
	SDL_Surface* playerSurface = nullptr;
	SDL_Rect* playerRect = new SDL_Rect();
	HITBOX collider;
	float deltaTime = 0.0f;

	void playAnimation(SDL_Surface* windowSurface) {
		if (current_frame >= max_frames - 1) {
			current_frame = 0;
		}
		SDL_BlitSurface(playerSurface, &animclips[static_cast<int>(current_frame)], windowSurface, playerRect);
		

		current_frame += deltaTime;
		
	}


	void playerMotorize(const SDL_Event & event) {
		if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_LEFT)) {
			playerRect->x -= 1;
			state = walking;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_RIGHT)) {
			playerRect->x += 1;
			state = walking;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_DOWN)) {
			playerRect->y += 1;
			state = walking;
		}
		else if (this->KeyboardInput->isKeyHeld(event, SDL_SCANCODE_UP)) {
			playerRect->y -= 1;
			state = walking;
		}
		else {
			state = idle;
		}
	}

	ENTITY() {
		
	}
};

class ENTITY_LOGIC {
	ENTITY entity;
public:
	ENTITY_LOGIC() {

	}
};

class PLAYER : public ENTITY {

public:
	PLAYER() : ENTITY() {
		max_frames = 7.0f;
		playerSurface = SDL_LoadBMP("character.bmp");

		if (playerSurface == NULL) {
			printf("SDL Error: %s", SDL_GetError());
		}

		playerRect->w = 64;
		playerRect->h = 64;

		addAnimation(animclips, 0, 7, 64, 64);

		playerSurface->format->Amask = 0xFF000000;
		playerSurface->format->Ashift = 24;

		this->deltaTime = 0.1f;
	}
};

class NPC : public ENTITY {

public:
	NPC() : ENTITY() {
	}
};

class NPC_JAMIE : public NPC {

	PLAYER target;

	bool targetIsRightOf() {
		return (target.playerRect->x > this->playerRect->x);
	}

	bool targetIsLeftOf() {
		return (target.playerRect->x < this->playerRect->x);
	}

	int lastPositionOf = 0;
public:
	SDL_Surface* winSurface;
	void scanTarget() {
		collider.setCollider(*playerRect, 1, 2, winSurface);

		if (targetIsLeftOf()==true) {
			if (lastPositionOf == 0) {
				printf("Left of target!\n");
				lastPositionOf = 1;
			}
		}else{
			if (lastPositionOf == 1) {
				printf("Right of target!\n");
				lastPositionOf = 0;
			}
		}
	}

	void setTarget(PLAYER pl) {
		target = pl;
	}

	NPC_JAMIE(SDL_Surface* windowSurfaceRef = NULL) : NPC() {
		max_frames = 7.0f;
		playerSurface = SDL_LoadBMP("jamie.bmp");

		if (playerSurface == NULL) {
			printf("SDL Error: %s", SDL_GetError());
		}

		if (windowSurfaceRef != NULL) {
			winSurface = windowSurfaceRef;
		}

		playerRect->w = 75;
		playerRect->h = 89;
		playerRect->x = 200;
		playerRect->y = 70;

		addAnimation(animclips, 0, 10, 75, 89);
		playerSurface->format->Amask = 0xFe000000;
		playerSurface->format->Ashift = 24;
		Uint32 color = SDL_MapRGB(playerSurface->format, 0xff, 0x00, 0x00);
		SDL_SetColorKey(playerSurface, color, SDL_MapRGB(playerSurface->format, 0xff, 0x0, 0x00));

		this->deltaTime = 0.03f;
		
	}
};



class MOB : public ENTITY {

public:
	MOB() : ENTITY() {
	}
};

enum MOB_ENTITY_TYPE {
	ShadeWraith=0
};

class MOB_ENTITY : public MOB {

	PLAYER target;

	bool targetIsRightOf() {
		return (target.playerRect->x > this->playerRect->x);
	}

	bool targetIsLeftOf() {
		return (target.playerRect->x < this->playerRect->x);
	}

public:
	MOB_ENTITY_TYPE type;
	SDL_Surface* winSurface;
	void scanTarget() {
		collider.setCollider(*playerRect, 20, 40, winSurface);
		if (targetIsLeftOf() == true) {
			printf("Left of target!\n");
		}
		else {
			printf("Right of target\n");
		}
	}

	void setTarget(PLAYER pl) {
		target = pl;
	}

	MOB_ENTITY(SDL_Surface* windowSurfaceRef, std::string initSpriteSheet, float maxFrames, int w, int h, int x, int y, MOB_ENTITY_TYPE entityType) : MOB() {
		max_frames = maxFrames;
		playerSurface = SDL_LoadBMP(initSpriteSheet.c_str());

		if (playerSurface == NULL) {
			printf("SDL Error: %s", SDL_GetError());
		}

		if (windowSurfaceRef != NULL) {
			winSurface = windowSurfaceRef;
		}

		playerRect->w = w;
		playerRect->h = h;
		playerRect->x = x;
		playerRect->y = y;

		addAnimation(animclips, 0, 6, w, h);
		playerSurface->format->Amask = 0xFe000000;
		playerSurface->format->Ashift = 24;
		Uint32 color = SDL_MapRGB(playerSurface->format, 0xff, 0x00, 0x00);
		SDL_SetColorKey(playerSurface, color, SDL_MapRGB(playerSurface->format, 0xff, 0x0, 0x00));

		this->deltaTime = 0.03f;
		this->type = entityType;

	}
};