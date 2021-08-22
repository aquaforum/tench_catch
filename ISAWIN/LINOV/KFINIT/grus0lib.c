#include <tasy0def.h>


extern UFP uspdef_eep_n_w (char *name);
extern UFP uspdef_eep_pr (char *name);
extern UFP uspdef_eep_en (char *name);
extern UFP uspdef_real_int (char *name);

UFP_LIST USPDEF[] = {
   uspdef_eep_n_w,
   uspdef_eep_pr,
   uspdef_eep_en,
   uspdef_real_int,
NULL };
