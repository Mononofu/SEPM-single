#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"

namespace po = boost::program_options;
using namespace std;

void require(int option, string msg) {
  if(!option) {
    cerr << msg << endl;
    exit(1);
  }
}

int main(int argc, char** argv) {
  // Get the initialized property set.
  Ice::PropertiesPtr props = Ice::createProperties(argc, argv);

  // initialize command line option parsing
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("server,s", po::value<string>(), "set server name")
    ("port,p", po::value<string>(), "set port")
    ("certificate-path,c", po::value<string>(), "set path to certificate")
  ;

  // try to actualize parse the options
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch(po::unknown_option &ex) {
    cerr << "sdc_client: " << ex.what() << endl;
    cerr << "Try `sdc_client --help' for more information." << endl;
    return 1;
  }

  if(vm.count("help")) {
    cout << desc << endl;
    return 1;
  }

  require(vm.count("server"), "please specify a server");
  require(vm.count("port"), "please specify a port");
  require(vm.count("certificate-path"), "please specify the path to a certificate");

  // get values out of the options
  boost::format connection_string = boost::format("Authentication:ssl -h %1% -p %2%");
  string server = vm["server"].as<string>();
  string port = vm["port"].as<string>();
  string cert_path = vm["certificate-path"].as<string>();

  // try to connect to the server using Ice
  Ice::CommunicatorPtr ic;
  try {
    // enable the SSL plugin for secure connections
    props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
    props->setProperty("IceSSL.CertAuthFile", cert_path);
    // necessary so invalid connections will time out
    props->setProperty("Ice.Override.ConnectTimeout", "1000");

    // Initialize a communicator with these properties.
    Ice::InitializationData id;
    id.properties = props;
    Ice::CommunicatorPtr ic = Ice::initialize(id);

    // call echo method
    Ice::ObjectPrx base = ic->stringToProxy(
      (connection_string % server % port).str() );
    sdc::AuthenticationIPrx auth = sdc::AuthenticationIPrx::checkedCast(base);
    cout << auth->echo("hello world") << endl;
    // catch all manners of error cases
  } catch (const Ice::ConnectTimeoutException& e) {
    cerr << "timeout while establishing connection - check server and port" << endl;
  } catch (const Ice::PluginInitializationException& e) {
    cerr << "failed to initialize SSL plugin - are you using the correct certificate?" << endl;
  } catch (const Ice::EndpointParseException& e) {
    cerr << "Failed to create endpoint, check server and port: \n  " << e.str << endl;
  } catch (const Ice::DNSException& e) {
    cerr << "error resolving hostname: " << e.host << endl;
  } catch (const Ice::Exception& e) {
    cerr << e << endl;
  }


  if (ic) ic->destroy();
  return EXIT_SUCCESS;

}
