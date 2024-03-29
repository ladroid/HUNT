#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
#include <Arduboy2.h>
#include "Global.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

Rect playerRect { hero.x, hero.y, 16, 16 };

BulletInfo bulletInfos[bullets];

inline void audio_on() {
  isSound = true;
  sound.tones(mainm);
}

inline void audio_off() {
  isSound= false;
  sound.noTone();
}

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
      updateOptions();
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
      drawOptions();
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
        hero.x = WIDTH / 2;
        hero.y = HEIGHT / 2;
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

void updateOptions()
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
    // Options to turn on / off audio
    switch(cursorIndex) {
      case 0:
        audio_on();
        break;
      case 1:
        audio_off();
        break;
    }
  }
  //go back to menu
  if(arduboy.justPressed(A_BUTTON)) {
    arduboy.clear();
    changeGameState(GameMenu::TitleScreen);
  }
}

void drawOptions() {
  // Layout helper constants
  constexpr uint8_t selectorPositionX = 2;
  constexpr uint8_t menuPositionX = (selectorPositionX + 8);
  constexpr uint8_t menuPositionY = 4;
  constexpr uint8_t menuPadding = 8;

  // Draw menu
  arduboy.setCursor(menuPositionX, menuPositionY + (menuPadding * 0));
  arduboy.print(F("Sound On"));
  
  arduboy.setCursor(menuPositionX, menuPositionY + (menuPadding * 1));
  arduboy.print(F("Sound Off"));

  // Draw cursor
  arduboy.setCursor(selectorPositionX, menuPositionY + (menuPadding * cursorIndex));
  arduboy.print('>');
}

void highscorescreen() {
  arduboy.setCursor(20, 30);
  arduboy.setTextSize(2);
  arduboy.print("Score " + (String)hitCount);
  if (arduboy.pressed(A_BUTTON)) {
    arduboy.clear();
    changeGameState(GameMenu::TitleScreen);
    restart_game();
  }
}

void draw_world() {
  const int tileswide = WIDTH / TILE_SIZE + 1;
  const int tilestall = HEIGHT / TILE_SIZE + 1;

  for (size_t y = 0; y < tilestall; ++y) {
    for (size_t x = 0; x < tileswide; ++x) {
      const int tilex = x - mapx / TILE_SIZE;
      const int tiley = y - mapy / TILE_SIZE;
      if (tilex >= 0 && tiley >= 0 && tilex < WORLD_WIDTH && tiley < WORLD_HEIGHT) {
        Sprites::drawOverwrite(x * TILE_SIZE + mapx % TILE_SIZE, y * TILE_SIZE + mapy % TILE_SIZE, tiles, world[tiley][tilex]);
      }
    }
  }

  arduboy.fillRect(0, 0, 48, 8, BLACK);
  arduboy.setCursor(0, 0);
  arduboy.print(hero.health);
  arduboy.print("/");
  arduboy.print("100");
}

void player_control() {
  Sprites::drawOverwrite(playerRect.x, playerRect.y, heroDown, 0);
  if (arduboy.pressed(UP_BUTTON)) {
    if (mapy < (HEIGHT/2)) {
      ++mapy;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroUp, frame);
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (16 + 32 < mapy + TILE_SIZE * WORLD_HEIGHT) {
      --mapy;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroDown, frame);
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (mapx < (WIDTH / 2)) {
      ++mapx;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroLeft, frame);
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if ((WIDTH/2) + 26 < mapx + TILE_SIZE * WORLD_WIDTH) {
      --mapx;
    }
    Sprites::drawOverwrite(playerRect.x, playerRect.y, heroRight, frame);
  }
  if(arduboy.pressed(B_BUTTON)) {
    if(isSound == true) {
      sound.tone(NOTE_C4, 100);
    }
    shoot(hero.x, hero.y);
  }
  // Always move bullets, regardless of shooting
  move_bullets();

  if(arduboy.pressed(A_BUTTON)) {
    changeGameState(GameMenu::HighScoreScreen);
  }
}

void update_bullets() {
  move_bullets();
  draw_bullets();
  check_collision_enemy();
}

//shoot
void shoot(int& x, int& y) {
  BulletDirection direction;
  if (arduboy.pressed(LEFT_BUTTON)) {
    direction = LEFT;
  } else if (arduboy.pressed(RIGHT_BUTTON)) {
    direction = RIGHT;
  } else if (arduboy.pressed(UP_BUTTON)) {
    direction = UP;
  } else if (arduboy.pressed(DOWN_BUTTON)) {
    direction = DOWN;
  }

  // Only proceed if there isn't an active bullet
  if (bulletInfos[0].rect.x == _bullet.bulletOff) {  // Check if the bullet is off-screen
    bulletInfos[0].rect.x = x;
    bulletInfos[0].rect.y = y + 3;
    bulletInfos[0].direction = direction;
    draw_bullets();
  }
}

//return the index of the first unused bullet or return the value of bullets if all are in use
uint8_t find_unused_bullet() {
  return 0;
}

// Move all the bullets and disable any that go off screen
void move_bullets() {
  for (uint8_t bulletNum = 0; bulletNum < bullets; ++bulletNum) {
    if (bulletInfos[bulletNum].rect.x != _bullet.bulletOff) {
      switch (bulletInfos[bulletNum].direction) {
        case LEFT:
          bulletInfos[bulletNum].rect.x -= _bullet.bulletSpeed;
          break;
        case RIGHT:
          bulletInfos[bulletNum].rect.x += _bullet.bulletSpeed;
          break;
        case UP:
          bulletInfos[bulletNum].rect.y -= _bullet.bulletSpeed;
          break;
        case DOWN:
          bulletInfos[bulletNum].rect.y += _bullet.bulletSpeed;
          break;
      }
    }
    
    if (bulletInfos[bulletNum].rect.x >= arduboy.width() || bulletInfos[bulletNum].rect.x < 0 ||
        bulletInfos[bulletNum].rect.y >= arduboy.height() || bulletInfos[bulletNum].rect.y < 0) {
      bulletInfos[bulletNum].rect.x = _bullet.bulletOff;
    }
  }
}

//check collision
void check_collision_enemy() {
  for(uint8_t i = 0; i < enemies.get_size(); ++i) {
    Rect enemyRect { enemies[i].x, enemies[i].y, 16, 18 };
    for(uint8_t bulletNum = 0; bulletNum < bullets; ++bulletNum) {
      if(arduboy.collide(bulletInfos[bulletNum].rect, enemyRect)) {
        ++hitCount;
        bulletInfos[bulletNum].rect.x = _bullet.bulletOff;
        //remove enemy
        if(isSound == true) {
          sound.tone(NOTE_E5, 100);
        }
        enemies.removeAt(i);
      }
    }
  }
}

//display hits
void display_hits() {
  arduboy.setCursor(65, 0);
  arduboy.print(F("KILLS: "));
  arduboy.print(hitCount);
}

// Draw all the active bullets
void draw_bullets() {
  if (bulletInfos[0].rect.x != _bullet.bulletOff) { // If bullet in use
    arduboy.fillRect(bulletInfos[0].rect.x, bulletInfos[0].rect.y, _bullet.bulletSize, _bullet.bulletSize, BLACK);
  }
}

//random spawning enemy
void spawn_enemy(const int8_t& count) {
  for(int i = 0; i < count; ++i) {
    
    int spawn_x = 20;
    int spawn_y = 20;

    while((spawn_x > X_MIN) && (spawn_x < X_MAX)) {
        spawn_x = random(X_MIN - 100, X_MAX + 100); // 50
        spawn_y = random(Y_MIN - 50, Y_MAX + 50); // 35
    } 
    enem.x = spawn_x;
    enem.y = spawn_y;

    if(random(0, 2) == 0) { // 50% chance for either type
        // Fast enemy
        enem.type = EnemyType::Fast;
        enem.health = 2; // Example health value, you can adjust
        enem.enemy_speed = 8; // High speed
    } else {
        // Strong enemy
        enem.type = EnemyType::Strong;
        enem.health = 100; // Example health value, you can adjust
        enem.enemy_speed = 2; // Low speed
    }

    // enem.enemy_speed = 4;
    //add enemies into list
    enemies.add(enem);
  }
}

void enemy_chase() {
  
  for(unsigned int i = 0; i < enemies.get_size(); ++i)
  {
    //Enemy enemy;
    enem.move_itter = enemies[i].move_itter + 1;
    enem.x = enemies[i].x;
    enem.y = enemies[i].y;
    // enem.enemy_speed = 4;
    
    Rect enemyRect { enemies[i].x, enemies[i].y, 16, 18 };
    if(enem.move_itter >= enem.enemy_speed) {
      enem.move_itter = 0;
      if(enem.x < hero.x) {
        ++enem.x; 
      }
      if(enem.x > hero.x) {
        --enem.x;
      }
      if(enem.y < hero.y) {
        ++enem.y;
      }
      if(enem.y > hero.y) {
        --enem.y;
      }
    }

    if(arduboy.collide(playerRect, enemyRect)) {
      --hero.health;
      if(hero.health <= 0) {
        if(isSound == true) {
          sound.tones(playerDead);
        }
        changeGameState(GameMenu::HighScoreScreen);
      }
      
    }
    
    enemies[i] = enem;
    // Sprites::drawOverwrite(enemies[i].x, enemies[i].y, enemy_down, frame);
    switch (enemies[i].type) {
      case EnemyType::Fast:
        Sprites::drawOverwrite(enemies[i].x, enemies[i].y, enemy_down, frame);
        break;
      case EnemyType::Strong:
        Sprites::drawOverwrite(enemies[i].x, enemies[i].y, enemy_second, 0);
        break;
    }
  }
}

void generate_wave() {
  for(unsigned int i = 0; i < hero.wave; ++i) {
    if(enemies.get_size() < SPAWN_LIMIT) {
      spawn_enemy(number_enemies);
    } else {
      ++queue;
    }
  }
  ++hero.wave;
}

//check limit of enemies (which spawned)
void check_enemy_queue() {
  if((enemies.get_size() < SPAWN_LIMIT) &&(queue > 0)) {
    for(int i = 0; i < queue; ++i) {
      spawn_enemy(number_enemies);
      --queue;
      if(enemies.get_size() >= SPAWN_LIMIT) {
        break;
      }
    }
  }
}

void restart_game() {
  enemies.clear_list();
  hitCount = 0;
  hero.health = 100;
  hero.x = WIDTH / 2;
  hero.y = HEIGHT / 2;
  hero.wave = 1;
  generate_wave();
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
  sound.tones(mainm);
  for (uint8_t bulletNum = 0; bulletNum < bullets; ++bulletNum) {
    bulletInfos[bulletNum].rect.x = _bullet.bulletOff;
    bulletInfos[bulletNum].rect.width = _bullet.bulletSize;
    bulletInfos[bulletNum].rect.height = _bullet.bulletSize;
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
  if (arduboy.everyXFrames(20)) ++frame;
  if (frame > 1) frame = 0;
  menuDraw();
  update_bullets();
  arduboy.display();
  
}
