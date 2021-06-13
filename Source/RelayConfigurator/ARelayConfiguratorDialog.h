#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>

class Ui_ARelayConfiguratorDialog;
class Configuration;

class ARelayConfiguratorDialog : public QMainWindow
{
    private:
        Ui_ARelayConfiguratorDialog* form;
        QTimer* broadcastTimer;
        QUdpSocket socket;
        QString filename;

        void saveForm(Configuration* configuration);
        void loadForm(Configuration* configuration);


    private slots:
        void mnuNew_clicked();
        void mnuOpen_clicked();
        void mnuSave_clicked();
        void mnuSaveAs_clicked();
        void mnuExit_clicked();
        void btnStartBroadcasting_clicked();
        void broadcast();
        bool validate();

    public:
        ARelayConfiguratorDialog();
        virtual ~ARelayConfiguratorDialog();
};
