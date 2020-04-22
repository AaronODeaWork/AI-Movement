#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Player
{
public:
	Player(sf::Vector2f t_postion, sf::RenderWindow& t_window);
	~Player();

	sf::Vector2f getPostion();
	sf::Vector2f getSpeed();
	sf::Vector2f getSize();
	sf::Vector2f getVelocity();

	float getAngle();
	sf::Sprite	 getSprite();

	void setPostion(sf::Vector2f t_postion);
	void setSpeed(sf::Vector2f t_speed);

	void setUpSprite();
	void move();
	void input();

	void checkBoundries();
	void draw();

private:

	sf::RenderWindow& m_window;

	sf::Texture m_texture; // texture used for Player
	sf::Sprite m_sprite; // sprite used for Player

	sf::Vector2f m_origin = { 0.0f ,0.0f };//origin of the player sprite 
	sf::Vector2f m_size; //size of player
	sf::Vector2f m_position;//postion of the Player sprite
	sf::Vector2f m_direction = { 0.0f, 0.0f };// direction of the player 
	sf::Vector2f m_velocity = { 1.0f, 0.0f };//speed of the Player sprite.

	float m_magnitude = 0.0f; // magnitude used for calculating the players direction
	float m_angle = 0.0f; // angle of the player 
	float const MAX_SPEED = 20.0f;//max player speed

	float m_rotationAmount = 1.5f;//max player speed
};


#endif // !PLAYER_H
