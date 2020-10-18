#ifndef LIFTUI_GLOBAL_H
#define LIFTUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIFTUI_LIBRARY)
#  define LIFTUI_EXPORT Q_DECL_EXPORT
#else
#  define LIFTUI_EXPORT Q_DECL_IMPORT
#endif

#endif // LIFTUI_GLOBAL_H
