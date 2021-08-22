#include <tasy0def.h>


extern UFP uspdef_eep_n_r (char *name);
extern UFP uspdef_eep_n_w (char *name);
extern UFP uspdef_eep_b_r (char *name);
extern UFP uspdef_eep_b_w (char *name);
extern UFP uspdef_eep_pr (char *name);
extern UFP uspdef_eep_en (char *name);
extern UFP uspdef_real_int (char *name);
extern UFP uspdef_int_real (char *name);
extern UFP uspdef_a4_20_to (char *name);

UFP_LIST USPDEF[] = {
   uspdef_eep_n_r,
   uspdef_eep_n_w,
   uspdef_eep_b_r,
   uspdef_eep_b_w,
   uspdef_eep_pr,
   uspdef_eep_en,
   uspdef_real_int,
   uspdef_int_real,
   uspdef_a4_20_to,
NULL };
