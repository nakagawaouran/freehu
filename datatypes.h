//Manage the state of whether the keys are being held or not, a value of 1 for true, 0 false
struct KeyDownState{
  int up, down, left, right, shoot, focus;
}keyState;

//All the variables the player needs, playerClean are the defaults given at the start of a new game
struct PlayerData{
  int vel, lives, bombs, level, xp, iframe, bulletCD, bombCD;
  SDL_Texture *texture;
  SDL_Rect rect;
}player, playerClean = {4, 3, 3, 0, 0, 0, 0, 0, NULL, {0, 0, 48, 48} };

struct EnemyData{
  int health, movePattern, maxBulletCD, xMov, yMov, bulletVelX, bulletVelY, points;
}enemys[99] = {20, NULL, 10, 0, 1, 0, 10, 100};

struct TextureData{
  SDL_Texture *texture;
  int width, height;
}bulletSprites[99], enemySprites[99];

struct AliveEnemys{
  int id, xMov, yMov, health, currentBulletCD, maxBulletCD, bulletVelX, bulletVelY, points, alive;
  SDL_Rect rect;
}activeEnemys[99];

struct BulletData{
  SDL_Rect rect;
  int velX, velY, framePos, damage, textureID, timer, alive, id;
}activePlayerBullets[99], activeEnemyBullets[300];

struct BackgroundArray{
  int bgIndex, fgIndex;
  float pos;
  SDL_Texture *fGrounds[9];
  SDL_Texture *bGrounds[9];

}backgrounds = {0,0,0.0};
