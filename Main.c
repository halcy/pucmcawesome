#include <nds.h>
#include <nds/fifocommon.h>
#include <stdlib.h>

#include "gfx/pucmcawesome.png.h"

int main()
{
// 	irqInit();
// 	irqEnable(IRQ_VBLANK);
// 	fifoInit();
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	u16* puc = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	dmaCopy((u8*)pucmcawesome_pngTiles, puc, 32*32);
	dmaCopy(pucmcawesome_pngPal, SPRITE_PALETTE, 512);

	int x = 0;
	int y = 0;
	
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

		oamSet(
			&oamMain,
			0, x, y, 0, 0,
			SpriteSize_32x32,
			SpriteColorFormat_256Color,
			puc,
			-1, false, false, false, false, false
		);

		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	
	return 0;
}
