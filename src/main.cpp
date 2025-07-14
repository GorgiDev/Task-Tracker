#include <QApplication>
#include <QFile>
#include "TaskTrackerApp.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/assets/icons/app_icon.ico"));

    TaskTrackerApp window;
    window.show();

    return app.exec();
}