#include <tasy0def.h>


extern UFP uspdef_retain_x (char *name);
extern UFP uspdef_to_a4_20 (char *name);
extern UFP uspdef_a4_20_to (char *name);
extern UFP uspdef_wd_long (char *name);

UFP_LIST USPDEF[] = {
   uspdef_retain_x,
   uspdef_to_a4_20,
   uspdef_a4_20_to,
   uspdef_wd_long,
NULL };
