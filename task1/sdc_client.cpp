#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"

namespace po = boost::program_options;
using namespace std;

void require(int option, string msg) {
  if(!option) {
    cout << msg << endl;
    exit(1);
  }
}

int main(int argc, char** argv) {
  Ice::CommunicatorPtr ic;
  ic = Ice::initialize(argc, argv);

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

  require(vm.count("server"), "please specify a server");
  require(vm.count("port"), "please specify a port");
  require(vm.count("certificate-path"), "please specify the path to a certificate");

  try {
    Ice::PluginManagerPtr pluginMgr = ic->getPluginManager();
    Ice::PluginPtr plugin = pluginMgr->getPlugin("IceSSL");
    IceSSL::PluginPtr sslPlugin = IceSSL::PluginPtr::dynamicCast(plugin);


    boost::format connection_string = boost::format("Authentication:ssl -h %1% -p %2%");
    string server = vm["server"].as<string>();
    string port = vm["port"].as<string>();

    Ice::ObjectPrx base = ic->stringToProxy(
      (connection_string % server % port).str() );
    sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);
    cout << auth->echo("hello world") << endl;
  } catch (const Ice::Exception& e) {
    std::cerr << e << std::endl;
  }
  if (ic) ic->destroy();
  return EXIT_SUCCESS;

}
