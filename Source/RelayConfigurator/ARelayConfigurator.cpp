#include <QApplication>
#include <ARelayConfiguratorDialog.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ARelayConfiguratorDialog* dialog = new ARelayConfiguratorDialog();
    dialog->show();
    return app.exec();
}