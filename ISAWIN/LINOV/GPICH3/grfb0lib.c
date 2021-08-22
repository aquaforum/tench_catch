#include <tasy0def.h>

extern ABP fbldef_systim_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7024 (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7017 (char *name, IBP *init, RBP *read);
extern ABP fbldef_pid_al (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_systim_w,
   fbldef_sysdat_w,
   fbldef_systim_r,
   fbldef_sysdat_r,
   fbldef_i_7024,
   fbldef_i_7017,
   fbldef_pid_al,
NULL };