#include <iostream>
#include "GameObject.h"

bool GameObject::initaliseSprite(sf::Texture &texture, std::string filename)
{

  if(!texture.loadFromFile(filename))
  {
    std::cout << filename << " failed to load \n";
  }
  sprite = new sf::Sprite(texture);
  return true;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}

GameObject::GameObject()
{
  blocks_position = new Vector2(150,50);
  ball_position = new Vector2(540,620);
  ball_direction = new Vector2(10,20);
  paddle_position = new Vector2(500,680);
  paddle_direction = new Vector2(0,0);
  gem_position = new Vector2(150, -1000);
  gem_direction = new Vector2(0,10);
}

GameObject::~GameObject()
{

}

bool GameObject::getVisibility() const
{
  return is_visible;
}

void GameObject::setVisibility(bool value)
{
  is_visible = value;
}
