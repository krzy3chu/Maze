#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

sf::VideoMode desktop = sf::VideoMode().getDesktopMode();

class Background: public sf::Sprite{
public:
    sf::Texture texture;
    Background(){
        setPosition(0, 0);
        scale(0.8, 0.8);
        texture.loadFromFile("res/grass.png");
        texture.setRepeated(true);
        setTextureRect(sf::IntRect(0, 0, desktop.width / getScale().x, desktop.height / getScale().y));
        setTexture(texture);
    }
};

class Wall: public sf::Sprite{
public:
    sf::Texture texture;
    Wall(const int &x, const int &y, const int &w, const int &h){
        setPosition(x, y);
        scale(0.8, 0.8);
        texture.loadFromFile("res/wall.png");
        texture.setRepeated(true);
        setTextureRect(sf::IntRect(0, 0, w / getScale().x, h / getScale().y));
        setTexture(texture);
    }
};


class Coin: public sf::Sprite{
public:
    std::vector<sf::IntRect> frames;
    sf::Texture texture;
    int FPS = 7; //FPS of animation
    size_t actualFrame;
    double timeCounter;
    Coin(){
        setPosition(1810, 940);
        setOrigin(325/2, 325/2);
        scale(0.2, 0.2);
        texture.loadFromFile("res/coin.png");
        for(int i = 0; i < 6; i++){ //load all animation frames
            frames.emplace_back(sf::IntRect(i*325, 0, 325, 325));
        }
        actualFrame = 0;
        setTextureRect(sf::IntRect(frames[actualFrame]));
        setTexture(texture);
    }
    void animate(const sf::Time &elapsed){ //animate coin
        timeCounter += elapsed.asMicroseconds() / 1.e6;
        if(timeCounter >= (double)1/FPS){
            if(actualFrame == 5){
                actualFrame = 0;
            }else{
                actualFrame++;
            }
            timeCounter = 0;
        }
        setTextureRect(frames[actualFrame]);
    }
};


class Guy: public sf::Sprite{
public:
    std::vector<sf::IntRect> frames;
    sf::Texture texture;
    int guySpeed, speedX, speedY;
    int FPS = 7; //FPS of animation
    size_t actualFrame;
    double timeCounter;
    Guy(const int &x, const int &y){
        setPosition(x, y);
        scale(3, 3);
        texture.loadFromFile("res/guy.png");
        for(int i = 0; i <= 700; i += 50){ //load all animation frames
            frames.emplace_back(sf::IntRect(i+14, 7, 22, 29));
        }
        actualFrame = 0;
        setTextureRect(sf::IntRect(frames[actualFrame]));
        setTexture(texture);
        guySpeed = 120;
    }
    void moveGuy(const char& direction){
        if(direction == 'W'){
            speedY = -guySpeed;
        }else if(direction == 'S'){
            speedY = guySpeed;
        }else if(direction == 'I'){
            speedY = 0;
        }
        if(direction == 'A'){
            speedX = -guySpeed;
        }else if(direction == 'D'){
            speedX = guySpeed;
        }else if(direction == 'O'){
            speedX = 0;
        }
    }
    void animate(const sf::Time &elapsed){
        sf::FloatRect bounds = getGlobalBounds();
        if((bounds.left <= 0 && speedX < 0) || (bounds.left + bounds.width >= desktop.width && speedX > 0)){ //check if guy hit screen border
            speedX = 0;
        }
        if((bounds.top <= 0 && speedY < 0) || (bounds.top + bounds.height >= desktop.height && speedY > 0)){
            speedY = 0;
        }
        move(speedX * elapsed.asMicroseconds() / 1.e6, speedY * elapsed.asMicroseconds() / 1.e6);
        timeCounter += elapsed.asMicroseconds() / 1.e6; //guy animation
        if(timeCounter >= (double)1/FPS){
            if(speedX == 0 && speedY == 0){
                if(actualFrame >= 2){
                    actualFrame = 0;
                }else{
                    actualFrame++;
                }
            }else{
                if(speedX >= 0){
                    if(actualFrame >= 7 || actualFrame < 4){
                        actualFrame = 4;
                    }else{
                        actualFrame++;
                    }
                }else{
                    if(actualFrame >= 13 || actualFrame < 9){
                        actualFrame = 9;
                    }else{
                        actualFrame++;
                    }
                }
            }
            timeCounter = 0;
        }
        setTextureRect(frames[actualFrame]);
    }
    void collision(const sf::Sprite &sprite){ //check if guy hit a wall
        sf::FloatRect guy = getGlobalBounds();
        sf::FloatRect wall = sprite.getGlobalBounds();
        if(((guy.left + 1 < wall.left + wall.width && guy.left > wall.left) || (guy.left + guy.width - 1 > wall.left && guy.left < wall.left))){ //if guy x coordinate is within wall with 1px margin
            if((guy.top < wall.top + wall.height && guy.top + guy.height > wall.top + wall.height) && speedY < 0){ //and if top of guy is within wall
                speedY = 0; //hit top
            }
            if((guy.top + guy.height > wall.top && guy.top < wall.top) && speedY > 0){ //and if bottom of guy is within wall
                speedY = 0; //hit down
            }
        }
        if((guy.top + 1 < wall.top + wall.height && guy.top > wall.top) || (guy.top + guy.height - 1> wall.top && guy.top < wall.top)){ //if guy y coordinate is within wall with 1px margin
            if((guy.left < wall.left + wall.width && guy.left + guy.width > wall.left + wall.width) && speedX < 0){ //and if left side of guy is within wall
                speedX = 0; //hit left
            }
            if((guy.left + guy.width > wall.left && guy.left < wall.left) && speedX > 0){ //and if right side of guy is within wall
                speedX = 0; //hit right
            }
        }
    }
    bool getCoin(const sf::Sprite &sprite){
        sf::FloatRect guy = getGlobalBounds();
        sf::FloatRect coin = sprite.getGlobalBounds();
        if( ( (guy.top < coin.top + coin.height && guy.top > coin.top) ||
                        (guy.top + guy.height > coin.top && guy.top < coin.top) ) &&
                                    ( (guy.left < coin.left + coin.width && guy.left > coin.left) ||
                                                (guy.left + guy.width > coin.left && guy.left < coin.left) ) ){
            return true;
        }else{
            return false;
        }
    }
};
