#include <tasy0def.h>

extern ABP fbldef_mbus_r (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_mbus_r,
NULL };
