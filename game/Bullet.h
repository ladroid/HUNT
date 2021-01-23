class Bullet {
public:
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  uint8_t bulletSize = 3; // Size of a square bullet
  uint8_t bulletWait = 6; // Minimum frames between firing
  int bulletOff = -1;     // "Bullet not in use" value;
};
