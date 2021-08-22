#include <tasy0def.h>

extern ABP fbldef_sysdat_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7043d (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7053d (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_sysdat_w,
   fbldef_systim_r,
   fbldef_sysdat_r,
   fbldef_systim_w,
   fbldef_i_7043d,
   fbldef_i_7053d,
NULL };
