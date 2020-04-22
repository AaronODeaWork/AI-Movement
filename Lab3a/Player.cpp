#include "Player.h"

Player::Player(sf::Vector2f t_postion, sf::RenderWindow& t_window) :
	m_position(t_postion),
	m_window(t_window)
{
}

Player::~Player()
{
}

sf::Vector2f Player::getPostion()
{
	return m_position;
}
sf::Vector2f Player::getSpeed()
{
	return m_velocity;
}
sf::Vector2f Player::getSize()
{
	return m_size;
}
sf::Vector2f Player::getVelocity()
{
	return m_velocity;
}
float Player::getAngle()
{
	return m_angle;
}
sf::Sprite Player::getSprite()
{
	return m_sprite;
}

void Player::setPostion(sf::Vector2f t_postion)
{
	m_position = t_postion;
}
void Player::setSpeed(sf::Vector2f t_speed)
{
	m_velocity = t_speed;
}
void Player::setUpSprite()
{
	if (!m_texture.loadFromFile("ASSETS\\IMAGES\\player.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading player sprite" << std::endl;
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x * .5f, m_texture.getSize().y * .5f);
	m_sprite.setPosition(m_position);
	m_size.x = m_texture.getSize().x;
	m_size.y = m_texture.getSize().y;
	m_sprite.setRotation(m_angle + 90);
}

void Player::move()
{
	if (MAX_SPEED < m_magnitude)
	{
		m_velocity = m_direction * MAX_SPEED;
	}
	m_position += m_velocity;
	m_sprite.setPosition(m_position);
}

void Player::input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//get the magnitude of the velocity vector
		m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));

		if (m_magnitude != 0)//if the magnitude is not 0 then you can get the direction vector 
		{
			m_direction = m_velocity / m_magnitude;
		}
		else
		{
			m_direction = sf::Vector2f(std::cos(m_angle * (3.14159265358979 / 180)), std::sin(m_angle * (3.14159265358979 / 180)));
		}
		m_velocity += m_direction;

	}//reduce the velocity by  20 % each button press 
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_velocity *= .8f;
		if (0.1f > m_magnitude)//if the magnitude is less than 0 stop the sprite 
		{
			m_velocity = sf::Vector2f(0, 0);
		}
	}

	//left right inputs for changeing the rotation of the player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_angle -= m_rotationAmount;
		m_sprite.setRotation(m_angle + 90);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_angle += m_rotationAmount;
		m_sprite.setRotation(m_angle + 90);
	}

	//if the ship is rotating get the new velocity vector 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
		m_velocity = m_magnitude * (sf::Vector2f(std::cos(m_angle * (3.14159265358979 / 180)), std::sin(m_angle * (3.14159265358979 / 180))));
	}
}

void Player::checkBoundries()
{
	if (m_position.x > m_window.getSize().x + (m_size.x) * .5f)
	{
		m_position.x = -(m_size.x * .5f);
		m_sprite.setPosition(m_position);
	}
	if (m_position.x < -(m_size.x) * .5f)
	{
		m_position.x = m_window.getSize().x + (m_size.x * .5f);
		m_sprite.setPosition(m_position);
	}

	if (m_position.y > (m_window.getSize().y) + (m_size.y) * .5f)
	{
		m_position.y = -(m_size.y * .5f);
		m_sprite.setPosition(m_position);
	}
	if (m_position.y < -(m_size.y) * .5f)
	{
		m_position.y = m_window.getSize().y + (m_size.y * .5f);
		m_sprite.setPosition(m_position);
	}
}

void Player::draw()
{
	m_window.draw(m_sprite);
}
