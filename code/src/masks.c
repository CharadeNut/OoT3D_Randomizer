#include "z3D/z3D.h"
#include "z3D/actors/z_en_item00.h"
#include "settings.h"

typedef enum {
    /* 0x00 */ PLAYER_MASK_NONE,
    /* 0x01 */ PLAYER_MASK_KEATON,
    /* 0x02 */ PLAYER_MASK_SKULL,
    /* 0x03 */ PLAYER_MASK_SPOOKY,
    /* 0x04 */ PLAYER_MASK_BUNNY,
    /* 0x05 */ PLAYER_MASK_GORON,
    /* 0x06 */ PLAYER_MASK_ZORA,
    /* 0x07 */ PLAYER_MASK_GERUDO,
    /* 0x08 */ PLAYER_MASK_TRUTH,
    /* 0x09 */ PLAYER_MASK_MAX
} PlayerMask;

f32 Misc_BunnyHoodFeature(void) {
	if (/*gSettingsContext.maskEffects && */ PLAYER->currentMask == PLAYER_MASK_BUNNY) {
		return 3;
	}
	return 1;
}