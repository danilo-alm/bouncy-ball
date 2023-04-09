#include <SFML/Graphics.hpp>
#include <iostream>

enum Side { left, right };

class BouncyBall
{
private:
    sf::Vector2f m_Position, m_Velocity, m_Acceleration = {2, 35};
    sf::CircleShape ball;
    bool m_IsJumping = false, m_WillJump = false;
    float m_Radius;

public:
    BouncyBall(float pos_x, float pos_y, float radius)
        : m_Position(pos_x, pos_y), m_Radius(radius)
    {
        ball.setRadius(radius);
        ball.setOrigin(radius, radius);
        ball.setPosition(m_Position);
    }

    void HandleInput(sf::Keyboard::Key& keyCode)
    {
        switch (keyCode)
        {
            case sf::Keyboard::A:
                MoveSideways(left);
                break;
            case sf::Keyboard::D:
                MoveSideways(right);
                break;
            case sf::Keyboard::Space:
                Jump();
                break;
        }
    }

    void UpdatePhysics(const int& windowWidth, const int& windowHeight, sf::Time& dt)
    {
        // Gravity
        m_Velocity.y += m_Acceleration.y;
        float yNextPosition = m_Position.y + m_Velocity.y * dt.asSeconds();

        if (yNextPosition >= windowHeight - m_Radius)
        {
            m_IsJumping = false;
            m_Position.y = windowHeight - m_Radius;
        
            if (m_WillJump)
            {
                m_Velocity.y = -1500;
                m_WillJump = false;
                m_IsJumping = true;
            }
            else
            {
                m_Velocity.y = 0;
            }

            // Attrict
            m_Velocity.x *= .95f;
        }
        else
        {
            m_Position.y = yNextPosition;
            m_WillJump = false;
        }

        m_Position.x += m_Velocity.x;

        ball.setPosition(m_Position);
    }

    void MoveSideways(Side side)
    {
        if (m_IsJumping)
            return;
        switch (side)
        {
            case left:
                m_Velocity.x -= m_Acceleration.x;
                break;
            case right:
                m_Velocity.x += m_Acceleration.x;
                break;
        }
    }

    void Jump()
    {
        if (!m_IsJumping)
            m_WillJump = true;
    }

    void Render(sf::RenderWindow& window)
    {
        window.draw(ball);
    }

    inline bool GetIsJumping()
    {
        return m_IsJumping;
    }
};

int main()
{
    const int windowWidth = 1280;
    const int windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bouncy Ball");
    window.setFramerateLimit(75);

    BouncyBall bouncyBall(640, 200, 30);

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();
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

        window.clear();
        bouncyBall.UpdatePhysics(windowWidth, windowHeight, dt);
        bouncyBall.Render(window);
        window.display();
    }
}