#include <tasy0def.h>


extern UFP uspdef_retain_x (char *name);
extern UFP uspdef_v0_10_to (char *name);
extern UFP uspdef_a4_20_to (char *name);
extern UFP uspdef_int_real (char *name);
extern UFP uspdef_to_a4_20 (char *name);

UFP_LIST USPDEF[] = {
   uspdef_retain_x,
   uspdef_v0_10_to,
   uspdef_a4_20_to,
   uspdef_int_real,
   uspdef_to_a4_20,
NULL };
