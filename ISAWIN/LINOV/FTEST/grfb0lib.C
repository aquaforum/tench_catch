#include <tasy0def.h>

extern ABP fbldef_i_7043d (char *name, IBP *init, RBP *read);
extern ABP fbldef_i_7053d (char *name, IBP *init, RBP *read);

FBL_LIST FBLDEF[] = {
   fbldef_i_7043d,
   fbldef_i_7053d,
NULL };
