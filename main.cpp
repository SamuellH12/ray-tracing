#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include "src/Objetos/Cilindro.cpp"
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>
#define WIDTH  (640) //(640 / 10)
#define HEIGHT (360) //(360 / 10) 

const double PI = acos(-1);

std::vector<objeto*> get_scene(){
    std::vector<objeto*> objs;
    // objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new esfera(point(700, 0, 1700), 500, vetor(1, 0, 0)));
    objs.emplace_back(new tabuleiro(point(0, -75, 0), vetor(0, 1, 0), vetor(100, 0, 0), vetor(0.3, 0.8, 0.4), vetor(1, 1, 1)));
    
    objs.emplace_back(new cilindro(point(1500, 50, 1200), vetor(5, 10, 5) * 100.0, 80, Color(0.8, 0.7, 0.2)));

    //cubo
    point cpos(1500, 0, 300);
    double csz = 500;
    double cr = 50;
    Color ccl (0.8, 0.3, 0.6);
    vetor nxt(csz, 0, 0);
    bool preenchido = false;

    for(int j=0; j<2; j++){
        for(int i=0; i<4; i++){
            objs.emplace_back(new cilindro(cpos, cpos+nxt, cr, ccl, preenchido));
            cpos = cpos+nxt;
            nxt = nxt.rotatey(PI/2.0);
            if(!j) objs.emplace_back(new cilindro(cpos, cpos + vetor(0, csz, 0) , cr, ccl, preenchido));
        }

        cpos = cpos + vetor(0, csz, 0);
    }

    for(int i=0; i<3; i+=1)
        for(int j=0; j<3; j+=2)
            for(int k=0; k<3; k+=1)
                objs.emplace_back(new esfera(point(i*300 - 300, j*300, k*300 - 300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));

    objs.emplace_back(new esfera(point(800, 20, 0), 75, vetor(0.2, 0.6, 0.9))); 

    return objs;
};

const double scale_down = 0.6;
const double EPS = 0.0001;

double velocidade = 45;
double rtt_vel = PI / 8;
vetor vel_vector, rotate_vector;
std::set<sf::Keyboard::Key> activeKeys;
sf::Vector2i mousePos;

double time_correction = 0.01;
inline double delta_time(){ return time_correction; }

void update_pos(Camera &cam, sf::RenderWindow &window){
    vetor x = cam.get_v(); x = vetor(x.getX(), 0, x.getZ()).normalized();
    vetor y(0, 1, 0);
    vetor z = cam.get_w(); z = vetor(z.getX(), 0, z.getZ()).normalized();
    double dlt_tm = delta_time();

    vetor delta(0, 0, 0); //get movement from imput

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))   delta = delta - (x * dlt_tm); else 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) delta = delta + (x * dlt_tm);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))  delta = delta - (z * dlt_tm); else 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) delta = delta + (z * dlt_tm);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))  delta = delta + (y * dlt_tm); else 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) delta = delta - (y * dlt_tm);

    
    auto mouseAt = sf::Mouse::getPosition(window);
    vetor dltrotate(0, mousePos.y - mouseAt.y, mousePos.x - mouseAt.x); // get rotate from mouse
    dltrotate = dltrotate * rtt_vel * (1.0 / window.getSize().y);


    //update values and check limits
    vel_vector = vel_vector*scale_down/dlt_tm + delta*velocidade;    // scale_down old velocity and apply the new
    if(vel_vector.norm2() < EPS) vel_vector = vetor(0, 0, 0);   //if it is almost 0 it is 0
    if(vel_vector.norm2() > velocidade*velocidade) vel_vector = vel_vector.normalized() * velocidade;   //dont let velocity go to infinity
    
    rotate_vector = rotate_vector*scale_down/dlt_tm + dltrotate*rtt_vel;
    if(rotate_vector.norm2() < EPS) rotate_vector = vetor();
    if(rotate_vector.norm2() > rtt_vel*rtt_vel) rotate_vector = rotate_vector.normalized() * rtt_vel;

    //apply movement
    cam.move(vel_vector);
    cam.rotatey(rotate_vector.getZ() * -1.0);
    cam.rotatew(rotate_vector.getY() * -1.0);
    sf::Mouse::setPosition( sf::Vector2i(window.getSize().x/2, window.getSize().y / 2), window);
}

void check_velocidade(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))  velocidade += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) velocidade -= 1;
}

void check_dist(Camera &cam){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))  cam.set_dist(cam.get_dist() - 0.1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))  cam.set_dist(cam.get_dist() + 0.1);
}


bool controlar = true;

void handle_events(sf::RenderWindow &window){
    while(const std::optional event = window.pollEvent()) // Close window: exit
    {
        if(event->is<sf::Event::Closed>()) window.close();
        else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if(keyPressed->code == sf::Keyboard::Key::Escape){
                controlar = !controlar;
                window.setMouseCursorVisible(!controlar);
            }
        }
        else if(const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        { 
            
        }
    }
}

bool isgoingup = true;
void update_moving(std::vector<objeto*> &objs){

    objs.back()->set_pos(objs.back()->get_pos() + vetor(0, (isgoingup? 1 : -1), 0)*30.0*time_correction);
    if(objs.back()->get_pos().getY() >= 500.0) isgoingup = false;
    if(objs.back()->get_pos().getY() <= 0.0) isgoingup = true;

    objs.back()->set_pos( point() + (objs.back()->get_pos() - point()).rotatey(0.1) );
}

int main(){
    cerr << fixed << setprecision(2);
    point pos (1500, 500, -1000);
    point mira = point();//  + vetor(10, 0, 0); 
    vetor up (0, 1, 0);
    Camera cam(pos, mira, up, 0.75, WIDTH, HEIGHT);
 
    auto scene_objs = get_scene();
    
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML window");
    // window.setFramerateLimit(60);
    sf::Image image(sf::Vector2u(WIDTH, HEIGHT), sf::Color::White);
    std::vector<vetor> img_colors;
    window.setMouseCursorVisible(!controlar);  

    clock_t t = clock();
    cam.shot(scene_objs);
    time_correction = (clock() - t) * 0.1;
    cerr << time_correction << "\n";



    while(window.isOpen()){
        handle_events(window);
        window.clear();

        time_correction = (clock() - t) * 0.1;
        check_velocidade();
        check_dist(cam);
        update_moving(scene_objs);

        if(controlar) update_pos(cam, window);
        else 
        {
            cam.rotatey(0.005 * velocidade);
            cam.set_position( point() + (cam.get_pos() - point()).rotatey(0.005 * velocidade) );
        }

        t = clock();
        mousePos = sf::Mouse::getPosition(window);

        img_colors = cam.shot(scene_objs);

        for(int x = 0; x < WIDTH; x++) {
            for(int y = 0; y < HEIGHT; y++) {
                sf::Color color(
                    (int)(img_colors[x + y*WIDTH].getX()*255),
                    (int)(img_colors[x + y*WIDTH].getY()*255),
                    (int)(img_colors[x + y*WIDTH].getZ()*255)
                );
                image.setPixel(sf::Vector2u(x, y), color);
            }
        }

        sf::Texture texture(image);
        window.draw(sf::Sprite(texture));

        int curr_fps = 1.0/(((double)time_correction*10.0)/CLOCKS_PER_SEC);
        pos = cam.get_pos();
        cerr << "FPS: " << curr_fps << "\t| velocidade: " << velocidade << "\t| cam_dist: " << cam.get_dist() << "\t | time_correction: " << time_correction << "\t | " << pos.getX() << "x " << pos.getY() << "y " << pos.getZ() << "z\t\t\t\r";

        window.display();
    }
    cerr << "\n";

    return 0;
}

/*** OBJETOS DE TESTE
    objs.emplace_back(new esfera(point(10000, -2000, -8000), 1500, vetor(1, 1, 0)));
    objs.emplace_back(new esfera(mira, 50, vetor(1, 0, 0)));
    objs.emplace_back(new esfera(point(1000, 0, 2000), 500, vetor(1, 0, 0)));
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                objs.emplace_back(new esfera(point(i*300, j*300, k*300), 75 , vetor(0.5*i, 0.5*j, 0.5*k)));
*/