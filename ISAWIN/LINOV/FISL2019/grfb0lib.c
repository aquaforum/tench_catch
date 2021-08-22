#include <tasy0def.h>

extern ABP fbldef_ebus_n_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_pid_al (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7043d (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7053d (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_ebus_n_w,
   fbldef_ebus_n_r,
   fbldef_pid_al,
   fbldef_i_7043d,
   fbldef_i_7053d,
NULL };
