#include <nds.h>
#include <nds/fifocommon.h>
#include <stdlib.h>

#include "gfx/pucmcawesome.png.h"
#include "gfx/bgbottom.png.h"
#include "gfx/bgtop.png.h"

int main()
{
// 	irqInit();
// 	irqEnable(IRQ_VBLANK);
// 	fifoInit();
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	u16* puc = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	dmaCopy((u8*)pucmcawesome_pngTiles, puc, 32*32*6);
	dmaCopy(pucmcawesome_pngPal, SPRITE_PALETTE, 512);

	int bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(bgtop_pngBitmap, bgGetGfxPtr(bg), 256*256);
	dmaCopy(bgtop_pngPal, BG_PALETTE, 256*2);

	bg = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(bgbottom_pngBitmap, bgGetGfxPtr(bg), 256*256);
	dmaCopy(bgbottom_pngPal, BG_PALETTE_SUB, 256*2);

	int x = 0;
	int y = 0;
	int frame = 0;
	int framedelay = 0;
	while( 1 ) {		
// 		iprintf("\n Lets print pretty text on the\n NDS screen ");
// 		iprintf("\x1b[32musing devkitpro!\n\n");
// 		iprintf("\x1b[32;1m All we need now is actual game\n code!\x1b[39m");
		scanKeys();
		int keys = keysHeld();

		if( keys & KEY_DOWN ) {
			y++;
		}
		if( keys & KEY_UP) {
			y--;
		}
		if( keys & KEY_RIGHT) {
			x++;
		}
		if( keys & KEY_LEFT) {
			x--;
		}
		framedelay = (framedelay + 1)%3;
		if(framedelay == 0) {
			frame = (frame + 1) % 6;
		}
		oamSet(
			&oamMain,
			0, x, y, 0, 0,
			SpriteSize_32x32,
			SpriteColorFormat_256Color,
			puc + (frame * 32 * 32)/2,
			-1, false, false, false, false, false
		);

		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	
	return 0;
}
