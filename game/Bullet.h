class Bullet {
public:
  uint8_t bulletSize = 3; // Size of a square bullet
  uint8_t bulletWait = 1; // Minimum frames between firing
  uint8_t bulletOff = -1;     // "Bullet not in use" value;
};
