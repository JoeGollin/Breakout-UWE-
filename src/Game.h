
#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include "GameObject.h"
#include "Vector2.h"
#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void spawnSprites();
  void textSetup();
  void collisionAction();
  //void mouseClicked(sf::Event event);

 private:
  sf::RenderWindow& window;
  sf::Clock clock;
  sf::Sprite title;
  sf::Texture title_texture;
  sf::Sprite background;
  sf::Texture background_texture;
  sf::Font font;
  sf::Text enter_text;
  sf::Text quit_text;
  sf::Text controls_text;
  sf::Text lives_text;
  sf::Text score_text;
  sf::Text winner_text;
  sf::Text loser_text;
  sf::Text to_menu_text;
  sf::Text extra_controls_text;
  sf::Texture ball_texture;
  sf::Texture paddle_texture;
  sf::Texture block_texture_blue;
  sf::Texture block_texture_green;
  sf::Texture block_texture_red;
  sf::Texture gem_texture;
  GameObject* paddle = nullptr;
  GameObject* ball = nullptr;
  GameObject* blocks = nullptr;
  GameObject* gems = nullptr;

  int grid_size = 13;
  int player_lives = 5;
  int player_score = 0;
  int blocks_score = 5;
  int gem_score = 10;
  int gems_amount = 4;
  int max_score = blocks_score * (grid_size*grid_size) + (gem_score*gems_amount);
  float ball_speed = 20;
  float player_speed = 500;
  float gem_speed = 10;
  bool in_menu = true;
  bool game_over = false;
  bool out_of_lives = false;
  static bool collisionCheck(sf::Sprite& sprite1, sf::Sprite& sprite2);
};

#endif // BREAKOUT_GAME_H
