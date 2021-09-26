
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(nullptr));
}

Game::~Game()
{
  delete[] blocks;
  blocks = nullptr;
  delete[] gems;
  gems = nullptr;
}

bool Game::init()
{
  in_menu = true;
  textSetup();

  if (!background_texture.loadFromFile("Data/Images/space.png"))
  {
    std::cout << "background texture did not load \n";
  }
  background.setTexture(background_texture);

  if (!title_texture.loadFromFile("Data/Images/title_logo.png"))
  {
    std::cout << "title file did not load \n";
  }
  title.setTexture(title_texture);
  title.setScale(0.5,0.5);
  title.setPosition(
    window.getSize().x / 2.0f - title.getGlobalBounds().width / 2,
    window.getSize().y / 7.0f - title.getGlobalBounds().height / 2);

  paddle = new GameObject();
  paddle->initaliseSprite(paddle_texture,"Data/Images/paddleBlue.png");

  ball = new GameObject();
  ball->initaliseSprite(ball_texture,"Data/Images/ballGrey.png");

  blocks = new GameObject[grid_size*grid_size];
  for (int i = 0; i < grid_size; i++)
  {
    for (int j = 0; j < grid_size; j++) // Y Axis
    {
      if((i*grid_size*j)% 3 == 0)
      {
        blocks[i*grid_size+j].initaliseSprite(
          block_texture_blue,"Data/Images/element_blue_rectangle_glossy.png");
      }
      if((i*grid_size*j)% 3 == 1)
      {
        blocks[i*grid_size+j].initaliseSprite(
          block_texture_green,"Data/Images/element_green_rectangle_glossy.png");
      }
      if((i*grid_size*j)% 3 == 2)
      {
        blocks[i*grid_size+j].initaliseSprite(
          block_texture_red,"Data/Images/element_red_rectangle_glossy.png");
      }
      blocks[i*grid_size+j].getSprite()->setScale(0.75,0.75);
      blocks[i*grid_size+j].getSprite()->setPosition(
        blocks->blocks_position->x + 65*i,
        blocks->blocks_position->y + 30*j);
    }
  }
  gems = new GameObject[gems_amount];
  for (int i = 0; i < gems_amount; i++)
  {
    gems[i].initaliseSprite(gem_texture, "Data/Images/element_grey_diamond_glossy.png");
    gems[i].getSprite()->setScale(0.5,0.5);
    gems[i].getSprite()->setPosition(gems->gem_position->x + 250*i, gems->gem_position->y - 800*i);
  }
  return true;
}

void Game::update(float dt)
{
  // Move Sprites
  paddle->getSprite()->move(paddle->paddle_direction->x * player_speed * dt,
                            paddle->paddle_direction->y * player_speed * dt);
  ball->getSprite()->move(ball->ball_direction->x * ball_speed * dt,
                          ball->ball_direction->y * ball_speed * dt);

  for (int i = 0; i <= gems_amount; i++) //
  {
    if(gems[i].getVisibility())
    {
      gems[i].getSprite()->move(gems->gem_direction->x * gem_speed * dt,
                              gems->gem_direction->y * gem_speed * dt);
    }
  }

  collisionAction();

  // Lives/Scoring
  if(player_lives == 0)
  {
    game_over = true;
    out_of_lives = true;
  }
  if(player_score == max_score || player_score >= grid_size*grid_size * 5)
  {
    game_over = true;
  }
}

void Game::render()
{

  if (in_menu)
  {
    window.draw(background);
    window.draw(enter_text);
    window.draw(quit_text);
    window.draw(title);
    window.draw(controls_text);
    window.draw(extra_controls_text);
  }
  else if(game_over)
  {
    window.draw(background);
    if (out_of_lives)
    {
      window.draw(loser_text);
      window.draw(to_menu_text);
    }
    else
    {
      window.draw(winner_text);
      window.draw(to_menu_text);
    }
  }
  else
  {
    window.draw(background);
    window.draw(lives_text);
    window.draw(score_text);
    window.draw(*(paddle->getSprite()));
    window.draw(*(ball->getSprite()));
    for (int i = 0; i < grid_size*grid_size; ++i) // Draw Blocks
    {
      if (blocks[i].getVisibility())
      {
        window.draw(*blocks[i].getSprite());
      }
    }
    for (int j = 0; j < gems_amount; j++)
    {
      if (gems[j].getVisibility())
      {
        window.draw(*gems[j].getSprite());
      }
    }
  }
}

void Game::keyPressed(sf::Event event)
{
  // Menu inputs
  if (in_menu and event.key.code == sf::Keyboard::Enter)
  {
    player_lives = 5;
    player_score = 0;
    in_menu = false;
    clock.restart();
    spawnSprites();
  }
  else if (in_menu and event.key.code == sf::Keyboard::Escape)
  {
    window.close();
  }

  // Game inputs
  if (game_over)
  {
    if (event.key.code == sf::Keyboard::Enter)
    {
      game_over = false;
      in_menu = true;
    }
    else
    {
      //game_over = true;
    }
  }
  if (!in_menu)
  {
    // Go back to menu
    if (event.key.code == sf::Keyboard::Escape)
    {
      in_menu = true;
    }
    // Move paddle left
    if (event.key.code == sf::Keyboard::A)
    {
      paddle->paddle_direction->x = -1;
    }
    // Move paddle right
    if (event.key.code == sf::Keyboard::D)
    {
      paddle->paddle_direction->x = 1;
    }
    if (event.key.code == sf::Keyboard::Z)
    {
      ball_speed -= 5;
    }
    if (event.key.code == sf::Keyboard::X)
    {
      ball_speed += 5;
    }
  }
}

void Game::keyReleased(sf::Event event)
{
  paddle->paddle_direction->x = 0;
  paddle->paddle_direction->y = 0;
}

bool Game::collisionCheck(sf::Sprite& sprite1, sf::Sprite& sprite2)
{
  if (sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds()))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Game::collisionAction()
{
  // Border Collision
  if (ball->getSprite()->getPosition().x + ball->getSprite()->getGlobalBounds().width
      >= window.getSize().x) // Collide with right of screen
  {
    ball->ball_direction->x = -ball->ball_direction->x;
  }
  if (ball->getSprite()->getPosition().x <= 5 ) // Collide with left of screen
  {
    ball->ball_direction->x = -ball->ball_direction->x;
  }
  if (ball->getSprite()->getPosition().y <= 5)// Collide with top of screen
  {
    ball->ball_direction->y = -ball->ball_direction->y;
  }
  if (ball->getSprite()->getPosition().y - ball->getSprite()->getGlobalBounds().width
      >= window.getSize().y) // Collide with the bottom of the screen
  {
    player_lives -= 1;
    lives_text.setString(std::to_string(player_lives));
    spawnSprites();
  }

  // Paddle Collides With Ball
  if(ball->getSprite()->getPosition().y + ball->getSprite()->getGlobalBounds().height
     > paddle->getSprite()->getPosition().y
     && ball->getSprite()->getPosition().x + ball->getSprite()->getGlobalBounds().width
        > paddle->getSprite()->getPosition().x
     && ball->getSprite()->getPosition().x
        < paddle->getSprite()->getPosition().x+ paddle->getSprite()->getGlobalBounds().width
    )
  {
    ball->ball_direction->y = -ball->ball_direction->y;
  }
  // Paddle Collides With Gem
  for (int i = 0; i < gems_amount; i++)
  {
    if(collisionCheck(*gems[i].getSprite(), *paddle->getSprite())
       && gems[i].getVisibility())
    {
      gems[i].setVisibility(false);
      player_score += 10;
      score_text.setString(std::to_string(player_score));
    }
  }

  for (int i = 0; i < grid_size*grid_size; i++)
  {
    if(collisionCheck(*ball->getSprite(), *blocks[i].getSprite())
       && blocks[i].getVisibility())
    {
      if (ball->getSprite()->getPosition().x < blocks[i].getSprite()->getPosition().x
                                                 - ball->getSprite()->getGlobalBounds().width
          && ball->getSprite()->getPosition().x > blocks[i].getSprite()->getPosition().x
                                                    - ball->getSprite()->getGlobalBounds().width)
        // Collision from Left
      {
        blocks[i].setVisibility(false);
        ball->ball_direction->x = -ball->ball_direction->x;
        player_score += 5;
        score_text.setString(std::to_string(player_score));
      }
      else if (ball->getSprite()->getPosition().x + ball->getSprite()->getGlobalBounds().width
                 > blocks[i].getSprite()->getPosition().x
               && ball->getSprite()->getPosition().x + ball->getSprite()->getGlobalBounds().width
                 < blocks[i].getSprite()->getPosition().x + ball->getSprite()->getGlobalBounds().width)
        // Collision from Right
      {
        blocks[i].setVisibility(false);
        ball->ball_direction->x = -ball->ball_direction->x;
        player_score += 5;
        score_text.setString(std::to_string(player_score));
      }
      else if(ball->getSprite()->getPosition().y < blocks[i].getSprite()->getPosition().y
                                                      - ball->getSprite()->getGlobalBounds().height
              && ball->getSprite()->getPosition().y > blocks[i].getSprite()->getPosition().y
                                                         - ball->getSprite()->getGlobalBounds().height)
        // Collision from Bottom
      {
        blocks[i].setVisibility(false);
        ball->ball_direction->y = -ball->ball_direction->y;
        player_score += 5;
        score_text.setString(std::to_string(player_score));
      }
      else if(ball->getSprite()->getPosition().y + ball->getSprite()->getGlobalBounds().height
                 > blocks[i].getSprite()->getPosition().y
              && ball->getSprite()->getPosition().y + ball->getSprite()->getGlobalBounds().height
                < blocks[i].getSprite()->getPosition().y + ball->getSprite()->getGlobalBounds().height)
        // Collision from Top
      {
        blocks[i].setVisibility(false);
        ball->ball_direction->y = -ball->ball_direction->y;
        player_score += 5;
        score_text.setString(std::to_string(player_score));
      }
    }
  }
}

void Game::spawnSprites()
{
  if (player_lives == 5)
  {
    blocks->getSprite()->setPosition(blocks->blocks_position->x,
                                     blocks->blocks_position->y);
    for (int i = 0; i < grid_size*grid_size; ++i) // Draw Blocks
    {
      blocks[i].setVisibility(true);
      window.draw(*blocks[i].getSprite());
    }
  }
  paddle->getSprite()->setPosition(paddle->paddle_position->x,
                                   paddle->paddle_position->y);
  ball->getSprite()->setPosition(ball->ball_position->x,
                                 ball->ball_position->y);
}

void Game::textSetup()
{
  if (!font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "Font did not load \n";
  }
  enter_text.setString("[Enter] to start the game");
  enter_text.setFont(font);
  enter_text.setCharacterSize(15);
  enter_text.setFillColor(sf::Color::White);
  enter_text.setPosition(
    window.getSize().x / 2.0f - enter_text.getGlobalBounds().width / 2,
    window.getSize().y / 3.6f - enter_text.getGlobalBounds().height / 2);

  quit_text.setString("[Esc] to quit");
  quit_text.setFont(font);
  quit_text.setCharacterSize(15);
  quit_text.setFillColor(sf::Color::White);
  quit_text.setPosition(
    window.getSize().x / 2.0f - quit_text.getGlobalBounds().width / 2,
    window.getSize().y / 3.2f - quit_text.getGlobalBounds().height / 2);

  controls_text.setString("[A] to move paddle left\n"
                          "[D] to move paddle right\n"
                          "[Esc] to quit at anytime");
  controls_text.setFont(font);
  controls_text.setCharacterSize(15);
  controls_text.setFillColor(sf::Color::White);
  controls_text.setPosition(10, 600);

  lives_text.setString(std::to_string(player_lives));
  lives_text.setFont(font);
  lives_text.setCharacterSize(20);
  lives_text.setFillColor(sf::Color::White);
  lives_text.setPosition(1020, 10);

  score_text.setString(std::to_string(player_score));
  score_text.setFont(font);
  score_text.setCharacterSize(20);
  score_text.setFillColor(sf::Color::White);
  score_text.setPosition(10, 10);

  winner_text.setString("Congratulations, you win!");
  winner_text.setFont(font);
  winner_text.setCharacterSize(50);
  winner_text.setFillColor(sf::Color::White);
  winner_text.setPosition(window.getSize().x / 2.0f - winner_text.getGlobalBounds().width / 2,
                          window.getSize().y / 3.3f - winner_text.getGlobalBounds().height / 2);

  loser_text.setString("You Lost!");
  loser_text.setFont(font);
  loser_text.setCharacterSize(50);
  loser_text.setFillColor(sf::Color::White);
  loser_text.setPosition(window.getSize().x / 2.0f - loser_text.getGlobalBounds().width / 2,
                         window.getSize().y / 3.2f - loser_text.getGlobalBounds().height / 2);

  to_menu_text.setString("[Enter] to return to menu");
  to_menu_text.setFont(font);
  to_menu_text.setCharacterSize(20);
  to_menu_text.setFillColor(sf::Color::White);
  to_menu_text.setPosition(window.getSize().x / 2.0f - to_menu_text.getGlobalBounds().width / 2,
                           window.getSize().y / 2.6f - to_menu_text.getGlobalBounds().height / 2);

  extra_controls_text.setString("[Z] to decrease ball speed\n"
                                "[X] to increase ball speed");
  extra_controls_text.setFont(font);
  extra_controls_text.setCharacterSize(15);
  extra_controls_text.setFillColor(sf::Color::White);
  extra_controls_text.setPosition(10, 660);

}


/*void Game::mouseClicked(sf::Event event) // Mouse not used
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);

}*/
