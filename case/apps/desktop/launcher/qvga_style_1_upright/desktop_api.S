#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"

//<><><><><><><><><><><><><><><><><><><><><><><><><><>
LEAF(launcher_scene_desktop)
	.set noreorder
	.text
launcher_scene_desktop:
	li	v1, DESKTOP_API_START+0x00
	syscall
END (launcher_scene_desktop)

