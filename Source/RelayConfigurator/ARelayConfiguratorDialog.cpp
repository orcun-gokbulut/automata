#include "ARelayConfiguratorDialog.h"

#include <QMessageBox>
#include <QRegExp>
#include <QUdpSocket>
#include <QtEndian>
#include <QFileDialog>

#include "ui_ARelayConfiguratorDialog.h"

const uint32_t ConfigurationHeader = 0x5fe08e26;
struct Configuration
{
    uint32_t header;
    uint32_t UUID;
    char wifiSSID[64];
    char wifiPassword[64];
    char hostname[32];
    bool DHCP;
    uint32_t IP;
    uint32_t subnet;
    uint32_t gateway;
    uint16_t httpPort;
    bool powerOnState;
};

const uint32_t ConfigurationBroadcastReplyPacketHeader = 0x53f59729;
struct ConfigurationBroadcastReplyPacket
{
    uint32_t header;
};

QString ToStringIP(uint32_t ip)
{
    return QString("%1.%2.%3.%4")
        .arg(((quint8*)&ip)[0])
        .arg(((quint8*)&ip)[1])
        .arg(((quint8*)&ip)[2])
        .arg(((quint8*)&ip)[3]);
}

void ARelayConfiguratorDialog::loadForm(Configuration* configuration)
{
    form->spnUUID->setValue(configuration->UUID);
    form->txtHostname->setText(configuration->hostname);
    form->txtWifiSSID->setText(configuration->wifiSSID);
    form->txtWifiPassword->setText(configuration->wifiPassword);
    form->chkDHCP->setChecked(configuration->DHCP);
    form->txtIP->setText(ToStringIP(configuration->IP));
    form->txtSubnet->setText(ToStringIP(configuration->subnet));
    form->txtGateway->setText(ToStringIP(configuration->gateway));
    form->spnHttpPort->setValue(configuration->httpPort);
    form->chkPowerOn->setChecked(configuration->powerOnState);
}

void ARelayConfiguratorDialog::saveForm(Configuration* configuration)
{
    memset(configuration, 0, sizeof(Configuration));

    configuration->header = ConfigurationHeader;
    configuration->UUID = form->spnUUID->value();

    QByteArray hostname = form->txtHostname->text().toLocal8Bit();
    memcpy(configuration->hostname, hostname.data(), hostname.size());

    QByteArray wifiSSID = form->txtWifiSSID->text().toLocal8Bit();
    memcpy(configuration->wifiSSID, wifiSSID.data(), wifiSSID.size());

    QByteArray wifiPassowrd = form->txtWifiPassword->text().toLocal8Bit();
    memcpy(configuration->wifiPassword, wifiPassowrd.data(), wifiPassowrd.size());

    configuration->DHCP = form->chkDHCP->isChecked();
    configuration->IP = qToBigEndian(QHostAddress(form->txtIP->text()).toIPv4Address());
    configuration->subnet = qToBigEndian(QHostAddress(form->txtSubnet->text()).toIPv4Address());
    configuration->gateway = qToBigEndian(QHostAddress(form->txtGateway->text()).toIPv4Address());

    configuration->httpPort = form->spnHttpPort->value();
    configuration->powerOnState = form->chkPowerOn->isChecked();

    validate();
}

void ARelayConfiguratorDialog::mnuNew_clicked()
{
    Configuration configuration;
    memset(&configuration, 0, sizeof(Configuration));
    configuration.UUID = qrand() % 999999999;
    configuration.DHCP = true;
    configuration.httpPort = 80;
    sprintf(configuration.hostname, "Relay-%u", configuration.UUID);
    configuration.IP = qToBigEndian(QHostAddress("192.168.1.61").toIPv4Address());
    configuration.subnet = qToBigEndian(QHostAddress("255.255.255.0").toIPv4Address());
    configuration.gateway = qToBigEndian(QHostAddress("192.168.1.1").toIPv4Address());
    loadForm(&configuration);
}

void ARelayConfiguratorDialog::mnuOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Open Configuration File", QString(), "Relay Configuration File (*.rconf);;All Files (*.*)");
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "File Read Error", "Cannot open file for reading.", QMessageBox::Ok);
        return;
    }

    Configuration configuration;
    qint64 size = file.read((char*)&configuration, sizeof(Configuration));
    if (size != sizeof(Configuration) || configuration.header != ConfigurationHeader)
    {
        QMessageBox::critical(this, "File Read Error", "File is not Relay Configuration file.", QMessageBox::Ok);
        return;
    }
    file.close();

    filename = path;

    loadForm(&configuration);
}

void ARelayConfiguratorDialog::mnuSave_clicked()
{
    if (filename.isEmpty())
        mnuSaveAs_clicked();

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "File Write Error", "Cannot open file for writing.", QMessageBox::Ok);
        return;
    }

    Configuration configuration;
    saveForm(&configuration);

    qint64 size = file.write((char*)&configuration, sizeof(Configuration));
    if (size != sizeof(Configuration))
    {
        QMessageBox::critical(this, "File Read Error", "Cannot write to the file.", QMessageBox::Ok);
        return;
    }
    file.close();
}

void ARelayConfiguratorDialog::mnuSaveAs_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save Configuration File", QString(), "Relay Configuration File (*.rconf);;All Files (*.*)");
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "File Write Error", "Cannot open file for writing.", QMessageBox::Ok);
        return;
    }

    Configuration configuration;
    saveForm(&configuration);

    qint64 size = file.write((char*)&configuration, sizeof(Configuration));
    if (size != sizeof(Configuration))
    {
        QMessageBox::critical(this, "File Read Error", "Cannot write to the file.", QMessageBox::Ok);
        return;
    }
    file.close();

    filename = path;
}

void ARelayConfiguratorDialog::mnuExit_clicked()
{
    close();
}

void ARelayConfiguratorDialog::btnStartBroadcasting_clicked()
{
    if (!broadcastTimer->isActive())
    {

        if (!socket.bind(1089))
        {
            QMessageBox::critical(this, "Network Error", "Cannot bind socket to port 1089.", QMessageBox::Ok);
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
    form->txtWifiSSID->setEnabled(!broadcastTimer->isActive());
    form->txtWifiPassword->setEnabled(!broadcastTimer->isActive());
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

    Configuration packet;
    saveForm(&packet);
    socket.writeDatagram((const char*)&packet, sizeof(Configuration), QHostAddress::Broadcast, 1089);

    ConfigurationBroadcastReplyPacket replyPacket;
    QHostAddress relayIP;

    while (true)
    {
        if (!socket.hasPendingDatagrams())
            break;

        if (socket.readDatagram((char*)&replyPacket, sizeof(ConfigurationBroadcastReplyPacket), &relayIP) != sizeof(ConfigurationBroadcastReplyPacket))
            continue;

        if (replyPacket.header != ConfigurationBroadcastReplyPacketHeader)
            continue;

        QMessageBox::information(this, "Confirmation Received",
            QString("Confirmation packed received from a relay with IP %1. Stopping broadcasting.").arg(relayIP.toString()),
            QMessageBox::Ok);

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

    broadcastTimer = new QTimer(this);
    broadcastTimer->setInterval(500);

    mnuNew_clicked();

    QObject::connect(this->form->btnStartBroadcasting, &QAbstractButton::clicked, this, &ARelayConfiguratorDialog::btnStartBroadcasting_clicked);
    QObject::connect(this->form->txtIP, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->txtSubnet, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->txtGateway, &QLineEdit::textChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->form->chkDHCP, &QCheckBox::stateChanged, this, &ARelayConfiguratorDialog::validate);
    QObject::connect(this->broadcastTimer, &QTimer::timeout, this, &ARelayConfiguratorDialog::broadcast);
    QObject::connect(this->form->mnuNew, &QAction::triggered, this, &ARelayConfiguratorDialog::mnuNew_clicked);
    QObject::connect(this->form->mnuOpen, &QAction::triggered, this, &ARelayConfiguratorDialog::mnuOpen_clicked);
    QObject::connect(this->form->mnuSave, &QAction::triggered, this, &ARelayConfiguratorDialog::mnuSave_clicked);
    QObject::connect(this->form->mnuSaveAs, &QAction::triggered, this, &ARelayConfiguratorDialog::mnuSaveAs_clicked);
    QObject::connect(this->form->mnuExit, &QAction::triggered, this, &ARelayConfiguratorDialog::mnuExit_clicked);

    validate();
}

ARelayConfiguratorDialog::~ARelayConfiguratorDialog()
{
    delete form;
}
