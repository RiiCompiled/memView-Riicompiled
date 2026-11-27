#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "BMfont5.h"
#include <grrlib.h>
#include "pointer.h"

#define MAX_PAGE 600

char ascii(char s) {
  if(s < 0x20) return '.';
  if(s > 0x7E) return '.';
  return s;
}

int main(int argc, char **argv) {
	ir_t ir;
	GRRLIB_Init();
	WPAD_Init();
	WPAD_SetIdleTimeout(60);
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	GRRLIB_texImg *tex_BMfont5 = GRRLIB_LoadTexture(BMfont5);
	GRRLIB_texImg *tex_pointer = GRRLIB_LoadTexture(pointer);
    GRRLIB_InitTileSet(tex_BMfont5, 8, 16, 0);
	int wpadx, wpady;
	int page = 0;
	while(1) {
		GRRLIB_FillScreen(0x000000FF);
		WPAD_ScanPads();
        WPAD_IR(WPAD_CHAN_0, &ir);
        wpadx=ir.sx-160;
        wpady=ir.sy-220;
		
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_HOME ) break;
		else if (pressed & WPAD_BUTTON_B) {
			page--;
			if (page < 0) page = MAX_PAGE-1;
		} else if (pressed & WPAD_BUTTON_A) {
			page++;
			if (page > MAX_PAGE-1) page = 0;
		}
		int * tmp = (int *)0x80000000+(page*140);
		int i, n, x, y;
		int popx = -1, popy = -1;
		int * popup;
		for (i=0;i<20;i++) {
			for (n=0; n<7; n++) {
				x = 40+(n*80);
				y = 50+(i*20);
				GRRLIB_Printf(x, y, tex_BMfont5, ((*tmp == 0)? 0xFFFFFFFF : 0xFFFF00FF), 1, "%08X", *tmp);
				if (GRRLIB_PtInRect(x,y, 80, 10, wpadx, wpady)) { 	
					popx = ((n > 5) ? x-100 : x+64);
					popy = ((i > 17) ? y-50 : y+10);
					popup = tmp;
				}
				tmp++;
			}
		}
		GRRLIB_DrawImg(wpadx-32, wpady-32, tex_pointer, 0, 1, 1, 0xFFFFFFFF);
		GRRLIB_Printf(128, 30, tex_BMfont5, 0xFFFFFFFF, 1, "Gannon's (crappy) Wii Memory Viewer Version 0.2a");
		//9 - 17 - 18 - 15 - 9
		GRRLIB_Printf(40, 450, tex_BMfont5, 0xFFFFFFFF, 1, "<< B");
		GRRLIB_Printf(92, 450, tex_BMfont5, 0xFFFFFFFF, 1, "Start: %p", 0x80000000+(page*140));
		GRRLIB_Printf(248, 450, tex_BMfont5, 0xFFFFFFFF, 1, "Press Home to Exit");
		GRRLIB_Printf(420, 450, tex_BMfont5, 0xFFFFFFFF, 1, "End: %p", tmp);
		GRRLIB_Printf(568, 450, tex_BMfont5, 0xFFFFFFFF, 1, "A >>");
		if (popx > 0) {
			GRRLIB_Rectangle(popx, popy, 100, 50, 0x000000FF, 1);
			GRRLIB_Rectangle(popx, popy, 100, 50, 0x00FF00FF, 0);
			char hex[5];
			memcpy(hex, popup, 4);
			for (i=0; i<4; i++) hex[i]=ascii(hex[i]);
			GRRLIB_Printf(popx+10, popy+10, tex_BMfont5, 0xFFFFFFFF, 1, "%p", popup);
			GRRLIB_Printf(popx+10, popy+30, tex_BMfont5, 0xFFFFFFFF, 1, "%s", hex);

		}
		GRRLIB_Render();
	}
    GRRLIB_Exit();
    GRRLIB_FreeTexture(tex_BMfont5);
    GRRLIB_FreeTexture(tex_pointer);
	exit(0);
}