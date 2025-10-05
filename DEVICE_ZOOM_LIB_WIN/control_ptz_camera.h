#ifndef CONTROL_PTZ_H
#define CONTROL_PTZ_H

#include <QObject>
#include "models.h"

class ControlPTZCamera : public QObject
{
    Q_OBJECT

public:
    explicit ControlPTZCamera(QObject *parent = nullptr);
            ~ControlPTZCamera();
    void ConnectToCamera(QString ip, QString port, QString user, QString pass);

public slots:

    void SlotMoveToHome();

    void SlotZoomIn();
    void SlotZoomOut();
    void SlotStopMove();

    void SlotCreatePreset();
    void SlotGoToPreset();
    void SlotGetPreset();

private:
    QString m_userName{""};
    QString m_passWord{""};
    QString m_PTZAddress{""};
    MediaProfile m_selectedMedia;
};

#endif // MAINWINDOW_H
