#ifndef CHAT_H
#define CHAT_H

#include <boost/format.hpp>
#include <Ice/Ice.h>
#include <IceSSL/IceSSL.h>
#include "SecureDistributedChat.h"

using namespace std;

class Chat {
public:
  Chat(string server, string port, string cert_path) {
    boost::format connection_string = boost::format("Authentication:ssl -h %1% -p %2%");
    int argc = 1;
    char *argv[] = {"sdc_client"};
    Ice::PropertiesPtr props = Ice::createProperties(argc, argv);
    try {
      props->setProperty("Ice.Plugin.IceSSL", "IceSSL:createIceSSL");
      props->setProperty("IceSSL.CertAuthFile", cert_path);
      props->setProperty("Ice.Override.ConnectTimeout", "2000");

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

  ~Chat() {
    if (ic) ic->destroy();
  }


  string echo(string s) {
    return auth->echo(s);
  }

private:
  Ice::CommunicatorPtr ic;
  sdc::AuthenticationIPrx auth;
};

#endif
