#include <iostream>
#include <fstream>
#include "src/ObjReader.cpp"
#include "src/Camera.cpp"
#include "src/Vector.cpp"
#include "src/Objetos/Esfera.cpp"
#include "src/Objetos/Plano.cpp"
#include "src/Objetos/Tabuleiro.cpp"
#include "src/Objetos/Malha.cpp"
#include "scenes/sceneObj.cpp"
#include "scenes/templateScene.cpp"
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>
const int WIDTH = 300; 
const int HEIGHT= 250;

const double scale_down = 0.6;
const double EPS = 0.0001;

double velocidade = 5;
double rtt_vel = PI / 8;
vetor vel_vector, rotate_vector;
std::set<sf::Keyboard::Key> activeKeys;
sf::Vector2i mousePos;

double time_correction = 0.01;
inline double delta_time(){ return time_correction; }
uint8_t minmax(double x){ return max(min((int)(x*255.0), 255), 0); }

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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))  velocidade += 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) velocidade -= 0.1;
}

void check_dist(Camera &cam){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))  cam.set_dist(cam.get_dist() - 0.1);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))  cam.set_dist(cam.get_dist() + 0.1);
}

int LUZID = 0;
void update_pos_luz(std::vector<Luz>& luzes, Camera& cam, sf::RenderWindow &window){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) LUZID = (LUZID+1)%luzes.size();
    auto &luz = luzes[LUZID];
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
    luz.pos = luz.pos + vel_vector;
    cam.rotatey(rotate_vector.getZ() * -1.0);
    cam.rotatew(rotate_vector.getY() * -1.0);
    sf::Mouse::setPosition( sf::Vector2i(window.getSize().x/2, window.getSize().y / 2), window);
}

int IDBEZIER = 0;
bool SHOWMETA = true;
double BEZIERT = 0.5;
void update_bezier(std::vector<point>& pts, Camera& cam, sf::RenderWindow &window){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) IDBEZIER = (IDBEZIER+1)%pts.size();
    auto &pt = pts[IDBEZIER];
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
    pt = pt + vel_vector;
    cam.rotatey(rotate_vector.getZ() * -1.0);
    cam.rotatew(rotate_vector.getY() * -1.0);
    sf::Mouse::setPosition( sf::Vector2i(window.getSize().x/2, window.getSize().y / 2), window);
}


int TOTMODOS = 3; //4;
int modo = 0;


void handle_events(sf::RenderWindow &window){
    while(const std::optional event = window.pollEvent()) // Close window: exit
    {
        if(event->is<sf::Event::Closed>()) window.close();
        else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if(keyPressed->code == sf::Keyboard::Key::Escape){
                modo = (modo+1) % TOTMODOS;
                window.setMouseCursorVisible(modo==1);
            }
        }
        else if(const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        { 
            
        }
    }
}


double KA=1, KD=1, KS=1, NS=1;
Color RED(1, 0, 0), GREEN(0, 1, 0), BLUE(0, 0, 1);
void check_colors(objeto* obj){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) KA += 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) KA -= 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) KD += 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) KD -= 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) KS += 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) KS -= 0.1;
    obj->setkd(RED*KD);
    obj->setka(BLUE*KA);
    obj->setks(GREEN*KS);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) NS += 0.1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) NS -= 0.1;
    obj->setns(NS);
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) NS -= 0.1;
}

int main(){
    cerr << fixed << setprecision(2);
 
    auto cena = get_scene_classic(HEIGHT, WIDTH);
    auto &cam = cena.cam;
    auto &scene_objs = cena.objetos;

    // auto Bez = new Bline(std::vector<point>({point(0, 0, 0), point(0, 42, 45), point(0, 12, 35), point(0, -10, 20)}), Color(0.1, 0.9, 0.2), 0.75);
    // scene_objs.emplace_back(Bez);

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML window");
    // window.setFramerateLimit(60);
    sf::Image image(sf::Vector2u(WIDTH, HEIGHT), sf::Color::White);
    std::vector<vetor> img_colors;
    
    window.setMouseCursorVisible(modo==1);  

    clock_t t = clock();
    cena.shot(false);
    time_correction = (clock() - t) * 0.1;
    time_correction = 0.1;
    cerr << time_correction << "\n";

    while(window.isOpen()){
        handle_events(window);
        window.clear();

        time_correction = (clock() - t) * 0.1;
        check_velocidade();
        // check_dist(cam);
        check_colors(scene_objs.back());

        if(modo==0) update_pos(cam, window);
        else 
        if(modo==1)
        {
            const double sla = 0.05;
            // cam.rotatey(sla * velocidade);
            // cam.set_position( point() + (cam.get_pos() - point()).rotatey(sla * velocidade) );
        }
        else
        if(modo == 2) update_pos_luz(cena.luzes, cam, window);
        // else 
        // if(modo == 3) update_bezier(Bez->pts, cam, window);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) BEZIERT -= 0.05;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) BEZIERT += 0.05;
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) Bez->showMeta = SHOWMETA = !SHOWMETA;
        // Bez->metat = BEZIERT;

        t = clock();
        mousePos = sf::Mouse::getPosition(window);

        img_colors = cena.shot(false);

        for(int x = 0; x < WIDTH; x++) {
            for(int y = 0; y < HEIGHT; y++) {
                sf::Color color(
                    minmax(img_colors[x + y*WIDTH].getX()),
                    minmax(img_colors[x + y*WIDTH].getY()),
                    minmax(img_colors[x + y*WIDTH].getZ())
                );
                image.setPixel(sf::Vector2u(x, y), color);
            }
        }

        sf::Texture texture(image);
        window.draw(sf::Sprite(texture));

        int curr_fps = 1.0/(((double)time_correction*10.0)/CLOCKS_PER_SEC);
        auto pos = cam.get_pos();
        cerr << "FPS: " << curr_fps << "\t| velocidade: " << velocidade << "\t| cam_dist: " << cam.get_dist() << "\t | time_correction: " << time_correction << "\t | " << pos.getX() << "x " << pos.getY() << "y " << pos.getZ() << "z" << "\t\t\t\r";

        window.display();
    }
    cerr << "\n";

    return 0;
}