#include "ucos/module.h"
#include "ucos/init.h"
#include "ucos/stringify.h"

#pragma __PRQA_IGNORE_START__
struct module __this_module __attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = __stringify(KBUILD_MODNAME),
 .init = init_module,
 .exit = cleanup_module,
};
#pragma __PRQA_IGNORE_END__
