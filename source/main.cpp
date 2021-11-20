#include "../include/rsgl/RSGL.hpp"

#define esc 0xff1b

bool running = true;
std::vector<RSGL::circle> circles = {};
std::vector<RSGL::color> cirColors = {};
int player = 0;
bool won = false;
std::string winner;
std::string mode;

RSGL::window win("Connect 4",{500,500,350,400},{0,0,215});

void checkEvents(){
    win.checkEvents();
    if (win.isPressed(esc)) running = false;

    switch(win.event.type){
        case RSGL::quit: running = false; break;
        case RSGL::MouseButtonPressed:
            if (!won){
                for (int c=0; c < circles.size(); c++){
                    bool click = RSGL::CircleCollidePoint(circles.at(c),{win.event.x,win.event.y});
                    if (click && cirColors.at(c).r+cirColors.at(c).g+cirColors.at(c).b == 255*3){
                        if (!player){
                            cirColors.at(c) = {255,0,0};
                            player=1;
                        } else{
                            cirColors.at(c) = {255,251,0};
                            player=0;
                        }
                    }
                }
            }
     }
}

void init(){
    for (int y=0; y < 6; y++){
        for (int x=0; x < 7; x++){
            circles.insert(circles.end(),{50*(x+1),50*(y+1),25});
            cirColors.insert(cirColors.end(),{255,255,255});
        }
    }
}

void checkGravityAndWins(){
    for (int c=0; c < circles.size(); c++){       
        if (cirColors.at(c).r+cirColors.at(c).b+cirColors.at(c).g != 255*3 && c+7 < circles.size() && cirColors.at(c+7).r+cirColors.at(c+7).b+cirColors.at(c+7).g == 255*3){
            cirColors.at(c+7) = cirColors.at(c);
            cirColors.at(c) = {255,255,255};
        }
        std::vector<int> areas = {-1,1,0,-7}; int points=0;
        std::map<std::string,int> col = {{"red",255},{"yellow",506}}; int ind=0;
        for (std::string color="red"; !won && color!="yellow"; ind++){
            if (ind==1) color = "yellow";
            int points=0;
            for (int a=0; a < areas.size(); a++){
                points=0;
                if (cirColors.at(c).r+cirColors.at(c).g == col.at(color)){
                    points++;
                    for (int i=1; i < 5; i++){
                        int value= c +(7*i)+areas.at(a);
                        if (i == 4){ winner=color; won=true; break;}
                        if (value < circles.size() && cirColors.at(value).r + cirColors.at(value).g == col.at(color)) points++;
                        else break;
                    }
                }
            }
        }
    }
}

int main(){
    init();
    while (running){
        win.clear();
        for (int i=0; i < circles.size(); i++) RSGL::drawCircle(circles.at(i),cirColors.at(i));
        checkEvents();
        if (!won) checkGravityAndWins();
    } win.close();
}