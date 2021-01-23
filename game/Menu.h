enum class GameMenu: uint8_t 
{
  TitleScreen,
  Menu,
  SinglePlayer,
  Options,
  HighScoreScreen
};

GameMenu gameState = GameMenu::TitleScreen;
void changeGameState(GameMenu newGameState)
{
  gameState = newGameState;
}

// The index of the currently selected menu item
uint8_t cursorIndex = 0;

// How many items the menu has
constexpr uint8_t menuItems = 2;
constexpr uint8_t firstMenuItem = 0;
constexpr uint8_t lastMenuItem = (menuItems - 1);
