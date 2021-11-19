#define __3DS__ 1
#include "../include/RSGL/RSGL.hpp"
#include <vector>

bool running = true;
std::vector<RSGL::circle> circles = {};
std::vector<RSGL::color> cirColors = {};
int player = 0;
bool won = false;
std::string winner;
std::string mode;

RSGL::window win(BOTTOM_SCREEN, "Connect 4", 0, 0, 320, 240, {0,0,215});

void checkEvents(){
    win.checkEvents();
    if (RSGL::isClicked(RK_Start)) running = false;

    if (RSGL::isClicked(RK_Touch)) {
            if (!won){
                for (int c=0; c < circles.size(); c++){
                    bool click = RSGL::CircleCollidePoint(circles.at(c),{win.touch_x, win.touch_y});
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
            circles.insert(circles.end(),{40*(x+1),35*(y+1),20});
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
        
        for (std::string color="red"; !won; color = "yellow"){
            int points=0;
            for (int a=0; a < areas.size(); a++){
                points=0;
                for (int i=0; i < 4; i++){
                    int value;
                    if ((c +7*(i+areas.at(a))) < circles.size()){ 
                        value= cirColors.at((c +7*(i+areas.at(a)))).r + cirColors.at((c +7*(i+areas.at(a)))).g;
                        if (i==0) value=cirColors.at(c).r+cirColors.at(c).g;
                        if (color=="red" && value == 255 || color=="yellow" && value == 506){
                            points++; //std::cout << ((c +7*(i+areas.at(a)))) << std::endl; - completely makes the performance bad
                        }
                    }
                }
            }
            if (points==4){ winner=color; won=true;}
            else if (color == "yellow") break;
        }
    }
}


int main() {
	init();
    while (running){
		checkEvents();
		win.clear();
        for (int i=0; i < circles.size(); i++) RSGL::drawCircle(circles.at(i),cirColors.at(i));
        if (!won) checkGravityAndWins(); 
		C3D_FrameEnd(0); //should be removed later
    } win.close();
}