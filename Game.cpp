#include "Game.h"

Game::Game(unsigned int& screenWidth, unsigned int& screenHeight, const char*& title)
{
	screenWidth = 640;
	screenHeight = 480;
	title = "The Game";

	m_screenSize = sf::Vector2i(screenWidth, screenHeight);

	m_grid = new Grid(20, screenWidth, screenHeight);

	m_gui = Gui(m_grid->getBottomRowPos(), 3, m_grid->getTileSize());
	m_startMenu = new StartMenu(sf::Vector2f(static_cast<float>(screenWidth), static_cast<float>(screenHeight)), this);

	m_grassRect = m_grid->getGrassRect();
	m_waterRect = m_grid->getWaterRect();

	/*m_frog = new Frog(m_grid->getPosByIndex(9, 13), m_grid->getTileSize(), screenWidth - m_grid->getTileSize());*/

	m_deleter[0] = sf::RectangleShape(sf::Vector2f(10, 160));
	m_deleter[0].setFillColor(sf::Color::Cyan);
	m_deleter[0].setPosition(m_grid->getPosByIndex(10, 8).y, static_cast<float>(screenWidth + m_grid->getTileSize() * 2));

	m_deleter[1] = sf::RectangleShape(sf::Vector2f(10, 160));
	m_deleter[1].setFillColor(sf::Color::Cyan);
	m_deleter[1].setPosition(m_grid->getPosByIndex(10, 8).y, static_cast<float>(0 - m_grid->getTileSize() * 2));

	m_carTemplates[0] = new Car(sf::Vector2f(0, 0), 25);
	m_carTemplates[1] = new Truck(sf::Vector2f(0, 0), 25);
	m_carTemplates[2] = new CarFast(sf::Vector2f(0, 0), 25);
	m_carTemplates[3] = new CarSlow(sf::Vector2f(0, 0), 25);
	m_carTemplates[4] = new CarHotDog(sf::Vector2f(0, 0), 25);

	m_spawnerPrototype[0] = new Car(sf::Vector2f(0, 0), 25);
	m_spawnerPrototype[1] = new Car(sf::Vector2f(0, 0), 25);
	m_spawnerPrototype[2] = new Car(sf::Vector2f(0, 0), 25);
	m_spawnerPrototype[3] = new Car(sf::Vector2f(0, 0), 25);
	m_spawnerPrototype[4] = new Car(sf::Vector2f(0, 0), 25);


	m_carSpawner[0] = new CarSpawner(m_spawnerPrototype[0], m_grid->getPosByIndex(0, 12) + sf::Vector2f(static_cast<float>(-m_grid->getTileSize()), 0));
	m_carSpawner[0]->addObserver(this);

	m_carSpawner[1] = new CarSpawner(m_spawnerPrototype[1], m_grid->getPosByIndex(19, 11) + sf::Vector2f(static_cast<float>(m_grid->getTileSize()), 0));
	m_carSpawner[1]->addObserver(this);

	m_carSpawner[2] = new CarSpawner(m_spawnerPrototype[2], m_grid->getPosByIndex(0, 10) + sf::Vector2f(static_cast<float>(-m_grid->getTileSize()), 0));
	m_carSpawner[2]->addObserver(this);

	m_carSpawner[3] = new CarSpawner(m_spawnerPrototype[3], m_grid->getPosByIndex(19, 9) + sf::Vector2f(static_cast<float>(m_grid->getTileSize()), 0));
	m_carSpawner[3]->addObserver(this);

	m_carSpawner[4] = new CarSpawner(m_spawnerPrototype[4], m_grid->getPosByIndex(0, 8) + sf::Vector2f(static_cast<float>(-m_grid->getTileSize()), 0));
	m_carSpawner[4]->addObserver(this);

	// Vertex graphic design
	{
		m_water = sf::VertexArray(sf::Quads, 4);

		m_water[0].position = sf::Vector2f(m_grid->getPosByIndex(0, 2));
		m_water[1].position = sf::Vector2f(static_cast<float>(screenWidth), m_grid->getPosByIndex(19, 2).y);
		m_water[2].position = sf::Vector2f(m_grid->getPosByIndex(0, 7));
		m_water[3].position = sf::Vector2f(static_cast<float>(screenWidth), m_grid->getPosByIndex(19, 7).y);
	}
	
	m_cutoffPoint = m_grid->getPosByIndex(0, 7).y;
	m_cutoffPoint2 = m_grassRect[0]->getPosition().y;

	int logMoveSpeed = 50;
	m_log[0] = new Log(m_grid->getTileSize(), 3, m_grid->getPosByIndex(18, 2), static_cast<float>(logMoveSpeed));
	m_log[1] = new Log(m_grid->getTileSize(), 2, m_grid->getPosByIndex(14, 3), static_cast<float>(-logMoveSpeed));
	m_log[2] = new Log(m_grid->getTileSize(), 3, m_grid->getPosByIndex(13, 4), static_cast<float>(logMoveSpeed));
	m_log[3] = new Log(m_grid->getTileSize(), 4, m_grid->getPosByIndex(14, 5), static_cast<float>(-logMoveSpeed));
	m_log[4] = new Log(m_grid->getTileSize(), 3, m_grid->getPosByIndex(15, 6), static_cast<float>(logMoveSpeed));
	
	m_log[5] = new Log(m_grid->getTileSize(), 2, m_grid->getPosByIndex(6, 2), static_cast<float>(logMoveSpeed));
	m_log[6] = new Log(m_grid->getTileSize(), 4, m_grid->getPosByIndex(3, 3), static_cast<float>(-logMoveSpeed));
	m_log[7] = new Log(m_grid->getTileSize(), 4, m_grid->getPosByIndex(3, 4), static_cast<float>(logMoveSpeed));
	m_log[8] = new Log(m_grid->getTileSize(), 2, m_grid->getPosByIndex(7, 5), static_cast<float>(-logMoveSpeed));
	m_log[9] = new Log(m_grid->getTileSize(), 3, m_grid->getPosByIndex(8, 6), static_cast<float>(logMoveSpeed));
	

	if (!m_splashBuffer.loadFromFile("../Res/sound-frogger-plunk.wav"))
	{

	}
	else
	{
		m_splashSound.setBuffer(m_splashBuffer);
	}

	if (!m_squashBuffer.loadFromFile("../Res/sound-frogger-squash.wav"))
	{

	}
	else
	{
		m_squashSound.setBuffer(m_squashBuffer);
	}
}

Game::~Game()
{
	delete m_frog;
	delete m_grid;

	int s = m_cars.size() - 1;
	for (int i = 0; i < s; i++)
	{
		delete m_cars[0];
		m_cars[0] = nullptr;
		m_cars.erase(m_cars.begin());
	}

	delete[] &m_carTemplates;
	delete[] &m_spawnerPrototype;
	delete[] & m_carSpawner;
	delete[] &m_log;

	delete m_startMenu;

	for (auto& a : m_goalFrog)
	{
		delete a;
	}

	s = m_grassRect.size() - 1;
	for (int i = 0; i < s; i++)
	{
		delete m_grassRect[0];
		m_grassRect[0] = nullptr;
		m_grassRect.erase(m_grassRect.begin());
	}

	s = m_waterRect.size() - 1;
	for (int i = 0; i < s; i++)
	{
		delete m_waterRect[0];
		m_waterRect[0] = nullptr;
		m_waterRect.erase(m_waterRect.begin());
	}
}

bool Game::Update(float deltaTime)
{
	if (m_frog != nullptr)
	{
		m_frog->update(deltaTime);
	}	

	for (auto& a : m_carSpawner)
	{
		if(a != nullptr)
			a->update(deltaTime);
	}	

	for (auto& a : m_cars)
	{
		if (a != nullptr)
			a->update(deltaTime);
	}

	for (auto& a : m_log)
	{
		if (a != nullptr)
		{
			a->update(deltaTime);
		}
	}	

	if (m_frog != nullptr)
	{
		if (m_frog->getPosition().y - m_grid->getTileSize() > m_cutoffPoint)
		{
			checkCollisionToCars();
		}
		else if(m_frog->getPosition().y - m_grid->getTileSize()/2 > m_cutoffPoint2)
		{
			checkCollisionToWater();
		}
		else
		{
			// Check collision to goal
			checkCollisionToGoal();
		}
	}

	if (m_startMenu != nullptr)
	{
		m_startMenu->update(deltaTime);
	}

	checkCollisionToDeleter();

	return true;
}

void Game::Draw(sf::RenderWindow& window)
{
	if (m_grid != nullptr)
	{
		m_grid->draw(window);
	}	

	for (auto& a : m_log)
	{
		if (a != nullptr)
		{
			window.draw(*a);
		}
	}

	if (m_frog != nullptr)
	{
		window.draw(*m_frog);
	}	

	for (auto& a : m_cars)
	{
		if (a != nullptr)
		{
			window.draw(*a);
		}
	}

	for (auto& a : m_goalFrog)
	{
		if (a != nullptr)
		{
			window.draw(*a);
		}
	}

	m_gui.draw(window);

	if(m_startMenu != nullptr)
		m_startMenu->draw(window);
}

void Game::onNotify(Entity* entity, Event event)
{
	switch (event)
	{
	case Event::VEHICLE_SPAWN_EVENT:
		m_cars.push_back(static_cast<Vehicle*>(entity));
		break;

	case Event::PLAYER_SPAWN_EVENT:
		if(m_frog == nullptr)
			m_frog = new Frog(m_grid->getPosByIndex(9, 13), m_grid->getTileSize(), m_screenSize.x - m_grid->getTileSize());

	case Event::MESSAGE_EVENT:
		std::cout << "Message recieved" << std::endl;
	default:
		break;
	}
}

void Game::onNotify(Event event)
{
	switch (event)
	{

	case Event::PLAYER_SPAWN_EVENT:
		if (m_frog == nullptr)
		{
			delete m_startMenu;
			m_startMenu = nullptr;
			m_frog = new Frog(m_grid->getPosByIndex(9, 13), m_grid->getTileSize(), m_screenSize.x - m_grid->getTileSize());
		}			
	
	default:
		std::cout << "Message recieved" << std::endl;
		break;
	}
}

bool Game::checkCollisionToCars()
{
	bool collided = false;

	if (m_frog != nullptr)
	{
		for (int i = 0; i < m_cars.size(); i++)
		{
			if (m_frog != nullptr && m_cars.at(i) != nullptr)
			{
				if (m_frog->getShapeBounds().intersects(m_cars.at(i)->getShapeBounds()))
				{
					damageFrog();
					if (m_squashSound.getStatus() != sf::Sound::Status::Playing)
					{
						m_squashSound.play();
					}
					else
					{
						m_squashSound.stop();
						m_squashSound.play();
					}
					collided = true;
				}
			}
		}
	}
	return collided;
}

bool Game::checkCollisionToWater()
{
	if (m_frog != nullptr)
	{
		if (m_frog->getShapeBounds().intersects(m_water.getBounds()))
		{
			bool onLog = false;
			for (int i = 0; i < sizeof(m_log) / sizeof(m_log[0]); i++)
			{
				if (m_frog->getShapeBounds().intersects(m_log[i]->getShapeBounds()))
				{
					m_frog->setHorizontalMove(m_log[i]->getMoveSpeed());
					onLog = true;
				}
			}
			if (!onLog)
			{
				damageFrog();
				if (m_splashSound.getStatus() != sf::Sound::Status::Playing)
				{
					m_splashSound.play();
				}
				else
				{
					m_splashSound.stop();
					m_splashSound.play();
				}
			}
		}
		else
		{
			m_frog->setHorizontalMove(0);
		}
	}
	return false;
}

void Game::checkCollisionToGoal()
{
	bool collided = false;
	for (auto& a : m_grassRect)
	{
		if (m_frog->getShapeBounds().intersects(a->getGlobalBounds()))
		{
			damageFrog();
			collided = true;
			break;
		}
	}
	for (auto& a : m_goalFrog)
	{
		if (m_frog != nullptr && a != nullptr)
		{
			if (m_frog->getShapeBounds().intersects(a->getShapeBounds()))
			{
				damageFrog();
				collided = true;
				break;
			}
		}		
	}

	if (!collided)
	{
		for (auto& a: m_waterRect)
		{
			if (m_frog->getShapeBounds().intersects(a->getGlobalBounds()))
			{
				frogInGoal(a->getPosition().x);
				break;
			}
		}
	}
}

void Game::checkCollisionToDeleter()
{
	for (int i = 0; i < m_cars.size() - 1; i++)
	{
		for (auto& a : m_deleter)
		{
			if (m_cars[i]->getShapeBounds().intersects(a.getGlobalBounds()))
			{
				delete m_cars[i];
				m_cars[i] = nullptr;
				m_cars.erase(m_cars.begin() + i);
			}
		}	
	}
}

void Game::damageFrog()
{
	m_frog->reset();
	m_frog->setPosition(m_grid->getPosByIndex(9, 13) + sf::Vector2f(m_grid->getTileSize() / 2, m_grid->getTileSize() / 2));
	m_frog->takeDamage();
	m_gui.removeLives();

	if (m_frog->getLives() < 1)
	{
		delete m_frog;
		m_frog = nullptr;
		restartGame();
	}
}

void Game::frogInGoal(float xPos)
{
	for (auto& a : m_goalFrog)
	{
		if (a == nullptr)
		{
			a = new Frog(sf::Vector2f(xPos, m_frog->getPosition().y - m_grid->getTileSize() / 2), m_frog->getSize(), 500);
			break;
		}
	}
	m_frog->setHorizontalMove(0);
	m_frog->setPosition(m_grid->getPosByIndex(9, 13) + sf::Vector2f(m_grid->getTileSize() /2, m_grid->getTileSize() / 2));

	

	m_goalCounter++;

	if (m_goalCounter == 6)
	{
		// Next Level
		nextLevel();
	}
}

void Game::nextLevel()
{
	m_levelCounter++;

	if (m_levelCounter < 6)
	{
		m_carSpawner[m_levelCounter % 5]->setPrototype(m_carTemplates[m_levelCounter % 5]);
	}

	for (int i = 0; i < sizeof(m_carSpawner) / sizeof(m_carSpawner[0]); i++)
	{
		m_carSpawner[i]->decreaseMinMax(1.2f);
	}

	for (auto& a : m_log)
	{
		a->increaseMoveSpeed(1.3f);
	}

	for (auto& a : m_goalFrog)
	{
		delete a;
		a = nullptr;
	}

	m_goalCounter = 0;
}

void Game::restartGame()
{
	if(m_startMenu == nullptr)
		m_startMenu = new StartMenu(static_cast<sf::Vector2f>(m_screenSize), this);

	m_levelCounter = 0;

	for (auto& a : m_carSpawner)
	{
		a->setSpawning(false);
	}

	int s = m_cars.size() - 1;
	for (int i = 0; i < s; i++)
	{
		delete m_cars[0];
		m_cars[0] = nullptr;
		m_cars.erase(m_cars.begin());
	}
	
	for (auto& a : m_carSpawner)
	{
		a->setSpawning(true);
	}

	for (auto& a : m_goalFrog)
	{
		delete a;
		a = nullptr;
	}

	m_goalCounter = 0;

	for (auto& a : m_carSpawner)
	{
		a->setPrototype(new Car(sf::Vector2f(0, 0), 25));
		a->resetSpawnRate();
	}

	for (auto& a : m_log)
	{
		a->resetSpeed();
	}

	m_gui.addLives(3);
}


