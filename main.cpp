#include "mangaviewer.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    qApp->setPalette(darkPalette);
    qApp->setStyle(QStyleFactory::create("Fusion"));

    MangaViewer w;
    w.show();
    return a.exec();
}
