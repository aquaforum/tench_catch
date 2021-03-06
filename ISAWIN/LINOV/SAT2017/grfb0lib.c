#include <tasy0def.h>

extern ABP fbldef_ebus_f_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_systim_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_sysdat_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_ebus_n_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_mbus_n_w (char *name, IBP *init, RBP *read);
extern ABP fbldef_mbus_r (char *name, IBP *init, RBP *read);
extern ABP fbldef_wd_bit (char *name, IBP *init, RBP *read);
extern ABP fbldef_pid_al (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_ebus_f_r,
   fbldef_systim_r,
   fbldef_sysdat_r,
   fbldef_systim_w,
   fbldef_sysdat_w,
   fbldef_ebus_n_r,
   fbldef_mbus_n_w,
   fbldef_mbus_r,
   fbldef_wd_bit,
   fbldef_pid_al,
NULL };
