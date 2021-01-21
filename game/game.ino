#include <Arduboy2.h>
#include "List.h"
#include "Enemy.h"
#include "Player.h"
#include "LevelMap.h"
#include "Menu.h"
#include "Bullet.h"

Arduboy2 arduboy;

#define SCREENWIDTH  128
#define SCREENHEIGHT  64

#define TILE_SIZE 16

#define circle_width 2 
#define X_MAX (WIDTH - circle_width - 1)
#define X_MIN circle_width
#define Y_MAX (HEIGHT - circle_width - 1)
#define STATUS_BAR 9
#define Y_MIN (circle_width + 1 + STATUS_BAR)

#define SPAWN_LIMIT 2

int mapx = 0;
int mapy = 0;

Player hero;

// //manu bypressing buttons
int gamestat = 0;

int frame = 0;

uint8_t queue = 0;

uint8_t waitCount = 0;

Rect playerRect { hero.x, hero.y, 16, 16 };

List<Enemy, SPAWN_LIMIT> enemies;
//List<Bullet, 8> bullets;

constexpr uint8_t bullets = 5;    // Maximum number of bullets
Bullet _bullet;

Rect bullet[bullets];

void _update()
{
  switch(gameState)
  {
    case GameMenu::TitleScreen:
      titlescreen();
      break;
    case GameMenu::Menu:
      updateMenu();
      break;
    case GameMenu::SinglePlayer:
      //updateSinglePlayer();
      break;
    case GameMenu::Options:
      //updateOptions();
      break;
  }
}

void draw()
{
  switch(gameState)
  {
    case GameMenu::Menu:
      drawMenu();
      break;
    case GameMenu::SinglePlayer:
      gameplay();
      break;
    case GameMenu::Options:
      //drawOptions();
      break;
  }
}

void titlescreen() {
  arduboy.setCursor(7, 30);
  arduboy.print(F("The hunt has begun!\n"));
  if (arduboy.justPressed(A_BUTTON)) {
    //gamestate = GAME_PLAY;
    gamestat = 1;
    changeGameState(GameMenu::Menu);
  }
}

void updateMenu()
{
  if (arduboy.justPressed(DOWN_BUTTON))
  {
    if(cursorIndex < lastMenuItem)
      ++cursorIndex;
    else
      cursorIndex = firstMenuItem;
  }

  if (arduboy.justPressed(UP_BUTTON))
  {
    if(cursorIndex > firstMenuItem)
      --cursorIndex;
    else
      cursorIndex = lastMenuItem;
  }

  if (arduboy.justPressed(B_BUTTON))
  {
    // Single player
    switch(cursorIndex)
    {
      case 0:
        changeGameState(GameMenu::SinglePlayer);
        break;
      case 1:
        changeGameState(GameMenu::Options);
        break;
    }
  }
}

void drawMenu()
{
  // Layout helper constants
  constexpr uint8_t selectorPositionX = 2;
  constexpr uint8_t menuPositionX = (selectorPositionX + 8);
  constexpr uint8_t menuPositionY = 4;
  constexpr uint8_t menuPadding = 8;

  // Draw menu
  arduboy.setCursor(menuPositionX, menuPositionY + (menuPadding * 0));
  arduboy.print(F("ONE PLAYER"));
  
  arduboy.setCursor(menuPositionX, menuPositionY + (menuPadding * 2));
  arduboy.print(F("OPTIONS"));

  // Draw cursor
  arduboy.setCursor(selectorPositionX, menuPositionY + (menuPadding * cursorIndex));
  arduboy.print('>');
}

void gameoverscreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Game Over Screen\n");
  if (arduboy.justPressed(A_BUTTON)) {
    //gamestate = GAME_HIGH;
  }
}

void highscorescreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("High Score Screen\n");
  if (arduboy.justPressed(A_BUTTON)) {
    //gamestate = GAME_TITLE;
  }
}

void draw_world() {
  const int tileswide = WIDTH / TILE_SIZE + 1;
  const int tilestall = HEIGHT / TILE_SIZE + 1;

  for (int y = 0; y < tilestall; y++) {
    for (int x = 0; x < tileswide; x++) {
      const int tilex = x - mapx / TILE_SIZE;
      const int tiley = y - mapy / TILE_SIZE;
      if (tilex >= 0 && tiley >= 0 && tilex < WORLD_WIDTH && tiley < WORLD_HEIGHT) {
        Sprites::drawOverwrite(x * TILE_SIZE + mapx % TILE_SIZE, y * TILE_SIZE + mapy % TILE_SIZE, tiles, world[tiley][tilex]);
      }
    }
  }

  arduboy.fillRect(0, 0, 48, 8, BLACK);
  arduboy.setCursor(0, 0);
  arduboy.print(mapx);
  arduboy.print(",");
  arduboy.print(mapy);
}

void player_control() {
  Sprites::drawOverwrite(playerRect.x, playerRect.y, heroDown, 0);
  if (arduboy.pressed(UP_BUTTON)) {
    if (mapy < 16) {
      mapy += 1;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroUp, frame);
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (16 + 32 < mapy + TILE_SIZE * WORLD_HEIGHT) {
      mapy -= 1;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroDown, frame);
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (mapx < 16) {
      mapx += 1;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroLeft, frame);
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (16 + 32 < mapx + TILE_SIZE * WORLD_WIDTH) {
      mapx -= 1;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroRight, frame);
  }
  if(arduboy.pressed(B_BUTTON)) {
    
    if(mapx < 16) {
      shoot(hero.x, hero.y);
      move_bullets(true);
    }
    else {
      shoot(hero.x, hero.y);
      move_bullets(false);
    }
  }
}

//shoot
void shoot(int x, int y) {
  if (waitCount == 0) {
    uint8_t bulletNum = find_unused_bullet();
    if (bulletNum != bullets) { // If we get an unused bullet
      // Set the start position. (A positive X indicates bullet in use)
      bullet[bulletNum].x = x;
      bullet[bulletNum].y = y + 3; // Part way down the player
      waitCount = 6; // Start the delay counter for the next bullet
    }
  }
  draw_bullets();

  // Decrement the bullet wait count if active
  if (waitCount != 0) {
    waitCount--;
  }
}

//drawing bullet
uint8_t find_unused_bullet() {
  uint8_t bulletNum;
  for (bulletNum = 0; bulletNum < bullets; bulletNum++) {
    if (bullet[bulletNum].x == _bullet.bulletOff) {
      break; // unused bullet found
    }
  }
  return bulletNum;
}

// Move all the bullets and disable any that go off screen
void move_bullets(bool shoot_left) {
  for (uint8_t bulletNum = 0; bulletNum < bullets; bulletNum++) {
    if (bullet[bulletNum].x != _bullet.bulletOff && shoot_left == false) { // If bullet in use
      bullet[bulletNum].x++; // move bullet right
    }
    if (bullet[bulletNum].x != _bullet.bulletOff && shoot_left == true) {
      bullet[bulletNum].x--;
    }
    if (bullet[bulletNum].x >= arduboy.width()) { // If off screen
      bullet[bulletNum].x = _bullet.bulletOff;  // Set bullet as unused
    }
  }
}

// Draw all the active bullets
void draw_bullets() {
  for (uint8_t bulletNum = 0; bulletNum < bullets; bulletNum++) {
    if (bullet[bulletNum].x != _bullet.bulletOff) { // If bullet in use
      //arduboy.drawCircle(bullet[bulletNum].x, bullet[bulletNum].y, 3, WHITE);
      arduboy.fillRect(bullet[bulletNum].x, bullet[bulletNum].y, _bullet.bulletSize, _bullet.bulletSize, BLACK);
    }
  }
}

//random spawning enemy
void spawn_enemy(int count) {
  for(int i = 0; i < count; i++) {
    Enemy spawn_enemy;
    
    int spawn_x = hero.x;
    int spawn_y = hero.y;

    while((spawn_x > X_MIN) && (spawn_x < X_MAX) && (spawn_y > Y_MIN) && (spawn_y < Y_MAX)) {
        spawn_x = random(X_MIN - 100, X_MAX + 100); // 50
        spawn_y = random(Y_MIN - 50, Y_MAX + 50); // 35
    }
    spawn_enemy.x = spawn_x;
    spawn_enemy.y = spawn_y;

    spawn_enemy.enemy_speed = 4; /*random(ENEMY_MAX_SPEED - 1, ENEMY_MIN_SPEED + 1);*/

    //add enemies into list
    enemies.add(spawn_enemy);
  }
}

void enemy_chase() {

  //if collide(hero, enemy) then restart the game
  
  for(unsigned int i = 0; i < enemies.get_size(); i++)
  {
    Enemy enemy;
    enemy.move_itter = enemies[i].move_itter + 1;
    enemy.x = enemies[i].x;
    enemy.y = enemies[i].y;
    enemy.enemy_speed = 4;
    
    Rect enemyRect  { enemy.x, enemy.y, 16, 18 };
    if(enemy.move_itter >= enemy.enemy_speed) {
      enemy.move_itter = 0;
      if(enemy.x < hero.x) {
        enemy.x++; 
      }
      if(enemy.x > hero.x) {
        enemy.x--;
      }
      if(enemy.y < hero.y) {
        enemy.y++;
      }
      if(enemy.y > hero.y) {
        enemy.y--;
      }
    }

    if(arduboy.collide(playerRect, enemyRect)) {
      restart_game();
    }
    
    enemies[i] = enemy;
    Sprites::drawOverwrite(enemies[i].x, enemies[i].y, enemy_down, frame);
  }
}

void generate_wave() {
  for(unsigned int i = 0; i < hero.wave; i++) {
    if(enemies.get_size() < SPAWN_LIMIT) {
      spawn_enemy(1);
    } else {
      queue++;
    }
  }
  hero.wave++;
}

//check limit of enemies (which spawned)
void check_enemy_queue() {
  if((enemies.get_size() < SPAWN_LIMIT) &&(queue > 0)) {
    for(int i = 0; i < queue; i++) {
      spawn_enemy(1);
      queue--;
      if(enemies.get_size() >= SPAWN_LIMIT) {
        break;
      }
    }
  }
}

void restart_game() {
  enemies.clear_list();

  hero.x = WIDTH / 2;
  hero.y = HEIGHT / 2;
  hero.wave = 1;
  
  generate_wave();
}

void count_timer(uint8_t start) {
  while(start > -1) {
    if(start != 0) {
      start--;
      delay(100);
    }
  }
}

void gameplay() {
  draw_world();
  player_control();

  if(enemies.get_size() <= 0) {
    generate_wave();
  }
  
  enemy_chase();
  check_enemy_queue();

  if (arduboy.justPressed(A_BUTTON)) {
    //gamestate = GAME_OVER;
  }
}

void setup() {

  arduboy.begin();
  arduboy.setFrameRate(60);

  for (uint8_t bulletNum = 0; bulletNum < bullets; ++bulletNum) {
    bullet[bulletNum].x = _bullet.bulletOff;
    bullet[bulletNum].width = _bullet.bulletSize;
    bullet[bulletNum].height = _bullet.bulletSize;
  }
  
  arduboy.display();
  generate_wave();
  arduboy.initRandomSeed();
  arduboy.clear();
  
}

void loop() {

  if(!(arduboy.nextFrame())) {
    return;
  }

  arduboy.pollButtons();
  arduboy.clear();
  if (arduboy.everyXFrames(20)) frame++;
  if (frame > 1) frame = 0;
  _update();
  draw();
  arduboy.display();
  
}
