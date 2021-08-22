#include <tasy0def.h>

extern ABP fbldef_ebus_n_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_b_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_f_r (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_ebus_n_w,
   fbldef_ebus_b_r,
   fbldef_ebus_f_r,
NULL };
