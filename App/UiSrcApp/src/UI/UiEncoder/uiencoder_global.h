#ifndef UIENCODER_GLOBAL_H
#define UIENCODER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UIENCODER_LIBRARY)
#  define UIENCODERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UIENCODERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UIENCODER_GLOBAL_H
