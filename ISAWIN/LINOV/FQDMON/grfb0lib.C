#include <tasy0def.h>

extern ABP fbldef_mbus_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_wd_bit (char *name, IBP *init, RBP *read);
extern ABP fbldef_mbus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_f_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_long_wd (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_mbus_r,
   fbldef_wd_bit,
   fbldef_mbus_n_r,
   fbldef_ebus_f_w,
   fbldef_long_wd,
NULL };
