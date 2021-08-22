#include <tasy0def.h>

extern ABP fbldef_i_7017 (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_f_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_b_w (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_i_7017,
   fbldef_ebus_f_w,
   fbldef_ebus_n_r,
   fbldef_ebus_b_w,
NULL };
