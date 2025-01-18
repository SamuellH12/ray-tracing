#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include <vector>

#include <SFML/Graphics.hpp>

#define WIDTH (640 / 5)
#define HEIGHT (360 / 5) 

std::vector<objeto*> get_scene(){
    std::vector<objeto*> objs;
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    // objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(1000, 0, 2000), 500, vetor(1, 0, 0)));
    objs.emplace_back(new tabuleiro(point(0, -75, 0), vetor(0, 1, 0), vetor(100, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));


    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));

    return objs;
};

clock_t lst_shot;

double delta_time(){
    double dlt = clock() - lst_shot;
    return dlt;
}

vetor get_delta_pos(){
    vetor delta(0, 0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) delta = delta + (vetor(0, 0, 1) * delta_time());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) delta = delta - (vetor(0, 0, 1) * delta_time());
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) delta = delta + (vetor(0, 1, 0) * delta_time());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) delta = delta - (vetor(0, 1, 0) * delta_time());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) delta = delta + (vetor(1, 0, 0) * delta_time());
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) delta = delta - (vetor(1, 0, 0) * delta_time());
    
    return delta;
}

double velocidade = 10;

void check_velocidade(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal)) velocidade += 5;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) velocidade -= 5;
}

int main(){
    point pos (0, 0, -50);
    point mira = pos + vetor(10, 0, 0); 
    vetor up (0, 1, 0);
 
    auto scene_objs = get_scene();
    
    std::vector<vetor> img_colors;

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML window");
    // window.setFramerateLimit(60);
    sf::Image image(sf::Vector2u(WIDTH, HEIGHT), sf::Color::White);


    while(window.isOpen()){
        while (const std::optional event = window.pollEvent()){ // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        clock_t t = clock();
        check_velocidade();
        vetor dlt = get_delta_pos();
        pos = pos + dlt * velocidade;
        mira= mira+ dlt * velocidade;

        window.clear();

        img_colors = Camera(pos, mira, up, 0.75, WIDTH, HEIGHT).shot(scene_objs);

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                sf::Color color(
                    (int)(img_colors[x + y*WIDTH].getX()*255),
                    (int)(img_colors[x + y*WIDTH].getY()*255),
                    (int)(img_colors[x + y*WIDTH].getZ()*255)
                );
                image.setPixel(sf::Vector2u(x, y), color);
            }
        }

        sf::Texture texture(image);
        sf::Sprite sprite(texture);

        window.draw(sprite);

        t = clock() - t;
        int curr_fps = 1.0/((double)t/CLOCKS_PER_SEC);
        cerr << "FPS: " << curr_fps << " | " << pos.getX() << " " << pos.getY() << " " << pos.getZ() << "\r";

        window.display();
        lst_shot = clock();
    }

    return 0;
}