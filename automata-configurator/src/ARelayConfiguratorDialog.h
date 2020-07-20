#include <QDialog>
#include <QTimer>
#include <QUdpSocket>

class Ui_ARelayConfiguratorDialog;

class ARelayConfiguratorDialog : public QDialog
{
    private:
        Ui_ARelayConfiguratorDialog* form;
        QTimer* broadcastTimer;
        QUdpSocket socket;

    
    private slots:
        void btnStartBroadcasting_clicked();
        void broadcast();
        bool validate();

    public:
        ARelayConfiguratorDialog();
        virtual ~ARelayConfiguratorDialog();
};