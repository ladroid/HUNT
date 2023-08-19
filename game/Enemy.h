enum class EnemyType {
  Fast,
  Strong
};

class Enemy
{
public:
  int16_t x = 0;
  int16_t y = 0;
  uint8_t move_itter = 0;
  uint8_t enemy_speed = 0;
  uint8_t health = 0;
  EnemyType type;
};

//draw enemy
unsigned char const enemy_down[] PROGMEM =
{
  16, 18,
  0x00, 0x00, 0x00, 0xb0, 0xb8, 0xb8, 0x78, 0xf8, 0xf8, 0xf8, 0x78, 0xb8, 0xb0, 0xb0, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x1d, 0x7d, 0x7d, 0x7e, 0x3f, 0x3f, 0x3f, 0x7e, 0x7d, 0x7d, 0x3d, 0x02, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  
  0x00, 0x00, 0x00, 0xb4, 0xb8, 0xb8, 0x78, 0xf8, 0xf8, 0xf8, 0x78, 0xb8, 0xb8, 0xb4, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x1d, 0x3d, 0x3d, 0x3e, 0x1f, 0x1f, 0x1f, 0x3e, 0x3d, 0x3d, 0x1d, 0x02, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

unsigned char const enemy_right[] PROGMEM = 
{
  16, 18,
  0x00, 0x00, 0xcc, 0xbc, 0xc0, 0xc0, 0xbc, 0xc0, 0xfc, 0xfc, 0xfc, 0x8c, 0x70, 0x70, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0x0f, 0x3f, 0x3f, 0x0f, 0x0f, 0x3f, 0x3f, 0x3f, 0x3f, 0x0f, 0x0f, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  
  0x00, 0x18, 0x98, 0x78, 0x78, 0x80, 0x78, 0x78, 0x98, 0xe0, 0xf8, 0xf8, 0x18, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x1f, 0x1f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1e, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

unsigned char const enemy_left[] PROGMEM =
{
  16, 18,
  0x00, 0x00, 0x80, 0x60, 0x9c, 0xfc, 0xfc, 0xfc, 0xfc, 0xc0, 0xbc, 0xc0, 0xbc, 0xbc, 0xdc, 0x00, 
  0x00, 0x00, 0x03, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x1f, 0x1f, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  
  0x00, 0x00, 0x80, 0x60, 0x9c, 0xfc, 0xfc, 0xe0, 0xdc, 0xdc, 0xbc, 0xc0, 0xbc, 0xbc, 0xdc, 0x1c, 
  0x00, 0x00, 0x03, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x1f, 0x1f, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

unsigned char const enemy_up[] PROGMEM = 
{
  16, 18,
  0x00, 0x00, 0xc0, 0xf8, 0xc0, 0xc0, 0xbc, 0xc0, 0xc0, 0xc0, 0xbc, 0xc0, 0xf8, 0xf8, 0xc0, 0x00, 
  0x00, 0x00, 0x03, 0x1f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x1f, 0x1f, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  
  0x00, 0x00, 0xc0, 0xdc, 0xdc, 0xdc, 0xbc, 0xc0, 0xc0, 0xc0, 0xbc, 0xdc, 0xdc, 0xdc, 0xc0, 0x00, 
  0x00, 0x00, 0x03, 0x1f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x1f, 0x1f, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

unsigned char const enemy_second[] PROGMEM = 
{
20, 26,
0x00, 0x00, 0x40, 0x80, 0xa0, 0xa8, 0x3c, 0xac, 0xb4, 0x34, 0xbc, 0x28, 0xb8, 0x6c, 0xd8, 0xb8, 0xe0, 0xd0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x02, 0xd7, 0xab, 0xd6, 0xab, 0xd1, 0xab, 0xd7, 0xab, 0xd6, 0x01, 0xff, 0xc1, 0xde, 0x0e, 0x00, 
0x00, 0x00, 0x20, 0x4e, 0xd4, 0xde, 0x45, 0x5e, 0x5e, 0x5e, 0x49, 0xde, 0xae, 0xa1, 0xdc, 0x67, 0x1f, 0x1f, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};