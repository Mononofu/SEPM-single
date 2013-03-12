#include <boost/program_options.hpp>
#include "ServerSelector.h"
#include "Chat.h"
#include "FileWatcher.h"

#include <QApplication>
#include <QMainWindow>
#include <QtCore>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <Ice/Ice.h>


namespace po = boost::program_options;
using namespace std;

void require(int option, string msg) {
  if(!option) {
    cout << msg << endl;
    exit(1);
  }
}

int main(int argc, char** argv) {
  // Get the initialized property set.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("server,s", po::value<string>(), "set server name")
    ("port,p", po::value<string>(), "set port")
    ("certificate-path,c", po::value<string>(), "set path to certificate")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  if(!vm.count("server") && !vm.count("port") && !vm.count("certificate-path")) {
    QApplication a(argc, argv);

    QMainWindow *window = new QMainWindow();
    QDeclarativeView *view = new QDeclarativeView(window);
    ServerSelector serverSelector(view);

    FileWatcher watcher(&serverSelector);
    QThread *workerThread = new QThread();

    QObject::connect(workerThread, SIGNAL(started()), &watcher, SLOT(doWork()));
    watcher.moveToThread(workerThread);

    // Starts an event loop, and emits workerThread->started()
    workerThread->start();

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty("ServerSelector", &serverSelector);
    view->setSource(QUrl("./ui/ui.qml"));

    window->setGeometry(100, 100, 800, 180);
    window->setCentralWidget(view);
    window->show();

    int r = a.exec();
    watcher.running = false;
    workerThread->terminate();
    watcher.cleanup();
    return r;
  }

  require(vm.count("server"), "please specify a server");
  require(vm.count("port"), "please specify a port");
  require(vm.count("certificate-path"), "please specify the path to a certificate");


  string server = vm["server"].as<string>();
  string port = vm["port"].as<string>();
  string cert_path = vm["certificate-path"].as<string>();

  try {
    Chat c = Chat(server, port, cert_path);
    cout << c.echo("Hello world") << endl;
  } catch (const Ice::Exception& e) {
    std::cerr << "Failed to connect to server: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;

}
