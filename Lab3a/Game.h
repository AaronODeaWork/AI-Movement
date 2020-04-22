#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
/// 
#include <SFML/Graphics.hpp>
#include "Pvector.h"
#include "Player.h"
#include "Npc.h"


class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:


	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();
	void setupSprite();
	void setUpAliens();


	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message


	sf::Vector2f m_startingPostion{ m_window.getSize().x * .5f, m_window.getSize().y * .5f };


	Player m_player{ m_startingPostion , m_window };
	
	std::vector<sf::Vector2f>m_alienPostions;
	std::vector<Npc> m_aliens;
	


	void vFormation();


	bool m_exitGame; // control exiting game



};

#endif // !GAME_HPP

