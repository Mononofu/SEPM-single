#include <string>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QObject>
#include "ServerSelector.h"

using namespace std;

class FileWatcher : public QObject {
  Q_OBJECT

public slots:
  void doWork() {
    running = true;
    watch("./ui");
  }

public:
  FileWatcher(ServerSelector *selector) : selector(selector) { }
  void addWatch(QDeclarativeView *view, QString layout) {
    views[layout] = view;
  }
  void watch(string dir);
  void cleanup();
  bool running;

private:
  QMap<QString, QDeclarativeView*> views;
  ServerSelector *selector;
  int wd;
  int fd;
};

class WatchException : exception {
public:
  WatchException(string reason) : reason(reason) { }
  const char* what() const throw() {
    return reason.c_str();
  }
  ~WatchException() throw() { }
private:
  string reason;
};
