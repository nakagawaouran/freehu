struct KeyDownState{
  int up, down, left, right, shoot;
}keyState;

struct PlayerData{
  int vel, lives, bombs, level, xp, iframe, bulletCD, bombCD;
  SDL_Texture *texture;
  SDL_Rect rect;
}player, playerClean = {6, 3, 3, 0, 0, 0, 0, 0, NULL, {0, 0, 64, 64} };
