#include <SFML/Graphics.hpp>
#include <iostream>

enum Side {left, right};

class BouncyBall
{
private:
    sf::Vector2f position;
    sf::Vector2f acceleration = sf::Vector2f(0.5, 0.5);
    sf::Vector2f maxVelocity = sf::Vector2f(20, 20);
    sf::Vector2f velocity = sf::Vector2f(0, 0);
    sf::CircleShape circle;
    bool isJumping = true;

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
        switch (side)
        {
            case left:
                break;
            case right:
                break;
        }
    }
    void jump()
    {
        if (!isJumping)
        {
            velocity.y = -23;
            isJumping = true;
        }
    }
    void updatePhysics(uint32_t& windowWidth, uint32_t& windowHeight)
    {
        // Gravity
        float nextYVelocity = velocity.y + acceleration.y;
        float nextYPosition = position.y + velocity.y;
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

        circle.setPosition(position);
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) bouncyBall.moveSideways(left);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) bouncyBall.moveSideways(right);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) bouncyBall.jump();
        }

        xVelocitytext.setString("X velocity: " + std::to_string(bouncyBall.getVelocity().x));
        yVelocitytext.setString("Y velocity: " + std::to_string(bouncyBall.getVelocity().y));
        isJumpingText.setString("Is jumping: " + std::to_string(bouncyBall.getIsJumping()));

        window.clear();

        bouncyBall.updatePhysics(windowWidth, windowHeight);
        bouncyBall.render(window);

        window.draw(xVelocitytext);
        window.draw(yVelocitytext);
        window.draw(isJumpingText);

        window.display();
    }

    return 0;
}