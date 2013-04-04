#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>

#include "Chat.h"

// backing class for QML view, handles events and calls to Ice
class ServerSelector : public QObject
{
  Q_OBJECT
public:
  ServerSelector(QDeclarativeView *view, QObject *parent = 0)
    : QObject(parent), view(view), chat(NULL) { }

  // echo string over Ice
  Q_INVOKABLE QString echo(const QString &text) const {
    if(!chat) return QString("no server set");
    string reply = chat->echo(text.toUtf8().constData());
    return QString::fromStdString(reply);
  }

  // open native file dialog to browse for cert file
  Q_INVOKABLE QString browse() const {
      return QFileDialog::getOpenFileName(view, tr("Open File"), cert, tr("Certificates (*.crt);;All files (*.*)"));
  }

  // create Chat object / connection using ice with values set in GUI
  Q_INVOKABLE QString setServer(const QString &server, const QString &port, const QString &cert) {
    if(chat != NULL && server == this->server && port == this->port && cert == this->cert)
      return "";

    this->server = server;
    this->port = port;
    this->cert = cert;

    if(chat)
      delete this->chat;

    this->chat = NULL;
    try {
      this->chat = new Chat(server.toUtf8().constData(),
                    port.toUtf8().constData(),
                    cert.toUtf8().constData());
    } catch (const Ice::ConnectTimeoutException& e) {
      return "timeout while establishing connection - check server and port";
    } catch (const Ice::PluginInitializationException& e) {
      return "failed to initialize SSL plugin - are you using the correct certificate?";
    } catch (const Ice::EndpointParseException& e) {
      return QString::fromStdString("Failed to create endpoint, check server and port: \n  " + e.str);
    } catch (const Ice::DNSException& e) {
      return QString::fromStdString("error resolving hostname: " + e.host);
    } catch (const Ice::Exception& e) {
      return QString::fromStdString(e.what());
    }

    return "";
  }

protected:
  QDeclarativeView *view;
  Chat* chat;
  QString server;
  QString port;
  QString cert;
};

#endif // STRINGHELPER_H
