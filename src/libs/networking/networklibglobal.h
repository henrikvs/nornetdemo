#include <QtCore/QtGlobal>

#if defined(NETWORKLIB)
#define NETWORKLIB_EXPORT Q_DECL_EXPORT
#else
#define NETWORKLIB_EXPORT Q_DECL_IMPORT
#endif
