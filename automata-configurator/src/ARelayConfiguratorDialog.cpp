#include "ARelayConfiguratorDialog.h"

#include <QMessageBox>
#include <QRegExp>
#include <QUdpSocket>
#include <QtEndian>
#include "ui_ARelayConfiguratorDialog.h"

struct Configuration
{
    bool configured;
    uint32_t UUID;
    char hostname[32];
    bool powerOnState;      
    bool DHCP;
    uint32_t IP;
    uint32_t subnet;
    uint32_t gateway;  
    uint16_t httpPort;
};

const uint32_t ConfigurationBroadcastPacketHeader = 0xb713e302;
struct ConfigurationBroadcastPacket
{
    uint32_t header;
    Configuration configuration;
};

const uint32_t ConfigurationBroadcastReplyPacketHeader = 0x53f59729;
struct ConfigurationBroadcastReplyPacket
{
    uint32_t header;
};

static ConfigurationBroadcastPacket packet;

void ARelayConfiguratorDialog::btnStartBroadcasting_clicked()
{
    if (!broadcastTimer->isActive())
    {
        ConfigurationBroadcastPacket packet;
        packet.header = ConfigurationBroadcastPacketHeader;
        
        packet.configuration.UUID = form->spnUUID->value();

        QByteArray hostname = form->txtHostname->text().toUtf8();
        memcpy(packet.configuration.hostname, hostname.data(), hostname.size());
        
        packet.configuration.DHCP = form->chkDHCP->isChecked();
        if (!packet.configuration.DHCP)
        {
            packet.configuration.IP = QHostAddress(form->txtIP->text()).toIPv4Address();
            packet.configuration.subnet = QHostAddress(form->txtSubnet->text()).toIPv4Address();
            packet.configuration.gateway = QHostAddress(form->txtGateway->text()).toIPv4Address();
        }
        else
        {
            packet.configuration.IP = 0x00000000;
            packet.configuration.subnet = 0x00000000;
            packet.configuration.gateway = 0x00000000;
        }

        packet.configuration.powerOnState = form->chkPowerOn->isChecked();
        packet.configuration.httpPort = form->spnHttpPort->value();

        if (!socket.bind(1089))
        {
            QMessageBox::critical(this, "Cannot bind socket to port 1089.", "Error Occured", QMessageBox::Ok);
            return;
        }

        broadcastTimer->start();
        form->lblBroadcast->setVisible(true);
        form->btnStartBroadcasting->setText("Stop Broadcasting");
    }
    else
    {
        broadcastTimer->stop();
        socket.close();
        form->lblBroadcast->setVisible(false);
        form->btnStartBroadcasting->setText("Start Broadcasting");
    }

    form->spnUUID->setEnabled(!broadcastTimer->isActive());
    form->txtHostname->setEnabled(!broadcastTimer->isActive());
    form->chkDHCP->setEnabled(!broadcastTimer->isActive());
    form->txtIP->setEnabled(!broadcastTimer->isActive() && !form->chkDHCP->isChecked());
    form->txtSubnet->setEnabled(!broadcastTimer->isActive() && !form->chkDHCP->isChecked());
    form->txtGateway->setEnabled(!broadcastTimer->isActive() && !form->chkDHCP->isChecked());
    form->chkPowerOn->setEnabled(!broadcastTimer->isActive());
    form->spnHttpPort->setEnabled(!broadcastTimer->isActive());
}

void ARelayConfiguratorDialog::broadcast()
{
    form->lblBroadcast->setVisible(!form->lblBroadcast->isVisible());

    socket.writeDatagram((const char*)&packet, sizeof(ConfigurationBroadcastPacket), QHostAddress::Broadcast, 1089);
    
    ConfigurationBroadcastReplyPacket packet;
    QHostAddress relayIP;
    
    while (true)
    {
        if (!socket.hasPendingDatagrams())
            break;

        if (socket.readDatagram((char*)&packet, sizeof(ConfigurationBroadcastReplyPacket), &relayIP) != sizeof(ConfigurationBroadcastReplyPacket))
            continue;

        if (packet.header != ConfigurationBroadcastReplyPacketHeader)
            continue;

        QMessageBox::information(this, 
            QString("Confirmation packed received from a relay with IP {1}. Stopping broadcasting.").arg(relayIP.toString()), 
            "Confirmation received", QMessageBox::Ok);
        
        btnStartBroadcasting_clicked();
    }
}

bool ARelayConfiguratorDialog::validate()
{
    bool validated = true;

    QRegExp regexHostname("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");
    if (!regexHostname.exactMatch(form->txtHostname->text()))
    {
        validated = false;
        form->txtHostname->setStyleSheet("background-color:red; color: white;");
    }
    else
    {
        form->txtHostname->setStyleSheet("");
    }
    
    QRegExp regexIP(
        "\\b(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
        "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
        "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\."
        "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\b");

    form->txtIP->setEnabled(!form->chkDHCP->isChecked());
    form->txtSubnet->setEnabled(!form->chkDHCP->isChecked());
    form->txtGateway->setEnabled(!form->chkDHCP->isChecked());

    if (!form->chkDHCP->isChecked())
    {
        if (!regexIP.exactMatch(form->txtIP->text()))
        {
            validated = false;
            form->txtIP->setStyleSheet("background-color:red; color: white;");
        }
        else
        {
            form->txtIP->setStyleSheet("");
        }

        if (!regexIP.exactMatch(form->txtSubnet->text()))
        {
            validated = false;
            form->txtSubnet->setStyleSheet("background-color:red; color: white;");
        }
        else
        {
            form->txtSubnet->setStyleSheet("");
        }

        if (!regexIP.exactMatch(form->txtGateway->text()))
        {
            validated = false;
            form->txtGateway->setStyleSheet("background-color:red; color: white;");
        }
        else
        {
            form->txtGateway->setStyleSheet("");
        }
    }

    form->btnStartBroadcasting->setEnabled(validated);
    
    return validated;
}

ARelayConfiguratorDialog::ARelayConfiguratorDialog()
{
    form = new Ui_ARelayConfiguratorDialog();
    form->setupUi(this);

    form->lblBroadcast->setVisible(false);
    form->spnUUID->setValue(qrand() % 999999999);
    
    broadcastTimer = new QTimer(this);
    broadcastTimer->setInterval(500);

    QObject::connect(this->form->btnStartBroadcasting, &QAbstractButton::clicked, this, &ARelayConfiguratorDialog::btnStartBroadcasting_clicked);
    QObject::connect(this->form->txtIP, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->txtSubnet, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->txtGateway, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->chkDHCP, &QCheckBox::stateChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->broadcastTimer, &QTimer::timeout, this, &ARelayConfiguratorDialog::broadcast);

    validate();
}

ARelayConfiguratorDialog::~ARelayConfiguratorDialog()
{
    delete form;
}