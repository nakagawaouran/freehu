#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "datatypes.h"
#include <unistd.h>

//Dimensions of the logical render size, Timing for game logic
const int WIDTH = 640, HEIGHT = 480;
const int FRAME_RATE_CAP = 60;
const int TICKS_PER_FRAME = 1000 / FRAME_RATE_CAP;

int main(int argc, char const *argv[]) {

  //Initalizing SDL and TTF
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();

  //2x The game if monitor is big enough
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  int screenWidth = WIDTH, screenHeight = HEIGHT;
  if(current.w >= WIDTH*2 && current.h >= HEIGHT*2){
    screenWidth = WIDTH*2;
    screenHeight = HEIGHT*2;
  }

  //Create the Window, Renderer
  SDL_Window *window = SDL_CreateWindow("2hu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
  SDL_Renderer *renderMain = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderMain, WIDTH, HEIGHT);

  //TESTTESTETST
  player = playerClean;
  SDL_Rect rect = {0,0,400,400};
  chdir("rez");
  SDL_Surface *temp = SDL_LoadBMP("test.bmp");
  player.texture = SDL_CreateTextureFromSurface(renderMain, temp);
  SDL_FreeSurface(temp);

  bulletSprites[0].width = loadTransparent(renderMain, "1.bmp", &bulletSprites[0].texture);

  chdir("..");

  //Main Game Loop
  int gameRunning = 1;
  SDL_Event event;
  while(gameRunning){


    SDL_SetRenderDrawColor(renderMain, 23, 23 ,23, 0xFF);
    SDL_RenderClear(renderMain);

    updatePlayer(player, keyState);
    renderPlayer(renderMain, player);
    renderBullets(sizeof(activePlayerBullets)/sizeof(activePlayerBullets[0]), renderMain, activePlayerBullets);
    renderGUI(renderMain, player);
    SDL_RenderPresent(renderMain);

  //  printf("UP:%d DOWN:%d LEFT:%d RIGHT:%d SHOOT:%d\n",keyState.up,keyState.down,keyState.left,keyState.right,keyState.shoot);


    //Get User Input
    SDL_PollEvent(&event);
    switch (event.type) {

      //Quit the game through the window
      case SDL_QUIT:
        SDL_Quit();
        exit(1);
        break;

      //Pressing down a keyboard key and setting the key manager to the correct state
      case SDL_KEYDOWN:
        if(event.key.keysym.sym ==  SDLK_UP){
          keyState.up = 1;}

        if(event.key.keysym.sym ==  SDLK_DOWN){
          keyState.down = 1;}

        if(event.key.keysym.sym ==  SDLK_RIGHT){
          keyState.right = 1;}

        if(event.key.keysym.sym ==  SDLK_LEFT){
          keyState.left = 1;}

        if(event.key.keysym.sym ==  SDLK_z){
          keyState.shoot = 1;}

        if(event.key.keysym.sym ==  SDLK_LSHIFT){
          keyState.focus = 1;}

      break;

      //Releasing a keyboard key
      case SDL_KEYUP:
        if(event.key.keysym.sym ==  SDLK_UP){
          keyState.up = 0;}

        if(event.key.keysym.sym ==  SDLK_DOWN){
          keyState.down = 0;}

        if(event.key.keysym.sym ==  SDLK_RIGHT){
          keyState.right = 0;}

        if(event.key.keysym.sym ==  SDLK_LEFT){
          keyState.left = 0;}

        if(event.key.keysym.sym ==  SDLK_z){
          keyState.shoot = 0;}

        if(event.key.keysym.sym ==  SDLK_LSHIFT){
          keyState.focus = 0;}

      break;

    }

  }



  SDL_RenderClear(renderMain);
  SDL_RenderPresent(renderMain);
  SDL_SetRenderDrawColor(renderMain, 222, 222, 222 ,0xFF);
  SDL_RenderFillRect(renderMain, &rect);
  SDL_RenderPresent(renderMain);

  SDL_Delay(2000);
  printf("You");
  return 0;
}

void updatePlayer(struct PlayerData p, struct KeyDownState ks){
  //Decrease Invinciblity Frame each tick + 1 tick closer to being able to shoot
  player.iframe--;
  player.bulletCD--;

  //Move player, cut velocity in half if they are moving in 2 directions at once
  int tempX = 0, tempY = 0, mod = 0;
  if(ks.up == 1){
    tempY-=player.vel;
    mod++;}
  if(ks.down == 1){
    tempY+=player.vel;
    mod++;}
  if(ks.left == 1){
    tempX-=player.vel;
    mod++;}
  if(ks.right == 1){
    tempX+=player.vel;
    mod++;}
  if(mod == 2){
    tempX*=0.5;
    tempY*=0.5;}
  if(ks.focus == 1){
    tempX*=0.5;
    tempY*=0.5;}
  player.rect.x+=tempX;
  player.rect.y+=tempY;

  //printf("tempx:%d tempy:%d\n",tempX,tempY);

  //Check if player is out of bounds, if they are then set them to the borders
  //The horizontal borders are intentionally set to be equal to height to create a square playing area
  if(player.rect.x < 0){
    player.rect.x = 0;
  }else if(player.rect.x > HEIGHT - player.rect.w){
    player.rect.x = HEIGHT - player.rect.w;
  }
  if(player.rect.y < 0){
    player.rect.y = 0;
  }else if(player.rect.y > HEIGHT - player.rect.h){
    player.rect.y = HEIGHT - player.rect.h;
  }

  //Check to see if the player can shoot, they have to be holding down shoot key plus not be on a cooldown
  if(player.bulletCD <= 0 && ks.shoot == 1){
    struct BulletData temp = { {player.rect.x + ( player.rect.w / 2 ) - 8, player.rect.y, 16, 16 }, 0, -5, 0, -20, 0, 0 ,0, 1};
    createBullet(sizeof(activePlayerBullets)/sizeof(activePlayerBullets[0]), activePlayerBullets, temp);

    player.bulletCD = 10;
  }

}

void renderPlayer(SDL_Renderer *r, struct PlayerData p){
  //Checks to see if the player is in Invinciblity OR
  //If they are then check to see if its a draw frame (for blink effect, can set higher for longer time in between occurnces)
  if(p.iframe < 0 || p.iframe % 2 == 0){
    SDL_RenderCopy(r, p.texture, NULL, &p.rect);
  }
}

void renderGUI(SDL_Renderer *r, struct PlayerData p){
  SDL_SetRenderDrawColor(r,124,124,124,0xFF);
  SDL_Rect sidebar = {HEIGHT, 0, WIDTH-HEIGHT, HEIGHT};
  SDL_RenderFillRect(r, &sidebar);
}

void renderBullets(int size, SDL_Renderer *r, struct BulletData activeList[]){
  for(int i = 0; i < size; i++){
    if(activeList[i].alive == 1){
      //Move the bullets
      activeList[i].rect.x+=activeList[i].velX;
      activeList[i].rect.y+=activeList[i].velY;

      //Check if bullets are out bounds
      SDL_Rect checkBounds = activeList[i].rect;
      if(checkBounds.x < 0 - checkBounds.w){
        activeList[i].alive = 0;
      }else if(checkBounds.x > HEIGHT){
        activeList[i].alive = 0;
      }
      if(checkBounds.y < 0 - checkBounds.h){
        activeList[i].alive = 0;
      }else if(checkBounds.y > HEIGHT){
        activeList[i].alive = 0;
      }

      if (activeList[i].alive == 1){
      SDL_RenderCopy(r, bulletSprites[activeList[i].textureID].texture, NULL, &activeList[i].rect);
    }
  }
  }
}

void createBullet(int size, struct BulletData activeList[], struct BulletData bulletData){
  for(int i = 0; i < size; i++){
    if(activeList[i].alive != 1){
      activeList[i] = bulletData;
      activeList[i].alive = 1;
      return 0;
    }
  }
}

int loadTransparent(SDL_Renderer *r, char path[], SDL_Texture **target){
  SDL_Surface *temp;
  int val = 0;
  if((temp = SDL_LoadBMP(path)) != NULL){
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0xFF, 0, 0xFF) );
    *target = SDL_CreateTextureFromSurface(r, temp);
    val = temp->w / 8;
  }
  SDL_FreeSurface(temp);
  return val;
}
