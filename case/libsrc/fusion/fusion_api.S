#include "asm-mips/asm.h"
#include "asm-mips/regdef.h"
#include "ucos/api.h"

//<><><><><><><><><><><><><><><><><><><><><><><><><><>
LEAF(fusion_set_effect)
	.set noreorder
	.text
fusion_set_effect:
	li	v1, FUSION_API_START+0x00
	syscall
END (fusion_set_effect)

LEAF(fusion_set_first_frame)
	.set noreorder
	.text
fusion_set_first_frame:
	li	v1, FUSION_API_START+0x01
	syscall
END (fusion_set_first_frame)

LEAF(fusion_set_last_frame)
	.set noreorder
	.text
fusion_set_last_frame:
	li	v1, FUSION_API_START+0x02
	syscall
END (fusion_set_last_frame)

LEAF(fusion_display)
	.set noreorder
	.text
fusion_display:
	li	v1, FUSION_API_START+0x03
	syscall
END (fusion_display)

LEAF(fusion_set_config)
	.set noreorder
	.text
fusion_set_config:
	li	v1, FUSION_API_START+0x04
	syscall
END (fusion_set_config)

LEAF(fusion_get_config)
	.set noreorder
	.text
fusion_get_config:
	li	v1, FUSION_API_START+0x05
	syscall
END (fusion_get_config)

LEAF(fusion_update_config)
	.set noreorder
	.text
fusion_update_config:
	li	v1, FUSION_API_START+0x06
	syscall
END (fusion_update_config)

