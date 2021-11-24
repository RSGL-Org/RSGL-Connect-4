#pragma once
#include <string.h>
#include <iostream>
#include <ctime>   
#include <vector>

struct Time {
    int y, m, d;
    int h, min, s;
    std::string day;
};
typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

namespace RSGL{
    int Quit();
    int nextFrame();
    int enableRomfs();
    int clearConsole();
    std::string getLanguage();
    std::string getSystem();
    std::string getRegion();

    const int KeyPressed=2;
    const int KeyReleased=3;
    const int TouchscreenPressed=4;
    const int TouchscreenReleased=5;
    const int MousePosChanged=6;
    const int quit = 33;
    const int dnd = 34;

    struct rect{int x,y; int length, width;};
    struct circle { int x, y; int radius; };
    struct triangle { int x, y; int length, width; };
    struct image {int number; RSGL::rect r; Sprite* sprite;};
    struct color { int r, g, b; };
    struct point { int x, y;};
    struct Text{
      RSGL::rect rect; 
      RSGL::color c;
      std::string text;
      std::string f;
      Text(std::string txt, RSGL::rect r, const char* font, RSGL::color col, bool draw=true);
      Text(){}
      void draw();
    };

    int CircleCollidePoint(RSGL::circle c, RSGL::point p);
    int CircleCollideRect(RSGL::circle c, RSGL::rect r);
    int CircleCollide(RSGL::circle cir1,RSGL::circle cir2);
    int RectCollidePoint(RSGL::rect r, RSGL::point p);
    int RectCollideRect(RSGL::rect r, RSGL::rect r2);
    int PointCollide(RSGL::point p, RSGL::point p2);
    /*int ImageCollideRect(RSGL::image img, RSGL::rect r);
    int ImageCollideCircle(RSGL::image img, RSGL::circle c);
    int ImageCollidePoint(RSGL::image img, RSGL::point p);
    int ImageCollideImage(RSGL::image img, RSGL::image img2);*/

    u32 background_color;
    bool romfs = false;
    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_RIGHT);
    C3D_RenderTarget* screen;

    int drawRect(rect r, color col, bool solid=true) {
        u32 clr = C2D_Color32(col.r, col.g, col.b, 0xFF);
        C2D_DrawRectSolid(r.x, r.y, 0, r.width, r.length, clr);
        if (!solid) {
            C2D_DrawRectSolid(r.x+3, r.y+3, 0, r.width-6, r.length-6, background_color);
        }
        return 1;
    }
    int drawPoint(point p, color c){
        drawRect({p.x,p.y,1,1},c);
        return 1;
    }

    void drawText(std::string text, RSGL::rect r, const char* font, RSGL::color col){
    stbtt_fontinfo Font; bool cre=false; //char ttf_buffer;
    //for (int i=0; i < ttf_fontnames.size(); i++) if (ttf_fontnames.at(i) == font){cre=true; ttf_buffer=ttf_buffers.at(i); break;}
    //if (cre){ ttf_buffers.insert(ttf_buffers.end(), 0); ttf_buffer = ttf_buffers.at(ttf_buffers.size()-1);}
    char ttf_buffer[1<<25];

    std::vector<std::string> ttf_fontnames;
    std::vector <char> ttf_buffers; 
    fread(ttf_buffer, 1, 1<<25, fopen(font, "rb"));

    for (int l=0; l < text.size(); l++){
        unsigned char *bitmap;
        int w,h,i,j,c = text.at(l), s = r.width, b=0, jb=0;
        stbtt_InitFont(&Font, (const unsigned char*)ttf_buffer, stbtt_GetFontOffsetForIndex((const unsigned char*)ttf_buffer,0));
        bitmap = stbtt_GetCodepointBitmap(&Font, 0,stbtt_ScaleForPixelHeight(&Font, s), c, &w, &h, 0,0);
        if (text.at(l)=='a' || text.at(l)=='c' || text.at(l)=='e' || text.at(l)=='m' || text.at(l)=='n' 
          || text.at(l)=='o' || text.at(l)=='r' || text.at(l)=='s' || text.at(l)=='u' || text.at(l)=='v' || text.at(l)=='w') b=4;
        if (text.at(l)=='j') jb=-4;
        for (j=0; j < h; ++j) 
            for (i=0; i < w; ++i) if ( " .:ioVM@"[bitmap[j*w+i]>>5] != ' ') RSGL::drawPoint({i+r.x+(l*s)+jb,j+(r.y+b)},col);
    }      
}

    struct window {
        C3D_RenderTarget* screen;
    
        int x;
        int y;
        int w;
        int h;
        RSGL::color clr;
        struct Event {
            int x, y;           //touch X and Y
            int cpad_x, cpad_y; //C-pad X and Y
            int type;
        };
        Event event; 
        window(C3D_RenderTarget* screen, RSGL::rect r, RSGL::color c) {
            gfxInitDefault();
            cfguInit();
            C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	        C2D_Prepare();
            RSGL::background_color = C2D_Color32(c.r, c.g, c.b, 0xFF); window::clr = c; RSGL::screen = screen;  window::screen = screen; window::x = r.x; window::y = r.y; window::w = r.width; window::h = r.length;
        };
        void close() {
			C3D_Fini();
 			gfxExit();
 			cfguExit();
    		if (romfs) romfsExit();
            window::event.type=RSGL::quit;
        }
        void checkEvents();

        bool isPressed(u32 key) { if (hidKeysHeld() & key && key==KEY_TOUCH) window::event.type = RSGL::TouchscreenPressed; return hidKeysHeld() & key; }
        bool isClicked(u32 key) { return hidKeysDown() & key; }
        bool isReleased(u32 key) { if (hidKeysUp() & key && key==KEY_TOUCH) window::event.type = RSGL::TouchscreenReleased; return hidKeysUp() & key; }

        void clear() {
            /*if (RSGL::topScreen == RSGL::screen && ((window::w < 399) && (window::h < 239))) {
                // X resolution
                RSGL::drawRect({window::x,window::y,240, (200-(window::w/2))}, {0,0,0});
	            RSGL::drawRect({window::x+(window::w+(200-(window::w/2))),0,300,(200-(window::w/2))}, {0,0,0});
                // Y resolution
	            RSGL::drawRect({window::x,window::y,(120-(window::h/2))*2, 400}, {0,0,0});
            }

            if (RSGL::bottomScreen == RSGL::screen && ((window::w <= 319) && (window::h <= 239))) {
                // X resolution
                RSGL::drawRect({window::x,window::y,240, (160-(window::w/2))}, {255,0,0});
	            RSGL::drawRect({window::x+(window::w+(160-(window::w/2))),0,240,(160-(window::w/2))}, {255,0,0});
                // Y resolution
                RSGL::drawRect({window::x,window::y+window::h,(120-(window::h/2))*2, 400}, {0,255,0});
            }*/
            C3D_FrameEnd(0);
        }
    };

    void window::checkEvents() {
        touchPosition touch;
        circlePosition cpad;
        hidScanInput();
        hidTouchRead(&touch);
        hidCircleRead(&cpad);
        window::event.cpad_x = cpad.dx;
        window::event.cpad_y = cpad.dy;
        window::event.x = touch.px;
        window::event.y = touch.py;
        window::event.type = 
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(RSGL::screen, RSGL::background_color);
        C2D_SceneBegin(RSGL::screen);
    }


    int drawTriangle(triangle t, color c, bool solid=true) {
        u32 colour = C2D_Color32(c.r, c.g, c.b, 0xFF);
        C2D_DrawTriangle(
            t.x+(t.length/2), t.y-(t.width/2), colour, 
			t.x,  t.y, colour,
		    t.x+t.length, t.y, colour, 0
        );
        if (!solid) {
            C2D_DrawTriangle(
                t.x+(t.length/2)+3, t.y-(t.width/2)-3, background_color, 
			    t.x+3,  t.y-3, background_color,
		        t.x+t.length, t.y-3, background_color, 0
            );
        }
        return 1;
    }
    int drawCircle(circle c, color col, bool solid=true, bool shadow=false) {
        u32 clr = C2D_Color32(col.r, col.g, col.b, 0xFF);
        if (shadow) C2D_DrawCircleSolid(c.x, c.y, 0, c.radius+1, C2D_Color32(col.r, col.g, col.b, 60));
        C2D_DrawCircleSolid(c.x, c.y, 0, c.radius, clr);
        if (!solid) {
            C2D_DrawCircleSolid(c.x, c.y, 0, c.radius-3, background_color);
        }
        return 1;
    }

    int enableConsole(C3D_RenderTarget* screen) {
        if (screen == RSGL::bottomScreen) {consoleInit(GFX_BOTTOM, NULL);}
        if (screen == RSGL::topScreen) {consoleInit(GFX_TOP, NULL);}
        else {return 0;} 
        //The programmer has a nap!
        //Hold out! Programmer!
        return 1;
    }

    image loadImage(int number, rect r) {
        C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
        
        static Sprite sprites[768];
        Sprite* sprite = &sprites[number];
	    C2D_SpriteFromSheet(&sprite->spr, spriteSheet, number);
		C2D_SpriteSetPos(&sprite->spr, r.x, r.y );
        if (r.length != 0 && r.width != 0) {C2D_SpriteScale(&sprite->spr, r.width, 1.11111);}
        return {number, r, sprite};
    }
    int drawImage(image img) {
        C2D_DrawSprite(&img.sprite[0].spr);
        return 0;
    }
    Time GetTime() {
        time_t t1 = time(NULL);
	    std::string s = std::ctime(&t1);
        int month=19910623;
        if (s.substr(3, 4) == " Jan") month=1; //Could be better but whatver, it works just fine
        else if (s.substr(3, 4) == " Feb") month=2;
        else if (s.substr(3, 4) == " Mar") month=3;
        else if (s.substr(3, 4) == " Apr") month=4;
        else if (s.substr(3, 4) == " May") month=5;
        else if (s.substr(3, 4) == " Jun") month=6;
        else if (s.substr(3, 4) == " Jul") month=7;
        else if (s.substr(3, 4) == " Aug") month=8;
        else if (s.substr(3, 4) == " Sep") month=9;
        else if (s.substr(3, 4) == " Oct") month=10;
        else if (s.substr(3, 4) == " Nov") month=11;
        else if (s.substr(3, 4) == " Dec") month=12;
        return {
            std::stoi(s.substr(20, 24)), //year
            month, //month
            std::stoi(s.substr(9, 9)), //day
            std::stoi(s.substr(11, 12)), //hour
            std::stoi(s.substr(14, 15)), //minutes
            std::stoi(s.substr(17, 18)), //seconds
            s.substr(0, 3) //day
        };
    }
};


std::string RSGL::getLanguage() {
    u8 language = 0;
    CFGU_GetSystemLanguage(&language); //doesn't even matter bruv
    switch ((int)language) {
        case 0:
        return "Japanese";
        break;

        case 1:
        return "English";
        break;

        case 2:
        return "French";
        break;

        case 3:
        return "German";
        break;

        case 4:
        return "Italian";
        break;

        case 5:
        return "Spanish";
        break;

        case 6:
        return "Simplified Chinese";
        break;

        case 7:
        return "Korean";
        break;

        case 8:
        return "Dutch";
        break;

        case 9:
        return "Portuguese";
        break;

        case 10:
        return "Russian";
        break;

        case 11:
        return "Traditional Chinese";
        break;
    }
    return "N/A";
}

std::string RSGL::getSystem() {
    u8 s = 0;
    CFGU_GetSystemModel(&s); //doesn't even matter bruv
    switch ((int)s) {
        case 0:
        return "3ds";
        break;

        case 1:
        return "3ds_XL";
        break;

        case 2:
        return "New_3ds";
        break;

        case 3:
        return "2ds";
        break;

        case 4:
        return "New_3ds_XL";
        break;

        case 5:
        return "New_2ds_XL";
        break;
    }
    return "N/A";
}

std::string RSGL::getRegion() {
    u8 s = 0;
    CFGU_GetSystemModel(&s); //doesn't even matter bruv
    switch ((int)s) {
        case 0:
        return "JPN";
        break;

        case 1:
        return "USA";
        break;

        case 2:
        return "EUR";
        break;

        case 3:
        return "AUS";
        break;

        case 4:
        return "CHN";
        break;

        case 5:
        return "KOR";
        break;

        case 6:
        return "TWN";
        break;
    }
    return "N/A";
}

int RSGL::enableRomfs() {
    romfsInit();
    romfs=true;
    return 1;
}
    
int RSGL::clearConsole() {
    consoleClear();
    return 1;
}