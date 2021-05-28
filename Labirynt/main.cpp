#include "objects.cpp"
#include <SFML/Window.hpp>
#include <fstream>

int main(){
    sf::RenderWindow window(desktop, "Window name", sf::Style::None);

    Background background;
    std::vector<Wall*> walls;
    std::ifstream file("walls.txt");
    int x, y, w, h;
    while(!file.eof()){ //read walls positions and shapes from txt file
        file >> x;
        file >> y;
        file >> w;
        file >> h;
        Wall *wall = new Wall(x+10, y+40, w, h);
        walls.emplace_back(wall);
    }
    Guy guy(25, 75);
    Coin coin;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)){
                window.close();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            guy.moveGuy('W');
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            guy.moveGuy('S');
        }else{
            guy.moveGuy('I');
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            guy.moveGuy('A');
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            guy.moveGuy('D');
        }else{
            guy.moveGuy('O');
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            guy.guySpeed = 300;
        }else{
            guy.guySpeed = 120;
        }
        sf::Time elapsed = clock.restart();
        for(Wall *wall: walls){
            guy.collision(*wall); //check guy colission with wall
        }
        if(guy.getCoin(coin)){ //check if guy got coin
            if(coin.getPosition().x == 60){
                coin.setPosition(1810, 940);
            }else{
                coin.setPosition(60, 90);
            }
        }
        guy.animate(elapsed); //animations
        coin.animate(elapsed);

        window.clear(sf::Color::Black);
        window.draw(background);
        for(Wall *wall: walls){
            window.draw(*wall);
        }
        window.draw(guy);
        window.draw(coin);
        window.display();
    }
    return 0;
}
