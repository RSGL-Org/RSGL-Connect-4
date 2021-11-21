#pragma once
#include <string.h>
#include <iostream>
#include <ctime>   

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

    struct rect{int x,y; int length, width;};
    struct circle { int x, y; int radius; };
    struct triangle { int x, y; int length, width; };
    struct image {int number; RSGL::rect r; Sprite* sprite;};
    struct color { int r, g, b; };
    struct point { int x, y;};

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

    struct window {
        int touch_x, touch_y, cpad_x, cpad_y;
        const char* name;
    
        int x;
        int y;
        int w;
        int h;
        RSGL::color clr;

        window(C3D_RenderTarget* screen, const char* name, RSGL::rect r, RSGL::color c) {
            gfxInitDefault();
            cfguInit();
            C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	        C2D_Prepare();
            RSGL::background_color = C2D_Color32(c.r, c.g, c.b, 0xFF); window::clr = c; RSGL::screen = screen;  window::name = name; window::x = r.x; window::y = r.y; window::w = r.width; window::h = r.length;
        };
        void close() {
			C3D_Fini();
 			gfxExit();
 			cfguExit();
    		if (romfs) romfsExit();
        }
        void checkEvents();
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
        window::touch_x = touch.px;
        window::touch_y = touch.py;
        window::cpad_x = cpad.dx;
        window::cpad_y = cpad.dy;
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(RSGL::screen, RSGL::background_color);
        C2D_SceneBegin(RSGL::screen);
    }

    bool isPressed(u32 key) { return hidKeysHeld() & key; }
    bool isClicked(u32 key) { return hidKeysDown() & key; }
    bool isReleased(u32 key) { return hidKeysUp() & key; }

    int CircleCollidePoint(RSGL::circle c, RSGL::point p){
		float testX = c.x; float testY = c.y;

  		if (c.x < p.x) {testX = p.x;}  else if (c.x > p.x+1) {testX = p.x+1;}
  		if (c.y < p.y) {testY = p.y;}  else if (c.y > p.y+1) {testY = p.y+1;} 
  
  		return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
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