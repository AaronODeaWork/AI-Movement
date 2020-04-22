/// <summary>
/// @author Aaron O'Dea
/// @date September 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 2000U, 1600U, 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	std::srand(std::time(nullptr));

	setUpAliens();//set up aliens;
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	m_player.input();

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	for (int i = 0; i< m_aliens.size(); i++)
	{
		m_aliens[i].updatePlayerData(m_player.getPostion(), m_player.getAngle(), m_player.getVelocity());
		m_aliens[i].move();
		
		m_aliens[i].checkBoundries();
		m_aliens[i].updateArc();
	}

	vFormation();

	m_player.move();
	m_player.checkBoundries();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_player.draw();


	for (int i = 0; i < m_aliens.size(); i++)
	{
		m_aliens[i].draw();
	}


	m_window.display();

}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	for (int i = 0; i < m_aliens.size(); i++)
	{
		m_aliens[i].setUpText(m_ArialBlackfont);
	}

}

/// <summary>
/// load the texture and setup the sprites 
/// </summary>
void Game::setupSprite()
{
	for (int i = 0; i < m_aliens.size(); i++)
	{
		m_aliens[i].setUpSprite();
	}

	m_player.setUpSprite();
}

void Game::setUpAliens()
{

	m_alienPostions.reserve(5);
	m_aliens.reserve(5);

	m_alienPostions.push_back(sf::Vector2f(300.0f, 200.0f));
	m_alienPostions.push_back(sf::Vector2f(600.0f, 200.0f));
	m_alienPostions.push_back(sf::Vector2f(900.0f, 200.0f));
	m_alienPostions.push_back(sf::Vector2f(1200.0f, 200.0f));
	m_alienPostions.push_back(sf::Vector2f(1500.0f, 200.0f));


	m_aliens.push_back(Npc(m_alienPostions[0], m_window, 0));
	m_aliens.push_back(Npc(m_alienPostions[1], m_window, 1));
	m_aliens.push_back(Npc(m_alienPostions[2], m_window, 3, .4f));
	m_aliens.push_back(Npc(m_alienPostions[3], m_window, 3, .2f));
	m_aliens.push_back(Npc(m_alienPostions[4], m_window, 4));


}

void Game::vFormation()
{

	float pi = 3.141592653589793;
	int fSize = 1;

	for (int i = 0; i < fSize; i++)
	{

		Pvector	sub(0, 0);
		Pvector sum(0, 0);
		int npcRadius = m_aliens[i].getSize().x;
		int closeEnough = 200;
		float angleAroundCircle = 0.0;
		Pvector targetSlot(0, 0);

			//Find our position in the circle
			angleAroundCircle = (float)i / (fSize-1);
			angleAroundCircle = angleAroundCircle * pi * 2;

			float radius = npcRadius / sin(pi / (fSize));

			targetSlot.x = m_player.getPostion().x;
			targetSlot.y = m_player.getPostion().y;

			targetSlot.x = targetSlot.x; + radius * cos(angleAroundCircle);
			targetSlot.y = targetSlot.y; + radius * sin(angleAroundCircle);

			Pvector alienpos;
			alienpos.x = m_aliens[i].getPostion().x;
			alienpos.y = m_aliens[i].getPostion().y;


			sub = sub.subTwoVector(targetSlot, alienpos);

			float D = sub.magnitude();
			if (D > closeEnough)
			{
				sum = sub;
				sum.normalize();
				//sum.mulScalar(2);

				sf::Vector2f sumDone;
				sumDone.x = sum.x;
				sumDone.y = sum.y;
				m_aliens[i].applyForce(sumDone); 

			}
			else
			{
				m_aliens[i].setSpeed(m_player.getSpeed()); //Match the leader's velocity if we are close enough
			}
		

			m_aliens[i].checkBoundries();
			m_aliens[i].updateArc();
			m_aliens[i].move2();
	}

	
	
}


