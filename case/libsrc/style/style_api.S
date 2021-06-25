#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"
//<><><><><><><><><><><><><><><><><><><><><><><><><><>

LEAF(open_style_file)
	.set noreorder
	.text
open_style_file:
	li	v1, STYLE_API_START
	syscall
	nop
END (open_style_file)

LEAF(close_style_file)
	.set noreorder
	.text
close_style_file:
	li	v1, STYLE_API_START+1
	syscall
	nop
END (close_style_file)


LEAF(load_scene)
	.set noreorder
	.text
load_scene:
	li	v1, STYLE_API_START+2
	syscall
	nop
END (load_scene)

LEAF(free_scene)
	.set noreorder
	.text
free_scene:
	li	v1, STYLE_API_START+3
	syscall
	nop
END (free_scene)




LEAF(get_scene_child)
	.set noreorder
	.text
get_scene_child:
	li	v1, STYLE_API_START+4
	syscall
	nop
END (get_scene_child)

LEAF(get_resgroup_child)
	.set noreorder
	.text
get_resgroup_child:
	li	v1, STYLE_API_START+5
	syscall
	nop
END (get_resgroup_child)



LEAF(get_picregion_frame)
	.set noreorder
	.text
get_picregion_frame:
	li	v1, STYLE_API_START+6
	syscall
	nop
END (get_picregion_frame)



LEAF(get_style_api)
	.set noreorder
	.text
get_style_api:
	li	v1, STYLE_API_START+7
	syscall
	nop
END (get_style_api)
