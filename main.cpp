#include "chessboardwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessBoardWindow w;
    w.show();

    return a.exec();
}
