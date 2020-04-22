#include "Npc.h"

Npc::Npc(sf::Vector2f t_postion, sf::RenderWindow& t_window, int t_type ,float t_maxArriveSpeed) :
	m_position(t_postion),
	m_window(t_window),
	m_type(t_type),
	m_maxArriveSpeed(t_maxArriveSpeed)
{
}
Npc::Npc(sf::Vector2f t_postion, sf::RenderWindow& t_window, int t_type):
	m_position(t_postion),
	m_window(t_window),
	m_type(t_type)
{
}
Npc::~Npc()
{
}

sf::Vector2f Npc::getPostion()
{
	return m_position;
}
sf::Vector2f Npc::getSpeed()
{
	return m_velocity;
}
sf::Vector2f Npc::getSize()
{
	return m_size;
}
sf::Sprite Npc::getSprite()
{
	return m_sprite;
}

void Npc::setPostion(sf::Vector2f t_postion)
{
	m_position = t_postion;
}
void Npc::setSpeed(sf::Vector2f t_speed)
{
	m_velocity = t_speed;
}
void Npc::setUpSprite()
{
	if (!m_texture.loadFromFile("ASSETS\\IMAGES\\alien.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading player sprite" << std::endl;
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_texture.getSize().x * .5f, m_texture.getSize().y * .5f);
	m_sprite.setPosition(m_position);
	m_size.x = m_texture.getSize().x;
	m_size.y = m_texture.getSize().y;

}
void Npc::setUpText(sf::Font t_font)
{
	m_font = t_font;
	m_behaviourText.setFont(m_font);


	switch (m_type)
	{
	default:
		m_behaviourText.setString("test");
		break;
	case 0:
		m_behaviourText.setString("Wander");
		break;
	case 1:
		m_behaviourText.setString("Seek");
		break;
	case 2:
		m_behaviourText.setString("Flee");
		break;
	case 3:
		m_behaviourText.setString("Arrive");
		break;
	case 4:
		m_behaviourText.setString("Pursue");
		break;
	}
	m_behaviourText.setFillColor(sf::Color::White);

}

void Npc::applyForce(sf::Vector2f t_force)
{
	setSpeed(getSpeed() + t_force);
}

void Npc::move()
{


	switch (m_type)
	{
	default:
		kinematicWander();
		break;
	case 0:
		kinematicWander();
		break;
	case 1:
		KinematicSeek();
		break;
	case 2:
		kinematicFlee();
		break;
	case 3:
		KinematicArrive();
		break;
	case 4:
		kinematicPursue();
		break;

	}

	m_behaviourText.setPosition(m_position.x, m_position.y+60);
	m_position += m_velocity;
	m_sprite.setPosition(m_position);
}
/// <summary>
/// function which checks the npc boundries and loops if it leaves the screen
/// </summary>
void Npc::checkBoundries()
{
	if (m_position.x > m_window.getSize().x + (m_size.x) * .5f)
	{
		m_position.x = -(m_size.x * .5f);
		m_sprite.setPosition(m_position);
	}
	else if (m_position.x < -(m_size.x) * .5f)
	{
		m_position.x = m_window.getSize().x + (m_size.x * .5f);
		m_sprite.setPosition(m_position);
	}

	if (m_position.y > (m_window.getSize().y) + (m_size.y) * .5f)
	{
		m_position.y = -(m_size.y * .5f);
		m_sprite.setPosition(m_position);
	}
	else if (m_position.y < -(m_size.y) * .5f)
	{
		m_position.y = m_window.getSize().y + (m_size.y * .5f);
		m_sprite.setPosition(m_position);
	}
}

void Npc::draw()
{
	m_window.draw(m_sprite);
	m_window.draw(m_arc);
	m_window.draw(m_behaviourText);
}

void Npc::updatePlayerData(sf::Vector2f t_playerPostion, float t_playerAngle , sf::Vector2f t_playerVelocity)
{
	m_playerPostion = t_playerPostion;
	m_playerVelocity = t_playerVelocity;
	m_playerAngle = t_playerAngle;
}
/// <summary>
/// function that updates and maintains the arc that the npc uses to detect the player.
/// </summary>
void Npc::updateArc()
{
	m_currentAngle = m_sprite.getRotation();//gets current angle 

	m_leftAngleFov = (m_currentAngle - m_fov) * (3.14159 / 180); // get the current for angle for left arc
	m_rightAngleFov = (m_currentAngle + m_fov) * (3.14159 / 180); // get the current for angle for right arc

	m_leftCornerArc.x = m_arclength * std::cos(m_leftAngleFov); // get the postion of the left arc
	m_leftCornerArc.y = m_arclength * std::sin(m_leftAngleFov);
	m_leftCornerArc += m_position;

	m_rightCornerArc.x = m_arclength * std::cos(m_rightAngleFov);// get the postion of the right arc
	m_rightCornerArc.y = m_arclength * std::sin(m_rightAngleFov);
	m_rightCornerArc += m_position;

	m_arc[0].position = m_position;
	m_arc[1].position = m_leftCornerArc;
	m_arc[2].position = m_rightCornerArc;
	

	m_distanceVector = m_playerPostion - m_position;
	m_distance = thor::length(m_distanceVector);

	if (m_distance < m_arclength)
	{
		if (m_velocity.x <= 0 && m_velocity.y <= 0)
		{
			m_unitVectorPostion = thor::unitVector(sf::Vector2f(0.1f,0.1f));
		}
		else
		{
			m_unitVectorPostion = thor::unitVector(m_velocity);
		}
		m_unitvectorDistance = thor::unitVector(m_distanceVector);


		m_unitVectorDotProduct = thor::dotProduct(m_unitVectorPostion, m_unitvectorDistance);
		if (m_unitVectorDotProduct > 1.0f)
		{
			m_unitVectorDotProduct = 1.0f;
		}

		m_angleBetweenPlayer = std::acos(m_unitVectorDotProduct) * (180/3.14159);
		if (m_angleBetweenPlayer < m_fov)
		{
			/// <summary>
			/// set the color of the arc to white 
			/// </summary>
			m_arc[0].color = sf	::Color(255,0,0, 100);
			m_arc[1].color = sf::Color(255,0,0, 100);
			m_arc[2].color = sf::Color(255, 0, 0, 100);
		}
	}
	else
	{
		/// <summary>
		/// set the color of the arc to red 
		/// </summary>
		m_arc[0].color = sf::Color(255, 255, 255, 100);
		m_arc[1].color = sf::Color(255, 255, 255, 100);
		m_arc[2].color = sf::Color(255, 255, 255, 100);
	}


}


/// <summary>
/// behaviour for the npc to wander randomly around the screen 
/// </summary>
void Npc::kinematicWander()
{
	m_randDirection = (rand() % 3) - 1;
	m_maxAngle = (rand() % 11);
	m_angle = m_angle + (m_maxAngle * m_randDirection);

	m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
	m_direction = sf::Vector2f(std::cos(m_angle * (3.14159265358979 / 180)), std::sin(m_angle * (3.14159265358979 / 180)));
	m_velocity = m_magnitude * (sf::Vector2f(std::cos(m_angle * (3.14159265358979 / 180)), std::sin(m_angle * (3.14159265358979 / 180))));
	m_velocity += m_direction;


	if (MAX_SPEED < m_magnitude)
	{
		m_velocity = m_direction * MAX_SPEED;
	}
	m_sprite.setRotation(m_angle);


}
/// <summary>
/// behaviour for the npc to seek the player at a constant speed  
/// </summary>
void Npc::KinematicSeek()
{


	m_direction.x = m_playerPostion.x - m_position.x; // direction for x
	m_direction.y = m_playerPostion.y - m_position.y; //direction for y

	m_distance = thor::length(m_direction);//find the distance between the two points 
	m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
	m_angle = std::atan(m_direction.y / m_direction.x) * (180 / 3.14159265358979);

	if (m_angle <= 0)
	{
		m_angle = m_angle - 1;
		m_angle += 180;
	}
	if (m_playerPostion.y < m_position.y)
	{
		m_angle += 180;
	}
	m_sprite.setRotation(m_angle);

	if (m_distance >= m_playerArriveRadius) //if the distance is more than this move the player to that point 
	{
		m_velocity = (m_direction / (m_distance * .2f));
	}

}
/// <summary>
/// behaviour for the npc to seek the player but slow down and stop when near the player 
/// </summary>
void Npc::KinematicArrive()
{
	
	m_direction.x = m_playerPostion.x - m_position.x; // direction for x
	m_direction.y = m_playerPostion.y - m_position.y; //direction for y

	m_distance = thor::length(m_direction);//find the distance between the two points 
	m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
	m_angle = std::atan(m_direction.y / m_direction.x) * (180 / 3.14159265358979);

	if (m_angle <= 0)
	{
		m_angle = m_angle - 1;
		m_angle += 180;
	}
	if (m_playerPostion.y < m_position.y)
	{
		m_angle += 180;
	}
	m_sprite.setRotation(m_angle);

	if (m_distance >= m_playerArriveRadius) //if the distance is more than this move the player to that point 
	{
		m_velocity = (m_direction / (m_distance * m_maxArriveSpeed));
	}
	else 
	{
		m_velocity = ((m_direction / m_distance) * .5f);
	}
	if (m_distance < 100)
	{
		m_velocity = { 0.01f,0.01f };
	}



}
/// <summary>
/// behaviour for the npc to flee the player when it is in range of the player 
/// </summary>
void Npc::kinematicFlee()
{
	m_direction.x = m_playerPostion.x - m_position.x; // direction for x
	m_direction.y = m_playerPostion.y - m_position.y; //direction for y

	m_distance = std::sqrt((m_direction.x * m_direction.x) + (m_direction.y * m_direction.y)); //find the distance between the two points 



	if (m_distance >= m_playerFleeRadius && m_fleeing == false) //if the distance is more than this move the player to that point 
	{
		m_behaviourText.setString("Wander(will flee)");
		kinematicWander();
	}
	else if (m_distance <= m_playerFleeRadius || m_fleeing == true)
	{
		m_behaviourText.setString("Flee");
		m_fleeing = true;
		m_angle = std::atan(m_direction.y / m_direction.x) * (180 / 3.14159265358979);

		if (m_angle <= 0)
		{
			m_angle = m_angle - 1;
			m_angle += 180;
		}
		if (m_playerPostion.y > m_position.y) { m_angle += 180; }
		m_sprite.setRotation(m_angle);

		m_velocity = (m_direction / (m_distance * .2f)) * -1.0f;

		if (m_distance >= m_playerFleeRadius * 2) { m_fleeing = false; }
	}


}
/// <summary>
/// behaviour for the npc to  pursue the npc and predict where the player might be 
/// </summary>
void Npc::kinematicPursue()
{

	m_direction = m_playerPostion - m_position;
	m_distance = thor::length(m_direction);
	m_speed = thor::length(m_velocity);
	if (m_speed <= m_distance / 1000)
	{
		m_timePrediction = 1000;
	}
	else
	{
		m_timePrediction = m_distance / m_speed;
	}
	m_newTargetPostion = m_playerPostion + m_playerVelocity * m_timePrediction;


	
	m_direction.x = m_newTargetPostion.x - m_position.x; // direction for x
	m_direction.y = m_newTargetPostion.y - m_position.y; //direction for y

	m_distance = thor::length(m_direction);//find the distance between the two points 

	m_magnitude = std::sqrt(std::pow(m_velocity.x, 2) + std::pow(m_velocity.y, 2));
	m_angle = std::atan(m_direction.y / m_direction.x) * (180 / 3.14159265358979);

	if (m_angle <= 0)
	{
		m_angle = m_angle - 1;
		m_angle += 180;
	}
	if (m_playerPostion.y < m_position.y)
	{
		m_angle += 180;
	}
	m_sprite.setRotation(m_angle);

	if (m_distance >= m_playerArriveRadius) //if the distance is more than this move the player to that point 
	{
		m_velocity = (m_direction / (m_distance * .5f));
	}
	
}


void Npc::setSpitePos()
{
	m_sprite.setPosition(m_position);
}

void Npc::move2()
{

	m_behaviourText.setPosition(m_position.x, m_position.y + 60);
	m_position += m_velocity;
	m_sprite.setPosition(m_position);
	

}
