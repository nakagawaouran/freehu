#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "datatypes.h"

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

  //Main Game Loop
  int running = 1;
  SDL_Event event;
  while(running){

    printf("UP:%d DOWN:%d LEFT:%d RIGHT:%d SHOOT:%d\n",keyState.up,keyState.down,keyState.left,keyState.right,keyState.shoot);

    //Get User Input
    SDL_PollEvent(&event);
    switch (event.type) {

      //Quit the game through the window
      case SDL_QUIT:
        SDL_Quit();
        exit(1);
        break;

      //Pressing down a keyboard key
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

      break;

    }

  }



  SDL_SetRenderDrawColor(renderMain, 23, 23 ,23, 0xFF);
  SDL_RenderClear(renderMain);
  SDL_RenderPresent(renderMain);
  SDL_SetRenderDrawColor(renderMain, 222, 222, 222 ,0xFF);
  SDL_Rect rect = {0,0,400,400};
  SDL_RenderFillRect(renderMain, &rect);
  SDL_RenderPresent(renderMain);

  SDL_Delay(2000);
  printf("You");
  return 0;
}
