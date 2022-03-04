#include "folderwidget.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("Folder Test");
    app.setApplicationVersion("0.1");

    FolderWidget w;
    w.show();

    return app.exec();
}
