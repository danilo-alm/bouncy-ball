#include <SFML/Graphics.hpp>
#include <iostream>

class BouncyBall
{
private:
    sf::Vector2f m_Position, m_Acceleration, m_Velocity;
    sf::CircleShape ball;
    bool m_IsJumping, m_WillBounce;
public:
    BouncyBall(float pos_x, float pos_y, float radius)
        : m_Position(pos_x, pos_y)
    {
        m_IsJumping = false;
        m_WillBounce = false;
        
        ball.setRadius(radius);
        ball.setOrigin(radius, radius);
        ball.setPosition(m_Position);
    }

    void HandleInput(sf::Keyboard::Key keyCode)
    {
        switch (keyCode)
        {
            case sf::Keyboard::A:
                std::cout << "Pressed A" << std::endl;
                break;
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Bouncy Ball");
    window.setFramerateLimit(75);

    BouncyBall bouncyBall(0, 0, 20);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    else
                        bouncyBall.HandleInput(event.key.code);
                    break;
            }
        }
    }
}