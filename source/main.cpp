#include "../include/rsgl/RSGL.hpp"

#define esc 0xff1b
#define WIDTH 500
#define LENGTH 500
#define rows 6
#define collums 7

bool running = true;
std::vector<RSGL::circle> circles = {};
std::vector<RSGL::color> cirColors = {};
int player = 0; int pressed=-1;
bool won = false;
std::string winner;
std::string mode;

RSGL::window win("Connect 4",{500,500,WIDTH,LENGTH},{0,0,215});

void checkEvents(){
    win.checkEvents();
    if (win.isPressed(esc)) running = false;
    switch(win.event.type){
        case RSGL::quit: running = false; break;
        case RSGL::MouseButtonPressed:
                for (int c=0; c < circles.size(); c++) if (RSGL::CircleCollidePoint(circles.at(c),{win.event.x,win.event.y})){  pressed=c; break;} break;
        case RSGL::MouseButtonReleased:
            if (!won){
                for (int c=0; c < circles.size(); c++){
                    if (pressed==c && cirColors.at(c).r+cirColors.at(c).g+cirColors.at(c).b == 255*3){
                        if (!player){ cirColors.at(c) = {255,0,0}; player=1; break;}  
                        else{ cirColors.at(c) = {255,251,0}; player=0; break;}
                    }
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
    for (int y=0; y < rows; y++){ for (int x=0; x < collums; x++){ circles.insert(circles.end(),{((WIDTH+LENGTH)/15)*x+(WIDTH+LENGTH)/18,((WIDTH+LENGTH)/15)*y+(WIDTH+LENGTH)/18,(WIDTH+LENGTH)/25}); cirColors.insert(cirColors.end(),{255,255,255});}}
    
    while (running){
        win.clear();
        for (int i=0; i < circles.size(); i++) RSGL::drawCircle(circles.at(i),cirColors.at(i));
        checkEvents();
        if (!won) checkGravityAndWins();
    } win.close();
}