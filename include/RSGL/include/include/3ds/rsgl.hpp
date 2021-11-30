#pragma once
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
    int clearConsole();
    int getLanguage();
    int getSystem();


    const int KeyPressed=2;
    const int KeyReleased=3;
    const int TouchscreenPressed=4;
    const int TouchscreenReleased=5;
    const int MousePosChanged=6;
    const int quit = 33;
    const int dnd = 34;

    struct rect{int x,y; int width, length;};
    struct circle { int x, y; int radius; };
    struct triangle { int x, y; int length, width; };
    struct image {int number; RSGL::rect r; Sprite* sprite;};
    struct color { int r, g, b; };
    struct point { int x, y;};
    struct text{
        std::string text;
        RSGL::rect r;
        std::string font;
        RSGL::color col;
        int pos;
        float width;
        float length;
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
    C3D_RenderTarget* screen[2] = {C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT), C2D_CreateScreenTarget(GFX_BOTTOM, GFX_RIGHT)};

    C2D_TextBuf g_staticBuf;
    C2D_Text g_staticText[16];
    C2D_Font font[16]; int fnt_size=0;

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

    struct window {
        C3D_RenderTarget* screen;
    
        int x;
        int y;
        int w;
        int h;
        int screen_num;
        RSGL::color clr;
        struct Event {
            int x, y;           //touch X and Y
            int cpad_x, cpad_y; //C-pad X and Y
            int type;
        };
        int CheckType() {
            if (hidKeysHeld() & KEY_TOUCH) return RSGL::TouchscreenPressed;
            if (hidKeysUp() & KEY_TOUCH) return RSGL::TouchscreenReleased;
            if (hidKeysHeld()) return RSGL::KeyPressed;
            if (hidKeysUp()) return RSGL::KeyReleased;
            return 0;
        }
        Event event; 
        window(C3D_RenderTarget* screen, RSGL::rect r, RSGL::color c) {
            gfxInitDefault();
            romfsInit();
            cfguInit();
            C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	        C2D_Prepare();
            //chdir("romfs:/");
            RSGL::background_color = C2D_Color32(c.r, c.g, c.b, 0xFF); window::clr = c;  window::screen = screen; window::x = r.x; window::y = r.y; window::w = r.width; window::h = r.length;
        };
        void close() {
            C2D_TextBufDelete(g_staticBuf);
            for (int i=0; i<=fnt_size; i++) C2D_FontFree(font[i]);
			C3D_Fini();
 			gfxExit();
 			cfguExit();
    		romfsExit();
            window::event.type=RSGL::quit;
        }
        void checkEvents();

        bool isPressed(u32 key) { return hidKeysHeld() & key; }
        bool isClicked(u32 key) { return hidKeysDown() & key; }
        bool isReleased(u32 key) { return hidKeysUp() & key; }

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
        window::event.type = window::CheckType();
        if (window::screen == RSGL::screen[0]) window::screen_num=0; 
        else {window::screen_num=1;}
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(RSGL::screen[screen_num], RSGL::background_color);
        C2D_SceneBegin(RSGL::screen[screen_num]);
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
        if (screen == RSGL::screen[1]) {consoleInit(GFX_BOTTOM, NULL);}
        if (screen == RSGL::screen[0]) {consoleInit(GFX_TOP, NULL);}
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
    text loadText(std::string text, rect r, std::string fnt, color col) {
        g_staticBuf = C2D_TextBufNew(4096);
        int num =  fnt_size; std::string romf = "romfs:/";
        font[num] = C2D_FontLoad(romf.append(fnt).c_str()); 
        C2D_TextFontParse(&g_staticText[num], font[num], g_staticBuf, text.c_str());
        C2D_TextOptimize(&g_staticText[num]);
        float h = 1.0f;
        C2D_TextGetDimensions(&g_staticText[num], 1.0f, 1.0f, 0, &h);

        fnt_size++;
        return {text, r, romf.append(fnt), col, num, g_staticText[num].width, h};
    }

    int drawText(text txt){
        C2D_DrawText(&g_staticText[txt.pos], C2D_WithColor, txt.r.x, txt.r.y, 0.0f, (float)txt.r.width/txt.width, (float)txt.r.length/txt.length, C2D_Color32(txt.col.r, txt.col.g, txt.col.b, 0xFF));
        return 1; 
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

int RSGL::getLanguage() {
    u8 language = 0;
    CFGU_GetSystemLanguage(&language); //doesn't even matter bruv
    return (int)language;
    //0 - Japanese, 1 - english, 2 - french, 3 - german, 4 - italian, 5 - spanish, 6 - simplified chinese, 7 - korean, 8 - dutch, 9 - portuguese, 10 - russian, 11 - traditional chinese
}
int RSGL::getSystem() {
    u8 s = 0;
    CFGU_GetSystemModel(&s); //doesn't even matter bruv
    return (int)s;
    //0 - 3DS, 1 - 3DS XL, 2 - New 3DS, 3 - 2DS, 4 - New 3DS XL, 5- New 2DS XL
}

int RSGL::clearConsole() {
    consoleClear();
    return 1;
}