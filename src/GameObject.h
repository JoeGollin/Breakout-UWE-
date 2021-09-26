
#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H
#include <SFML/Graphics.hpp>
#include "Vector2.h"


class GameObject
{
 public:
  GameObject();
  ~GameObject();
  bool initaliseSprite(sf::Texture &texture, std::string filename);
  sf::Sprite* getSprite();
  Vector2* blocks_position = nullptr;
  Vector2* ball_position = nullptr;
  Vector2* ball_direction = nullptr;
  Vector2* paddle_position = nullptr;
  Vector2* paddle_direction = nullptr;
  Vector2* gem_direction = nullptr;
  Vector2* gem_position = nullptr;

  bool getVisibility() const;
  void setVisibility(bool value);

 private:
  sf::Sprite* sprite = nullptr;
  bool is_visible = true;

};


#endif // BREAKOUT_GAMEOBJECT_H
