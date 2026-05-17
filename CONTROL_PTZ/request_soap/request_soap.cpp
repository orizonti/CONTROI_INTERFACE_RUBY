#include "request_soap.h"
#include <QDateTime>
#include <QTimer>
#include <QtConcurrent>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;


SoapRequest::SoapRequest() : QObject (nullptr)
{
    networkManager = new QNetworkAccessManager();

    QObject::connect(this->networkManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)) , 
                                     this, SLOT  (authRequired(QNetworkReply *, QAuthenticator *)));
}

SoapRequest::~SoapRequest()
{
    delete networkManager;
}

bool SoapRequest::sendRequest(QString &result)
{
    QNetworkRequest request(this->address);
                    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/soap+xml");

              QString concatenated = this->username + ":" + this->password;
                                  QString headerData = "Basic " + concatenated.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    QNetworkReply* reply = this->networkManager->get(request, this->createRequest().toUtf8());

    connect(reply, &QIODevice::readyRead        , [reply,this]() 
        { 
               reply_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                                    FLAG_SUCCESS_REQUEST = false;
            if(reply_status == 200) FLAG_SUCCESS_REQUEST = true;
                                    FLAG_WAIT_REQUEST = false; });

    connect(reply, &QNetworkReply::errorOccurred, [reply,this]() 
        { 
        qDebug() << "[SOAP ERROR ]";
        FLAG_WAIT_REQUEST    = false; 
        FLAG_SUCCESS_REQUEST = false;
        FLAG_ERROR_REQUEST   = true; });

    connect(networkManager, &QNetworkAccessManager::finished, [this,&result](QNetworkReply* reply) 
        { 
        //qDebug() << "[SOAP REQUESET FINISHED ]";
        if(reply->error() <= 0) result = QString(reply->readAll());

        });


    return waitReply();
}

bool SoapRequest::waitReply()
{
               loop.connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(quit()));

         auto result = std::async(std::launch::async,[this]() -> bool 
         {
                FLAG_WAIT_REQUEST= true;
          std::chrono::duration<double,std::milli> duration = 0ms;
      
          while(FLAG_WAIT_REQUEST) 
          {
               duration += 2ms;   std::this_thread::sleep_for(2ms); 
            if(duration > 60ms) { FLAG_WAIT_REQUEST = false; this->loop.quit(); return false; }
          }
      
          return true;
         });

             loop.exec(); 

     return result.get();
}

void SoapRequest::authRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    authenticator->setUser(this->username);
    authenticator->setPassword(this->password);
}

QString SoapRequest::createRequest()
{
    QString request("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            request.push_back("<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\"");

    for (int i = 0; i < this->XMLNs.size(); i++) request.push_back(" " + this->XMLNs[i]);
                                                 request.push_back(">");
    if (this->username != "" || this->action != "")
    {
        request.push_back("<s:Header>");
        if (this->action != "")
        {
            request.push_back("<Action mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">");
            request.push_back(this->action);
            request.push_back("</Action>");
        }

        if (this->username != "") request.push_back(this->createUserToken());
                                  request.push_back("</s:Header>");
    }
    request += "<s:Body>" + this->body + "</s:Body>";
    request += "</s:Envelope>";
    return request;
}

QString SoapRequest::createUserToken()
{
                QUuid nonce = QUuid::createUuid();
    QString nonce64 = nonce.toByteArray().toBase64();
    auto timestamp = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    auto token = nonce.toString() + timestamp + this->password;

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(token.toUtf8());
    QString hashTokenBase64 = hash.result().toBase64();

    QString result("<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">");
    result.push_back("<UsernameToken><Username>");
    result.push_back(this->username);
    result.push_back("</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">");
    result.push_back(hashTokenBase64);
    result.push_back("</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">");
    result.push_back(nonce64);
    result.push_back("</Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">");
    result.push_back(timestamp);
    result.push_back("</Created></UsernameToken></Security>");
    return result;
}
