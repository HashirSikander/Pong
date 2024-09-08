#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <pthread.h>
using namespace std;

// insert paths here

#define backgroundPath "pic.png"
#define fontPath "Arial.ttf"
#define BallTimeDelay 10000


sf::RenderWindow window;
sf::Event event;
sf::CircleShape circle;
sf::RectangleShape rect1;
sf::RectangleShape rect2;
sf::Sprite img;
sf::Texture texture;
sf::Font font;
sf::Text Score1;
sf::Text Score2;



string maxScore = "10";


int rectspeed = 8;

float height = 720;
float width = 1280;
float radius = 12;

sf::Vector2f s(16, 150);
sf::Vector2f p1(2, height / 2 - s.y / 2);
sf::Vector2f p2(width - s.x - 2, height / 2 - s.y / 2);

float x = 1280 / 2;
float y = 720 / 2;

float vx = 7;
float vy = 7;

bool KeyStates[4];

bool Ongoing = 1;

void React()
{
    while (Ongoing)
    {
        if (KeyStates[0])
        {
            if (p2.y <= 0)
            {
                p2.y = 0;
            }
            else
            {
                p2.y -= rectspeed;
            }
        }
        else if (KeyStates[1])
        {
            if (p2.y + s.y >= height)
            {
                p2.y = height - s.y;
            }
            else
            {
                p2.y += rectspeed;
            }
        }
        if (KeyStates[2])
        {
            if (p1.y <= 0)
            {
                p1.y = 0;
            }
            else
            {
                p1.y -= rectspeed;
            }
        }
        else if (KeyStates[3])
        {
            if (p1.y + s.y >= height)
            {
                p1.y = height - s.y;
            }
            else
            {
                p1.y += rectspeed;
            }
        }
        usleep(10000); // Sleep for 10 milliseconds
    }
}

void *rectMoveThread(void *)
{
    React();
    return NULL;
}

void BallMove()
{
    while (Ongoing)
    {
        // Ball movement logic
        x += vx;
        y += vy;

        
        if ((y +radius < p1.y || y - radius > p1.y + s.y) )
        {
            if(x - radius < p1.x)
            {    
                Score1.setString(std::to_string(std::stoi(Score1.getString().toAnsiString()) + 1));
                Ongoing = Score1.getString() != maxScore;
                x = width / 2;
                y = height / 2;
                usleep(1500000);
                continue;
            }
        }
        else if(x-radius <= p1.x+s.x && x-radius >= p1.x) {vx = -vx;continue;} 
        if (y + radius < p2.y || y-radius > p2.y + s.y)
        {
            if(x + radius > p2.x+s.x)
            {
                Score2.setString(std::to_string(std::stoi(Score2.getString().toAnsiString()) + 1));
                Ongoing = Score2.getString() != maxScore;
                x = width / 2;
                y = height / 2;
                usleep(1500000);
                continue;
            }
        }
        else if(x+radius >= p2.x && x+radius <= p2.x+s.x) {vx = -vx;continue;}
        // if (x + radius >= width || x - radius <= 0)
        // {
        //     vx = -vx;
        // }
        if (y + radius >= height || y - radius <= 0)
        {
            vy = -vy;
        }


        usleep(BallTimeDelay);   
        
        
    }
}

void *BallMoveThread(void *)
{
    BallMove();
    return NULL;
}

int main()
{

    
    window.create(sf::VideoMode(width, height), "Pong");
    window.setFramerateLimit(60);
   

    font.loadFromFile(fontPath);
    Score1.setFont(font);
    Score1.setColor(sf::Color::Blue);
    Score1.setStyle(sf::Text::Bold);
    Score1.setCharacterSize(30);
    Score1.setString("0");


    Score2 = Score1;
    Score1.setPosition(1 + s.x, 1);
    Score2.setPosition(width - s.x - Score1.getCharacterSize(), 1);

    texture.loadFromFile(backgroundPath);
    img.setTexture(texture);
    img.setOrigin(img.getLocalBounds().width / 2, img.getLocalBounds().height / 2);
    img.setPosition(width / 2, height / 2);

    circle.setFillColor(sf::Color(0, 0, 230));
    circle.setOrigin(radius, radius);
    circle.setRadius(radius);
    circle.setPointCount(100);

    rect1.setFillColor(sf::Color(250, 0, 0));
    rect2.setFillColor(sf::Color(250, 0, 0));
    rect1.setSize(s);
    rect2.setSize(s);

    pthread_t rectThreadid, ballThreadid ;
    pthread_create(&rectThreadid, NULL, rectMoveThread, NULL);
    pthread_create(&ballThreadid, NULL, BallMoveThread, NULL);
    while (window.isOpen())
    {
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    KeyStates[0] = 1;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    KeyStates[1] = 1;
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    KeyStates[2] = 1;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    KeyStates[3] = 1;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    KeyStates[0] = 0;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    KeyStates[1] = 0;
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    KeyStates[2] = 0;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    KeyStates[3] = 0;
                }
            }
        }
        window.clear();
        
        
        circle.setPosition(x, y);
        rect1.setPosition(p1.x, p1.y);
        rect2.setPosition(p2.x, p2.y);

        window.draw(img);
        window.draw(rect1);
        window.draw(rect2);
        window.draw(circle);

        window.draw(Score1);
        window.draw(Score2);

        window.display();
        if(!Ongoing)
            window.close();
    }

    return 0;
}
