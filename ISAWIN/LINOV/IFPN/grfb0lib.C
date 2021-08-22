#include <tasy0def.h>

extern ABP fbldef_i_7043d (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7051d (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_f_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7017 (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_n_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_b_r (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_i_7043d,
   fbldef_i_7051d,
   fbldef_ebus_f_r,
   fbldef_ebus_n_r,
   fbldef_i_7017,
   fbldef_ebus_n_w,
   fbldef_ebus_b_r,
NULL };
