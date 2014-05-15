#include <QtCore/QtGlobal>

#if defined(UTILSLIB)
#define UTILSLIB_EXPORT Q_DECL_EXPORT
#else
#define UTILSLIB_EXPORT Q_DECL_IMPORT
#endif
