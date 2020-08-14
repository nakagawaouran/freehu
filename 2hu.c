#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "datatypes.h"
#include <unistd.h>
#include <math.h>

//Dimensions of the logical render size, Timing for game logic
const int WIDTH = 640, HEIGHT = 480;
const int FRAME_RATE_CAP = 60;
const int TICKS_PER_FRAME = 1000 / FRAME_RATE_CAP;
const int MAX_FRAMES = 8;

SDL_Rect getAnimPos(int *framePos, int w, int h);

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
  float globalY = 0.0;
  chdir("rez");
  SDL_Surface *temp = SDL_LoadBMP("test.bmp");
  player.texture = SDL_CreateTextureFromSurface(renderMain, temp);
  SDL_FreeSurface(temp);

//  temp = SDL_LoadBMP("bg1.bmp");
//  backgrounds.bGrounds[0] = SDL_CreateTextureFromSurface(renderMain, temp);
  //SDL_FreeSurface(temp);
  loadBackgrounds(renderMain, &backgrounds);


  loadTransparent(renderMain, "2.bmp", &bulletSprites[0].texture, &bulletSprites[0].width, &bulletSprites[0].height);
  printf("height: %d width: %d \n", bulletSprites[0].height, bulletSprites[0].width);
  loadTransparent(renderMain, "2.bmp", &enemySprites[0].texture, &enemySprites[0].width, &enemySprites[0].height);

  chdir("..");

  //Main Game Loop
  int gameRunning = 1;
  SDL_Event event;
  createEnemy(0, 200, 200, sizeof(activeEnemys)/sizeof(activeEnemys[0]));
  while(gameRunning){


    SDL_SetRenderDrawColor(renderMain, 23, 23 ,23, 0xFF);
    SDL_RenderClear(renderMain);
    renderBackground(renderMain, &backgrounds, &globalY);
    updatePlayer(player, keyState);
    updateEnemys(renderMain, sizeof(activeEnemys)/sizeof(activeEnemys[0]));
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
//  SDL_RenderFillRect(renderMain, &rect);
  SDL_RenderPresent(renderMain);

  SDL_Delay(2000);
  printf("You");
  return 0;
}

//Renders both the foreground and background, if you want to do stuff with the background than
//make sure you do it before this
void renderBackground(SDL_Renderer *r, struct BackgroundArray *ba){
  ba->pos+=0.6;
  if(ba->pos > HEIGHT){
    ba->pos-=HEIGHT;
    if(ba->bGrounds[ba->bgIndex+2] != NULL){
        ba->bgIndex+=1;
      //  if(ba->fGrounds[ba->fgIndex+1] != NULL){
    //    ba->fgIndex = ba->bgIndex / 2;
      //  }
    }
  }
  //SDL_Rect rectForeground = {0,rectBackground.y*2 - HEIGHT,HEIGHT,HEIGHT*2};
  SDL_Rect rectBackground = {0,(int)floor(ba->pos),HEIGHT,HEIGHT};
  for(int i = 0; i < 2; i++){
    SDL_RenderCopy(r, ba->bGrounds[ba->bgIndex+i], NULL, &rectBackground);
    rectBackground.y-=HEIGHT;
  }
 /* for(int i = 0; i < 2; i++){
    SDL_RenderCopy(r, ba->fGrounds[ba->fgIndex+i], NULL, &rectForeground);
    rectForeground.y-=2*HEIGHT;
  }*/


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
    struct BulletData temp = { {player.rect.x + ( player.rect.w / 2 ) - 8, player.rect.y, 16, 16 }, 0, -20, 0, -20, 0, 0 ,0, 1};
    createBullet(sizeof(activePlayerBullets)/sizeof(activePlayerBullets[0]), activePlayerBullets, temp);
    temp.velX = 4;
    temp.velY = -15;
    createBullet(sizeof(activePlayerBullets)/sizeof(activePlayerBullets[0]), activePlayerBullets, temp);
    temp.velX = -4;
    createBullet(sizeof(activePlayerBullets)/sizeof(activePlayerBullets[0]), activePlayerBullets, temp);

    player.bulletCD = 6;
  }

}

void renderPlayer(SDL_Renderer *r, struct PlayerData p){
  //Checks to see if the player is in Invinciblity OR
  //If they are then check to see if its a draw frame (for blink effect, can set higher for longer time in between occurences)
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
      if(activeList[i].rect.x < 0 - activeList[i].rect.w){
        activeList[i].alive = 0;
      }else if(activeList[i].rect.x > HEIGHT){
        activeList[i].alive = 0;
      }
      if(activeList[i].rect.y < 0 - activeList[i].rect.h){
        activeList[i].alive = 0;
      }else if(activeList[i].rect.y > HEIGHT){
        activeList[i].alive = 0;
      }

      if (activeList[i].alive == 1){\
      //animate the bullet and draw it to the screen
      // NOTE THAT THE getAnimPos FUNCTION ALREADY INCREMENTS THE FRAME POSITION FOR YOU, THERE IS NO NEED TO DO IT YOURSELF
      SDL_Rect framePosRect = getAnimPos(&activeList[i].framePos, bulletSprites[activeList[i].textureID].width, bulletSprites[activeList[i].textureID].height);
      SDL_RenderCopy(r, bulletSprites[activeList[i].textureID].texture, &framePosRect, &activeList[i].rect);
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

void createEnemy(int id, int x, int y, int size){
  for(int i = 0; i < size; i++){
    if(activeEnemys[i].alive != 1){
      activeEnemys[i].id = id;
      activeEnemys[i].health = enemys[i].health;
      activeEnemys[i].currentBulletCD = 0;
      activeEnemys[i].maxBulletCD = enemys[i].maxBulletCD;
      activeEnemys[i].bulletVelX = enemys[i].bulletVelX;
      activeEnemys[i].bulletVelY = enemys[i].bulletVelY;
      activeEnemys[i].points = enemys[i].points;
      activeEnemys[i].xMov = enemys[i].xMov;
      activeEnemys[i].yMov = enemys[i].yMov;
      SDL_Rect temp = {x, y, enemySprites[id].width, enemySprites[id].height};
      activeEnemys[i].rect = temp;
      activeEnemys[i].alive = 1;
      return 0;
    }
  }
}

void updateEnemys(SDL_Renderer *r, int size){
  SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0, 0xFF);
  for(int i = 0; i < size; i++){
    if(activeEnemys[i].alive == 1){
      switch(activeEnemys[i].id){
      }
      activeEnemys[i].rect.x+=enemys[i].xMov;
      activeEnemys[i].rect.y+=enemys[i].yMov;

      SDL_RenderFillRect(r, &activeEnemys[i].rect);
    }

  }

}
/*temp = SDL_LoadBMP("bg1.bmp");
backgrounds.bGrounds[0] = SDL_CreateTextureFromSurface(renderMain, temp);
SDL_FreeSurface(temp);*/

void loadBackgrounds(SDL_Renderer *r, struct BackgroundArray *bgArray){
  SDL_Surface *temp;
  int running = 1, i = 0, charpos = 0;
  char name[16], prefix[][4] = {"bg","fg"};
  while(running){
    sprintf(name, "%s%d.bmp",prefix[charpos],i);
    if((temp = SDL_LoadBMP(name)) != NULL){
      if(charpos == 1){
        SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0xFF, 0, 0xFF) );
        bgArray->fGrounds[i] = SDL_CreateTextureFromSurface(r, temp);
      }else{
        bgArray->bGrounds[i] = SDL_CreateTextureFromSurface(r, temp);
      }
      SDL_FreeSurface(temp);
    }else {
      sprintf(name, "%s%d.bmp",prefix[0],i);
      if(SDL_LoadBMP(name) == NULL){
        sprintf(name, "%s%d.bmp",prefix[1],i);
        if(SDL_LoadBMP(name) == NULL){
          running = 0;
        }
      }
    }
    if(charpos == 0){
      charpos++;
    }else{
      charpos--;
      i++;
    }
  }
SDL_FreeSurface(temp);
}

//Use to load an image with a bg of (0xFF, 0, 0xFF) as the alpha channel, works for TextureData things like bullets, chracters, enemys, particles
void loadTransparent(SDL_Renderer *r, char path[], SDL_Texture **tex, int *width, int *height){
  SDL_Surface *temp;
  //Check to see if the path can be found, if it is than it will continue
  if((temp = SDL_LoadBMP(path)) != NULL){
    //Sets the color key of the loading, change the value in order to change what pixels are considred transparent
    SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0xFF, 0, 0xFF) );
    //Assigns all the data neccesarry to the TexutreData target
    *tex = SDL_CreateTextureFromSurface(r, temp);
    *width = temp->w / MAX_FRAMES;
    *height = temp->h;
  }
  SDL_FreeSurface(temp);
}

//Use whenver you need to render something that has an animation, it will automatically give you the SDL_Rect src
//It will even increment the framePostion for you so no need to worry about doing it manually
SDL_Rect getAnimPos(int *framePos, int w, int h){
  SDL_Rect temp = { (*framePos % MAX_FRAMES) * w, 0, w, h};
  *framePos+=1;
  return temp;
}
