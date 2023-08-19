#include "List.h"
#include "Enemy.h"
#include "Player.h"
#include "LevelMap.h"
#include "Menu.h"
#include "Bullet.h"
#include "Sound.h"

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

Player hero;

int frame = 0;

uint8_t queue = 0;

uint8_t waitCount = 0;

uint16_t hitCount = 0;

List<Enemy, SPAWN_LIMIT> enemies;

constexpr uint8_t bullets = 1;    // Maximum number of bullets
constexpr int8_t number_enemies = 1;
Bullet _bullet;

Enemy enem;
