#include <tasy0def.h>


extern UFP uspdef_retain_x (char *name);
extern UFP uspdef_v0_10_to (char *name);
extern UFP uspdef_a4_20_to (char *name);
extern UFP uspdef_wd_long (char *name);
extern UFP uspdef_to_a4_20 (char *name);
extern UFP uspdef_w_mb_rel (char *name);
extern UFP uspdef_r_mb_rel (char *name);

UFP_LIST USPDEF[] = {
   uspdef_retain_x,
   uspdef_v0_10_to,
   uspdef_a4_20_to,
   uspdef_wd_long,
   uspdef_to_a4_20,
   uspdef_w_mb_rel,
   uspdef_r_mb_rel,
NULL };
