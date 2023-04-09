#include <SFML/Graphics.hpp>
#include <iostream>

enum Side { left, right };

class BouncyBall
{
private:
    sf::Vector2f m_Position, m_Velocity, m_Acceleration = {100, 35};  // m_Acceleration.y = gravity
    sf::CircleShape ball;
    bool m_IsJumping = true, m_WillJump = false;
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

    void UpdatePhysics(const uint32_t& windowWidth, const uint32_t& windowHeight, sf::Time& dt)
    {
        // Gravity
        float yNextVelocity = m_Velocity.y + m_Acceleration.y;
        float yNextPosition = m_Position.y + yNextVelocity * dt.asSeconds();

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
                // Bounce
                if (m_Velocity.y >= 170)
                {
                    m_Velocity.y = (m_Velocity.y - 170) * -1;
                    m_IsJumping = true;
                }
                else
                    m_Velocity.y = 0;
                    
            }

            // Friction
            m_Velocity.x *= .9f;
        }
        else
        {
            m_Position.y = yNextPosition;
            m_Velocity.y = yNextVelocity;
            m_WillJump = false;
        }

        // Walls
        if (m_Position.x > windowWidth - m_Radius || m_Position.x < m_Radius)
            m_Velocity.x *= -1;

        m_Position.x += m_Velocity.x * dt.asSeconds();

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

    inline sf::Vector2f GetAcceleration()
    {
        return m_Acceleration;
    }

    inline sf::Vector2f GetVelocity()
    {
        return m_Velocity;
    }
};

void setTextUp(sf::Text& text, sf::Font& font, float pos_x, float pos_y)
{
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(pos_x, pos_y);
}

int main()
{
    const u_int32_t windowWidth = 1280;
    const u_int32_t windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bouncy Ball");
    window.setFramerateLimit(75);

    //BouncyBall bouncyBall(640, 200, 30);
    BouncyBall bouncyBall(640, 880, 30);

    sf::Font font;
    if (!font.loadFromFile("fonts/Ubuntu-Regular.ttf"))
    {
        return 1;
    }

    sf::Text xVelocitytext;
    setTextUp(xVelocitytext, font, 10, 10);

    sf::Text yVelocitytext;
    setTextUp(yVelocitytext, font, 10, 40);

    sf::Text isJumpingText;
    setTextUp(isJumpingText, font, 10, 70);

    sf::Text xAccelerationText;
    xAccelerationText.setString("Acceleration: 0.000000");  // template
    setTextUp(xAccelerationText, font, 0, 0);

    sf::Text yAccelerationText;
    setTextUp(yAccelerationText, font, 0, 0);

    sf::FloatRect textBounds = xAccelerationText.getLocalBounds();
    xAccelerationText.setPosition(window.getSize().x - textBounds.width - 25, 10);
    yAccelerationText.setPosition(window.getSize().x - textBounds.width - 25, 40);

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

        xVelocitytext.setString("X velocity: " + std::to_string(bouncyBall.GetVelocity().x));
        yVelocitytext.setString("Y velocity: " + std::to_string(bouncyBall.GetVelocity().y));
        isJumpingText.setString("Is jumping: " + std::to_string(bouncyBall.GetIsJumping()));
        xAccelerationText.setString("X acceleration " + std::to_string(bouncyBall.GetAcceleration().x));
        yAccelerationText.setString("Y acceleration " + std::to_string(bouncyBall.GetAcceleration().y));

        window.clear();
        bouncyBall.UpdatePhysics(windowWidth, windowHeight, dt);
        bouncyBall.Render(window);

        window.draw(xVelocitytext);
        window.draw(yVelocitytext);
        window.draw(isJumpingText);
        window.draw(xAccelerationText);
        window.draw(yAccelerationText);

        window.display();
    }
}