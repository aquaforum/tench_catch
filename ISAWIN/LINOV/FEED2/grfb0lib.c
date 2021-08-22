#include <tasy0def.h>

extern ABP fbldef_mbus_n_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_mbus_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_pid_al (char *name, IBP *init, RBP *read);
extern ABP fbldef_wd_bit (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_mbus_n_w,
   fbldef_mbus_r,
   fbldef_pid_al,
   fbldef_wd_bit,
NULL };
