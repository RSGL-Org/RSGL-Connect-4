#define __3DS__ 1
#include "../include/RSGL/RSGL.hpp"
#include <vector>
#include <map>

#define WIDTH 320
#define LENGTH 240
#define rows 6
#define collums 7

bool running = true;
std::vector<RSGL::circle> circles = {};
std::vector<RSGL::color> cirColors = {};
int player = 0; int pressed=-1;
bool won = false;
std::string winner;
std::string mode;

RSGL::window win(BOTTOM_SCREEN, "Connect 4", {0,0,WIDTH,LENGTH}, {0,0,215});

void checkEvents(){
    win.checkEvents();
    if (RSGL::isClicked(RK_Start)) running = false;
    if (RSGL::isClicked(RK_Touch)) {
        for (int c=0; c < circles.size(); c++) {if (RSGL::CircleCollidePoint(circles.at(c),{win.touch_x,win.touch_y})){  pressed=c; break;}}
    }
    if (RSGL::isReleased(RK_Touch)) {
        for (int c=0; c < circles.size(); c++){
            if (pressed==c && cirColors.at(c).r+cirColors.at(c).g+cirColors.at(c).b == 255*3){
                if (!player){ cirColors.at(c) = {255,0,0}; player=1;}  
                else{ cirColors.at(c) = {255,251,0}; player=0; }
                break;
            }
        }
    }
}

void checkGravityAndWins(){
    for (int c=0; c < circles.size(); c++){        
        if (cirColors.at(c).r+cirColors.at(c).b+cirColors.at(c).g != 255*3 && c+collums < circles.size() && cirColors.at(c+collums).r+cirColors.at(c+collums).b+cirColors.at(c+collums).g == 255*3){
            cirColors.at(c+collums) = cirColors.at(c);
            cirColors.at(c) = {255,255,255};
        }
        std::vector<int> areas = {-1,1,0,(-collums)+1}; int points=0;
        std::map<std::string,int> col = {{"red",255},{"yellow",506}}; int ind=0;
        for (std::string color="red"; !won && color!="yellow"; ind++){
            if (ind==1) color = "yellow";
            int points=0;
            for (int a=0; a < areas.size(); a++){
                points=0;
                if (cirColors.at(c).r+cirColors.at(c).g == col.at(color)){
                    points++;
                    for (int i=1; i < 4; i++){
                        int value= c +(collums*i)+areas.at(a);
                        //
                        if (value < circles.size() && cirColors.at(value).r + cirColors.at(value).g == col.at(color)){
                            if (color == "red") std::cout << a << std::endl; points++;
                        }
                        else break;
                    } if (points == 4){ winner=color; won=true; break;}
                }
            }
        }
    }
}


int main(){
    for (int y=0; y < rows; y++){ for (int x=0; x < collums; x++){ circles.insert(circles.end(),{40*(x+1),35*(y+1),20}); cirColors.insert(cirColors.end(),{255,255,255});}}
    RSGL::enableConsole(TOP_SCREEN);
    while (running){
        checkEvents();
        for (int i=0; i < circles.size(); i++) RSGL::drawCircle(circles.at(i),cirColors.at(i));
        if (!won) checkGravityAndWins(); else {RSGL::drawRect({0,0,50,50}, {0,255,0});}
        win.clear();
    } win.close();
}