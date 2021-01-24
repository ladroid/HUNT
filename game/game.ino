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

int frame = 0;

uint8_t queue = 0;

uint8_t waitCount = 0;

uint16_t hitCount = 0;

Rect playerRect { hero.x, hero.y, 16, 16 };

List<Enemy, SPAWN_LIMIT> enemies;

constexpr uint8_t bullets = 5;    // Maximum number of bullets
Bullet _bullet;

Enemy enem;
Rect bullet[bullets];

void menuUpdate()
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
    case GameMenu::HighScoreScreen:
      highscorescreen();
      break;
  }
}

void menuDraw()
{
  switch(gameState)
  {
    case GameMenu::TitleScreen:
      titlescreen();
      break;
    case GameMenu::Menu:
      drawMenu();
      break;
    case GameMenu::SinglePlayer:
      gameplay();
      break;
    case GameMenu::Options:
      //drawOptions();
      break;
    case GameMenu::HighScoreScreen:
      highscorescreen();
      break;
  }
}

void titlescreen() {
  arduboy.setCursor(40, 30);
  arduboy.setTextSize(2);
  arduboy.print(F("HUNT!\n"));
  if (arduboy.justPressed(A_BUTTON)) {
    arduboy.setTextSize(1);
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
  arduboy.print(F("PLAY"));
  
  arduboy.setCursor(menuPositionX, menuPositionY + (menuPadding * 1));
  arduboy.print(F("OPTIONS"));

  // Draw cursor
  arduboy.setCursor(selectorPositionX, menuPositionY + (menuPadding * cursorIndex));
  arduboy.print('>');
}

void gameoverscreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Game Over Screen\n");
  if (arduboy.justPressed(A_BUTTON)) {
  }
}

void highscorescreen() {
  arduboy.setCursor(20, 30);
  arduboy.setTextSize(2);
  arduboy.print("Score " + (String)hitCount);
  if (arduboy.pressed(A_BUTTON)) {
    arduboy.clear();
    hitCount = 0;
    changeGameState(GameMenu::TitleScreen);
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
      check_collision_enemy();
    }
    else {
      shoot(hero.x, hero.y);
      move_bullets(false);
      check_collision_enemy();
    }
  }
  if(arduboy.pressed(A_BUTTON)) {
    changeGameState(GameMenu::HighScoreScreen);
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
      bullet[bulletNum].x--; // move bullet left
    }
    if (bullet[bulletNum].x >= arduboy.width()) { // If off screen
      bullet[bulletNum].x = _bullet.bulletOff;  // Set bullet as unused
    }
  }
}

//check collision
void check_collision_enemy() {
  for(uint8_t i = 0; i < enemies.get_size(); i++) {
    Rect enemyRect { enemies[i].x, enemies[i].y, 16, 18 };
    for(uint8_t bulletNum = 0; bulletNum < bullets; bulletNum++) {
      if(arduboy.collide(bullet[bulletNum], enemyRect)) {
        hitCount++;
        bullet[bulletNum].x = _bullet.bulletOff;
        //remove enemy
        enemies.removeAt(i);
      }
    }
  }
}

//display hits
void display_hits() {
  arduboy.setCursor(80, 0);
  arduboy.print(F("HITS: "));
  arduboy.print(hitCount);
}

// Draw all the active bullets
void draw_bullets() {
  for (uint8_t bulletNum = 0; bulletNum < bullets; bulletNum++) {
    if (bullet[bulletNum].x != _bullet.bulletOff) { // If bullet in use
      arduboy.fillRect(bullet[bulletNum].x, bullet[bulletNum].y, _bullet.bulletSize, _bullet.bulletSize, BLACK);
    }
  }
}

//random spawning enemy
void spawn_enemy(int count) {
  for(int i = 0; i < count; i++) {
    
    int spawn_x = hero.x;
    int spawn_y = hero.y;

    while((spawn_x > X_MIN) && (spawn_x < X_MAX) && (spawn_y > Y_MIN) && (spawn_y < Y_MAX)) {
        spawn_x = random(X_MIN - 100, X_MAX + 100); // 50
        spawn_y = random(Y_MIN - 50, Y_MAX + 50); // 35
    } 
    enem.x = spawn_x;
    enem.y = spawn_y;

    enem.enemy_speed = 4;
    //add enemies into list
    enemies.add(enem);
  }
}

void enemy_chase() {
  
  for(unsigned int i = 0; i < enemies.get_size(); i++)
  {
    //Enemy enemy;
    enem.move_itter = enemies[i].move_itter + 1;
    enem.x = enemies[i].x;
    enem.y = enemies[i].y;
    enem.enemy_speed = 4;
    
    Rect enemyRect { enemies[i].x, enemies[i].y, 16, 18 };
    if(enem.move_itter >= enem.enemy_speed) {
      enem.move_itter = 0;
      if(enem.x < hero.x) {
        enem.x++; 
      }
      if(enem.x > hero.x) {
        enem.x--;
      }
      if(enem.y < hero.y) {
        enem.y++;
      }
      if(enem.y > hero.y) {
        enem.y--;
      }
    }

    if(arduboy.collide(playerRect, enemyRect)) {
      changeGameState(GameMenu::HighScoreScreen);
    }
    
    enemies[i] = enem;
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

void gameplay() {
  draw_world();
  player_control();

  if(enemies.get_size() <= 0) {
    generate_wave();
  }
  
  enemy_chase();
  check_enemy_queue();

  display_hits();
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
  menuUpdate();
  arduboy.clear();
  if (arduboy.everyXFrames(20)) frame++;
  if (frame > 1) frame = 0;
  menuDraw();
  arduboy.display();
  
}
