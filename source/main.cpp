#include "../include/RSGL/RSGL.hpp"

#ifdef __3DS__
    #define esc RK_Start
    #define name BOTTOM_SCREEN
    #include <map>
    #define WIDTH SCREEN_WIDTH
    #define LENGTH SCREEN_LENGTH
    #define X 0
    #define Y 0
    #define MouseButtonPressed RSGL::TouchscreenPressed
    #define MouseButtonReleased RSGL::TouchscreenReleased
    #define Expose true
#endif
#ifdef __linux__
    #define esc 0xff1b
    std::string name = "Connect 4";
    #define WIDTH 500
    #define LENGTH 500
    #define X 500
    #define Y 500
    #define MouseButtonPressed RSGL::MouseButtonPressed
    #define MouseButtonReleased RSGL::MouseButtonReleased
#endif
#define rows 6
#define collums 7
#define maxPoints 4

#include "../include/theory.hpp"
bool running = true, ai = false, won = false, home=true, nh=false;
std::vector<RSGL::circle> circles = {};
std::vector<RSGL::color> cirColors = {};
int player = 0, pressed=-1; std::string winner;

RSGL::window win(name,{X,Y,WIDTH,LENGTH},{0,0,215});

void checkEvents(){
    win.checkEvents();
    if (win.isPressed(esc)){win.clear(); home=true; nh=true;}
    switch(win.event.type){
        case RSGL::quit: running = false; break;
        case MouseButtonPressed:  for (int c=0; c < circles.size(); c++) if (RSGL::CircleCollidePoint(circles.at(c),{win.event.x,win.event.y})){  pressed=c; break;} break;
        case MouseButtonReleased: if (!won){
            for (int c=0; c < circles.size(); c++){
                if (pressed==c && cirColors.at(c).r+cirColors.at(c).g+cirColors.at(c).b == 255*3){
                    if (!player){ cirColors.at(c) = {255,0,0}; player=1; if(ai) player=3;}  
                    else if (player == 1){ cirColors.at(c) = {255,251,0}; player=0; }
                    if (player==3){cirColors.at(aiChoice(cirColors)) = {255,251,0}; player=0;} break;
                }
            }
        } default: break; 
     }
}


void checkGravityAndWins(){
    for (int c=0; c < circles.size(); c++){        
        if (cirColors.at(c).r+cirColors.at(c).b+cirColors.at(c).g != 255*3 && c+collums < circles.size() && cirColors.at(c+collums).r+cirColors.at(c+collums).b+cirColors.at(c+collums).g == 255*3){
            cirColors.at(c+collums) = cirColors.at(c); cirColors.at(c) = {255,255,255};
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
                    for (int i=1; i < maxPoints; i++){
                        int value= c +(collums*i)+(areas.at(a)*i);
                        if (value < circles.size() && cirColors.at(value).r + cirColors.at(value).g == col.at(color)) points++;
                        else break;
                    } if (points == maxPoints){ won=true; winner=color; break;}
                }
            }
        }
    }
}

void Home();

int main(int argc, char** argv){
    
    if (argc > 1 && (std::string)argv[1] == "-AI") ai=true;
    for (int y=0; y < rows; y++){ for (int x=0; x < collums; x++){ circles.insert(circles.end(),{((WIDTH+LENGTH)/15)*x+(WIDTH+LENGTH)/18,((WIDTH+LENGTH)/15)*y+(WIDTH+LENGTH)/18,(WIDTH+LENGTH)/25}); cirColors.insert(cirColors.end(),{255,255,255});}}
    //consoleInit(GFX_BOTTOM, NULL);
    while (running){
        if (!home){
            for (int i=0; i < circles.size(); i++) RSGL::drawCircle(circles.at(i),cirColors.at(i));
            checkEvents(); win.clear();
            if (!won) checkGravityAndWins();
            else RSGL::drawText(winner + " won", {50,LENGTH-50,25,25},"res/fonts/SansPosterBold.ttf",{255,255,255});
        } else Home();
    } win.close();
}

struct Button{
    std::string text; RSGL::rect r; RSGL::color tc,c; int size;
    void draw(){ RSGL::drawRect(r,c); RSGL::drawText(text,{r.x+(r.width/8),r.y+(r.length/5),size,size},"res/fonts/SansPosterBold.ttf",tc); }
    bool pressed=false;
    bool isClicked(){
        if(RSGL::RectCollidePoint(r,{win.event.x,win.event.y}) && pressed) return true;
        return false;
    } 
    void buttonLoop(){
        if (RSGL::RectCollidePoint(r,{win.event.x,win.event.y}) && win.event.type == MouseButtonPressed) pressed=true;
        else if (!RSGL::RectCollidePoint(r,{win.event.x,win.event.y}) && pressed) pressed=false;
    }
};

void Home(){
    win.checkEvents(); int click=0;
    Button a,b,c;
    a.text="AI"; a.r={15,(int)(LENGTH/1.3),(int)(LENGTH/11),(int)(WIDTH/9)}; a.tc={255,255,255}; a.c={0,0,0};a.size=25; 
    b.text="Normal"; b.r={100,(int)(LENGTH/1.3),(int)(LENGTH/11),(int)(WIDTH/3)}; b.tc={255,255,255}; b.c={0,0,0};b.size=25; 
    c.text="Online"; c.r={300,(int)(LENGTH/1.3),(int)(LENGTH/11),(int)(WIDTH/3)}; c.tc={255,255,255}; c.c={0,0,0};c.size=25; 
    
    std::vector<Button> buttons = {a,b,c};
        
    RSGL::drawText("Connect 4",{WIDTH/4,LENGTH/2+LENGTH/8,25,25},"res/fonts/SansPosterBold.ttf",{255,255,255});
    for (int i=0; i < buttons.size(); i++){
        if (win.event.type == Expose || nh) buttons.at(i).draw();
        buttons.at(i).buttonLoop();
        if (buttons.at(i).isClicked()) click=i+1;
    } switch(click){
         case 1: ai=true; home=false; break;
         case 2: ai=false; home=false; break;
         case 3: ai=false; home=false; break;
         default: break;
    } nh=false;  
    if (win.event.type == RSGL::quit || win.isPressed(esc)) running=false;
    RSGL::drawCircle({WIDTH/4,LENGTH/8,(WIDTH+LENGTH)/4},{255,251,0}); 
}