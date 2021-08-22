#include <tasy0def.h>


extern UFP uspdef_ebus_sts (char *name);
extern UFP uspdef_v0_10_to (char *name);
extern UFP uspdef_a4_20_to (char *name);

UFP_LIST USPDEF[] = {
   uspdef_ebus_sts,
   uspdef_v0_10_to,
   uspdef_a4_20_to,
NULL };
