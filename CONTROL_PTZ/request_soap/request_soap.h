#ifndef SOAPREQUEST_H
#define SOAPREQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUuid>
#include <QAuthenticator>
#include <atomic>
#include <QThread>
#include <QElapsedTimer>
#include <QFuture>
#include <QEventLoop>

class SoapRequest : QObject
{
    Q_OBJECT
public:
     SoapRequest();
    ~SoapRequest();

    bool sendRequest(QString &result);
public slots:
    void authRequired(QNetworkReply *reply, QAuthenticator *authenticator);
public:
    QString username; 
    QString password; 
    QString address;

    QString body; 
    QString action;

    QList<QString> XMLNs;

    QNetworkAccessManager *networkManager;

    std::atomic<bool> FLAG_WAIT_REQUEST{false};
    std::atomic<bool> FLAG_SUCCESS_REQUEST{false};
    std::atomic<bool> FLAG_ERROR_REQUEST{false};
    QString reply_result;
        int reply_status;

    QEventLoop loop;

    bool waitReply();
    QElapsedTimer timeMeasure;

private:
    QString createRequest();
    QString createUserToken();
};

#endif // SOAPREQUEST_H
