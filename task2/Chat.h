#ifndef CHAT_H
#define CHAT_H

#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"

using namespace std;

// wrapper for Ice boilerplate
class Chat {
public:
  // constructor tries to set up connection to server, throws if it fails
  Chat(string server, string port, string cert_path) {
    boost::format connection_string = boost::format("Authentication:ssl -h %1% -p %2%");
    int argc = 1;
    char prog_name[] = "sdc_client";
    char *argv[] = { prog_name };
    Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
    try {
      // enable the SSL plugin for secure connections
      props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
      props->setProperty("IceSSL.CertAuthFile", cert_path);
      // necessary so invalid connections will time out
      props->setProperty("Ice.Override.ConnectTimeout", "1000");

      // Initialize a communicator with these properties.
      Ice::InitializationData id;
      id.properties = props;
      ic = Ice::initialize(id);

      Ice::ObjectPrx base = ic->stringToProxy(
        (connection_string % server % port).str() );

      auth = sdc::AuthenticationIPrx::checkedCast(base);
    } catch (const Ice::Exception& e) {
      if (ic) ic->destroy();
      throw;
    }
  }

  // automatically close connection on destruction
  ~Chat() {
    if (ic) ic->destroy();
  }

  // wrapper for echo method
  string echo(string s) {
    return auth->echo(s);
  }

private:
  Ice::CommunicatorPtr ic;
  sdc::AuthenticationIPrx auth;
};

#endif
