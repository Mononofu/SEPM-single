#include <string>
#include <QtDeclarative/QDeclarativeView>
#include <QObject>
#include "NotifyPipe.h"
#include <QFuture>

using namespace std;

// inotify based class to watch a specific directory for changes
class FileWatcher : public QObject {
  Q_OBJECT

public:
  // destruction automatically stops watching
  ~FileWatcher() {
    stop();
  }
  // sets path for view to be reloaded on change
  void addWatch(QDeclarativeView *view, QString layout) {
    views[layout] = view;
  }
  // start watching a given directory, calling refreshView on all views added
  // using "addWatch" every time a file in that directory is changed
  void start(string dir);
  void stop();

public slots:
  // helper method to refresh a view, as that needs to happen on the UI thread
  void refreshView(QDeclarativeView* view, const QUrl &url);

private:
  void watch(string dir);

  QMap<QString, QDeclarativeView*> views;
  NotifyPipe pipe;
  QFuture<void> future;
  bool running;
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
