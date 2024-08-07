#include <QtCore/qglobal.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/qapplication.h>
#else // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/qapplication.h>
#endif // (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

#include "../include/precompiled.h"
#include "../include/main_dlg.h"

#include <QxOrm_Impl.h>

int main(int argc, char * argv[])
{
   QApplication app(argc, argv);

   main_dlg dlg;
   dlg.show();

   return app.exec();
}
