#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include "Pvector.h"
#include <iostream>


class Npc
{
public:


	Npc(sf::Vector2f t_postion, sf::RenderWindow& t_window, int t_type, float t_maxArriveSpeed);
	Npc(sf::Vector2f t_postion, sf::RenderWindow& t_window, int t_type);
	~Npc();
	

	sf::Vector2f getPostion();//gets the postion of the npc
	sf::Vector2f getSpeed();//gets the speed of the npc 
	sf::Vector2f getSize();//gets the size of the npc 
	sf::Sprite getSprite();//gets the sprite refrence 

	void setPostion(sf::Vector2f t_postion);// set the postion of the npc 
	void setSpeed(sf::Vector2f t_speed);//set the speed of the npc 

	void setUpSprite();//set up the sprite on start up
	void setUpText(sf::Font t_font);//set up fonts and texts on startup


	void applyForce(sf::Vector2f t_force);



	void move();//moves npc
	void updateArc();//updates arcs 

	void checkBoundries();//check boundries for the npc
	void draw();//draw the npc and text and arc 

	void updatePlayerData(sf::Vector2f t_playerPostion, float t_playerAngle, sf::Vector2f t_playerVelocity);

	void setSpitePos();

	void move2();//moves npc


private:

	sf::RenderWindow& m_window;
	sf::Font m_font;

	sf::Text m_behaviourText;

	sf::Texture m_texture; // texture used for Npc
	sf::Sprite m_sprite; // sprite used for npc
	sf::Vector2f m_position;//postion of the npc sprite.	
	sf::Vector2f m_velocity = { 1.0f , 0.0f };//speed of the npc sprite.
	sf::Vector2f m_size; //size of the npc
	sf::Vector2f m_direction = { 0.0f, 0.0f };// direction of the NPC 

	float m_randDirection = 0.0f; // random choice of direction (-1 ,0, 1) (left ,straight,right)
	float m_magnitude = 0.0f; // magnitude used for calculating the NPC direction
	float m_angle = 0.0f; // angle of the NPC 
	float m_maxAngle = 10.0f;//maximum angle the npc wanders 
	float const MAX_SPEED = 2.0f;//max NPC speed

	sf::Vector2f m_playerPostion = { 0.0f,0.0f };//the postion of the player object 
	sf::Vector2f m_playerVelocity = { 0.0f,0.0f };//the velocity of the player object 


	/// <summary>
	/// used in update arc function to create and maintain the arc
	/// </summary>
	/// 
	sf::VertexArray m_arc{ sf::TriangleFan, 3 };
	float m_fov = 25; //field of view for half the view 
	float m_arclength = 300; //lenght of the arc 
	float m_currentAngle = m_sprite.getRotation();
	float m_leftAngleFov = 0;
	float m_rightAngleFov = 0;
	sf::Vector2f m_leftCornerArc = { 0.0f,0.0f };
	sf::Vector2f m_rightCornerArc = { 0.0f,0.0f };
	sf::Vector2f m_distanceVector = { 0.0f,0.0f };
	sf::Vector2f m_unitVectorPostion = { 0.0f,0.0f };
	sf::Vector2f m_unitvectorDistance = { 0.0f,0.0f };
	float m_unitVectorDotProduct = 0;
	float m_angleBetweenPlayer = 0;

	/// <summary>
	/// variables used in purrsue sunction
	/// </summary>
	float m_maxArriveSpeed = 0;
	float m_speed;
	float m_timePrediction = 0;
	sf::Vector2f m_newTargetPostion = { 0.0f,0.0f };

	int m_type = 0;//what type of behaviour the npc has 

	void kinematicWander();//function to make the npc wander around the screen aimlessley 
	float m_playerAngle = 0.0;//the angle the player will move

	void KinematicSeek();//seeks the player 
	void KinematicArrive();//when gets within a range of the player react diffrently
	float  m_distance = 0.0f;//distance from player
	float m_playerArriveRadius = 300.0f;//the radius around the player where npc changes behaviour

	void kinematicFlee();//function to make the player flee the player 
	float m_playerFleeRadius = 400.0f;//radius from the player where the npc will start to flee
	bool m_fleeing = false;//bool to say if the npc is fleeing 

	void kinematicPursue();



};





#endif // !NPC_H

