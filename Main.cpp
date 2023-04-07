#include <SFML/Graphics.hpp>
#include <iostream>

enum Side {left, right};

class BouncyBall
{
private:
    sf::Vector2f position;
    sf::Vector2f acceleration = sf::Vector2f(0.4, 0.5);
    sf::Vector2f maxVelocity = sf::Vector2f(1000, 1000);
    sf::Vector2f velocity = sf::Vector2f(0, 0);
    sf::CircleShape circle;
    bool isJumping = true;
    float frictionStrength = 0.15;

public:
    BouncyBall(float pos_x, float pos_y, float radius)
    {
        position.x = pos_x;
        position.y = pos_y;

        circle.setRadius(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(position);
        circle.setFillColor(sf::Color::White);
    }
    void moveSideways(Side side)
    {
        if (isJumping)
        {
            return;
        }

        float xNewVelocity;
        switch (side)
        {
            case left:
                xNewVelocity = velocity.x - acceleration.x;
                break;
            case right:
                xNewVelocity = velocity.x + acceleration.x;
                break;
        }
        if (xNewVelocity > maxVelocity.x)
        {
            velocity.x = maxVelocity.x;
        }
        else if (xNewVelocity < maxVelocity.x * -1)
        {
            velocity.x = maxVelocity.x * -1;
        }
        else
        {
            velocity.x = xNewVelocity;
        }
    }
    void jump()
    {
        if (!isJumping)
        {
            velocity.y = -15;
            isJumping = true;
        }
    }
    void updatePhysics(uint32_t& windowWidth, uint32_t& windowHeight)
    {
        // Gravity
        float nextYVelocity = velocity.y + acceleration.y;
        float nextYPosition = position.y + velocity.y;

        // Collision (ground)
        if (nextYPosition < windowHeight - circle.getRadius())
        {
            velocity.y = nextYVelocity;
            position.y = nextYPosition;
        }
        else
        {
            isJumping = false;
            velocity.y = 0.f;
            position.y = windowHeight - circle.getRadius();
        }

        // Account for floating point imprecision
        if (velocity.x > -0.1 && velocity.x < 0.1)
        {
            velocity.x = 0;
        }

        // Friction
        if (!isJumping)
        {
            velocity.x *= 0.975;
        }

        // Collision (walls)
        float xNewPosition = position.x + velocity.x;
        if (xNewPosition - circle.getRadius() < 0)
        {
            position.x = circle.getRadius();
            velocity.x = 0;
        }
        else if (xNewPosition + circle.getRadius() > windowWidth)
        {
            position.x = windowWidth - circle.getRadius();
            velocity.x = 0;
        }
        else
        {
            position.x = xNewPosition;
        }

        circle.setPosition(position);
    }
    void mofifyXAcceleration(short change)
    {
        if (change > 0)
        {
            acceleration.x += 0.05;
        }
        else
        {
            acceleration.x -= 0.05;
        }
    }
    void render(sf::RenderWindow& window)
    {
        window.draw(circle);
    }
    sf::Vector2f getVelocity()
    {
        return velocity;
    }
    bool getIsJumping()
    {
        return isJumping;
    }
    sf::Vector2f getAcceleration()
    {
        return acceleration;
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
    uint32_t windowWidth = 1280;
    uint32_t windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bouncy Ball");
    window.setFramerateLimit(75);

    BouncyBall bouncyBall(windowWidth / 2, 60, 30);

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
                
                case sf::Event::MouseWheelMoved:
                    bouncyBall.mofifyXAcceleration(event.mouseWheel.delta);
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        
                        case sf::Keyboard::A:
                            bouncyBall.moveSideways(left);
                            break;
                        
                        case sf::Keyboard::D:
                            bouncyBall.moveSideways(right);
                            break;
                        
                        case sf::Keyboard::Space:
                            bouncyBall.jump();
                            break;
                    }

            }
        }

        xVelocitytext.setString("X velocity: " + std::to_string(bouncyBall.getVelocity().x));
        yVelocitytext.setString("Y velocity: " + std::to_string(bouncyBall.getVelocity().y));
        isJumpingText.setString("Is jumping: " + std::to_string(bouncyBall.getIsJumping()));
        xAccelerationText.setString("X acceleration " + std::to_string(bouncyBall.getAcceleration().x));
        yAccelerationText.setString("Y acceleration " + std::to_string(bouncyBall.getAcceleration().y));

        window.clear();

        bouncyBall.updatePhysics(windowWidth, windowHeight);
        bouncyBall.render(window);

        window.draw(xVelocitytext);
        window.draw(yVelocitytext);
        window.draw(isJumpingText);
        window.draw(xAccelerationText);
        window.draw(yAccelerationText);

        window.display();
    }

    return 0;
}