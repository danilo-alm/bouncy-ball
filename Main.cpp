#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class BouncyBall
{
private:
    sf::Vector2f position;
    sf::Vector2f acceleration = sf::Vector2f(0.5, 0.5);
    sf::Vector2f maxVelocity = sf::Vector2f(20, 20);
    sf::Vector2f velocity = sf::Vector2f(0, 0);
    sf::CircleShape circle;

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
    void updatePhysics(uint32_t& windowWidth, uint32_t& windowHeight)
    {
        // Gravity
        float nextYVelocity = velocity.y + acceleration.y;
        float nextYPosition = position.y + velocity.y;
        if (position.y < windowHeight - circle.getRadius() && nextYPosition < windowHeight - circle.getRadius())
        {
            velocity.y = nextYVelocity;
            position.y = nextYPosition;
        }
        else
        {
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
};

int main()
{
    uint32_t windowWidth = 1280;
    uint32_t windowHeight = 720;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bouncy Ball");
    window.setFramerateLimit(60);

    BouncyBall bouncyBall(windowWidth / 2, 60, 30);

    sf::Font font;
    std::string file = "fonts/Ubuntu-Regular.ttf";
    if (!font.loadFromFile(file))
    {
        std::cout << "Could not load " << file << std::endl;
        return 1;
    }

    sf::Text xVelocitytext;
    xVelocitytext.setFont(font);
    xVelocitytext.setCharacterSize(24);
    xVelocitytext.setFillColor(sf::Color::White);
    xVelocitytext.setPosition(10, 10);

    sf::Text yVelocitytext;
    yVelocitytext.setFont(font);
    yVelocitytext.setCharacterSize(24);
    yVelocitytext.setFillColor(sf::Color::White);
    yVelocitytext.setPosition(10, 40);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        xVelocitytext.setString("X velocity: " + std::to_string(bouncyBall.getVelocity().x));
        yVelocitytext.setString("Y velocity: " + std::to_string(bouncyBall.getVelocity().y));

        window.clear();

        bouncyBall.updatePhysics(windowWidth, windowHeight);
        bouncyBall.render(window);

        window.draw(xVelocitytext);
        window.draw(yVelocitytext);

        window.display();
    }

    return 0;
}