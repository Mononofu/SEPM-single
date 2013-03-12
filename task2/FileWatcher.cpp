#include "FileWatcher.h"
#include <sys/inotify.h>
#include <iostream>
#include <unistd.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void FileWatcher::watch(string dir) {
  fd = inotify_init();
  if(fd < 0)
    throw WatchException("failed to init inotify");

  wd = inotify_add_watch( fd, dir.c_str(),
                         IN_MODIFY | IN_CREATE | IN_DELETE );

  char buffer[BUF_LEN];
  while(true) {
    int length = read( fd, buffer, BUF_LEN );
    if(length < 0) {
       //throw WatchException("failed to read events");
    } else if(running) {
       QMetaObject::invokeMethod(selector, "open", Qt::QueuedConnection, Q_ARG(QUrl, QUrl("./ui/ui.qml")));
    }
  }

}

void FileWatcher::cleanup() {
  inotify_rm_watch(fd, wd);
}
