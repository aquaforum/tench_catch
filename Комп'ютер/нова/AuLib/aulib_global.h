#ifndef AULIB_GLOBAL_H
#define AULIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AULIB_LIBRARY)
#  define AULIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define AULIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // AULIB_GLOBAL_H
