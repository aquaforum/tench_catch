#include <tasy0def.h>

extern ABP fbldef_mbus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_f_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7017 (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7022 (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7033 (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7024 (char *name, IBP *init, RBP *read);
extern ABP fbldef_pid_al (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_mbus_n_r,
   fbldef_ebus_f_w,
   fbldef_sysdat_w,
   fbldef_systim_r,
   fbldef_sysdat_r,
   fbldef_systim_w,
   fbldef_i_7017,
   fbldef_i_7022,
   fbldef_i_7033,
   fbldef_i_7024,
   fbldef_pid_al,
NULL };
