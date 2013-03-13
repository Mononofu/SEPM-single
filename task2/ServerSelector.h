#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <QObject>
#include <QString>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>

#include "Chat.h"

class ServerSelector : public QObject
{
  Q_OBJECT
public:
  ServerSelector(QDeclarativeView *view, QObject *parent = 0)
    : QObject(parent), view(view), chat(NULL) { }

  Q_INVOKABLE QString echo(const QString &text) const {
    if(!chat) return QString("no server set");
    string reply = chat->echo(text.toUtf8().constData());
    return QString::fromStdString(reply);
  }

  Q_INVOKABLE bool setServer(const QString &server, const QString &port, const QString &cert) {
    if(server == this->server && port == this->port && cert == this->cert)
      return chat != NULL;

    this->server = server;
    this->port = port;
    this->cert = cert;

    if(chat)
      delete this->chat;

    try {
      this->chat = new Chat(server.toUtf8().constData(),
                    port.toUtf8().constData(),
                    cert.toUtf8().constData());
    } catch (const Ice::Exception& e) {
      this->chat = NULL;
      return false;
    }
    return true;
  }

  Q_INVOKABLE void reload() const {
    QMetaObject::invokeMethod(const_cast<ServerSelector*>(this), "open", Qt::QueuedConnection,
      Q_ARG(QUrl, QUrl("./ui/ui.qml")));
  }

public slots:
  void open(const QUrl &url) {
    view->engine()->clearComponentCache();
    view->setSource(url);
    view->setGeometry(100, 100, 800, 480);
    view->show();
  }

  void open(QDeclarativeView*& view, const QUrl &url) {
    view->engine()->clearComponentCache();
    view->setSource(url);
    view->setGeometry(100, 100, 800, 480);
    view->show();
  }

protected:
  QDeclarativeView *view;
  Chat* chat;
  QString server;
  QString port;
  QString cert;
};

#endif // STRINGHELPER_H
