#ifndef DEVICE_ZOOM_ROTATION_H
#define DEVICE_ZOOM_ROTATION_H

#include <QString>
#include <QList>
#include <QObject>

#include "./request_soap/request_soap.h"
#include "device_generic_interface.h"
#include <QTimer>

class DeviceZoomRotation: public QObject, public DeviceGenericHandleControl
{
  Q_OBJECT
private:
    static QList<QString> onvif_schemas;
public:
    DeviceZoomRotation();
    DeviceZoomRotation(QString user, QString pass, QString ip, QString port, QObject* parent = nullptr);
    std::vector<float> PositionsZoom{0.10,0.70,0.80,0.85,0.90,0.95,0.98};

    void connectToDevice(QString user, QString pass, QString ip, QString port);

    public:
    QString TAG_NAME{"[ZOOM_CONTROL]"};

	  void setParam (uint16_t CommandID, float    CommandParam)  override ;
	  void setEnable(bool OnOff        , uint16_t Number = 0) override { if(!OnOff) slotStop(); };

    SoapRequest requestStop;
    SoapRequest requestMove;
    SoapRequest requestSetHome;
    SoapRequest requestSetPreset;
    SoapRequest requestGoPreset;
    SoapRequest requestGetPreset;
    SoapRequest requestGoHome;
    SoapRequest requestGetStatus;

    SoapRequest requestMoveAbs;

    QString profile_token;
    QString response;

    int posStep = 0;
    int timePeriodStep = 1200;

    public slots:
    void slotContinuousMove(double x, double y, double z);
    void slotAbsoluteMove(double x, double y, double z);

    void slotMovePosTiming(int pos);
    void slotMovePos(int pos);
    void slotMoveStep();
    void slotMoveLimit();

    void slotZoomIn();
    void slotZoomOut();

    void slotStop();
    void slotGoToHomePosition();
    void slotSetHomePosition();
    void slotSetPreset();
    void slotGotoPreset();
    void slotGetPreset();
    void slotGetStatus();
private:
};

#endif // DeviceZoomRotation_H
