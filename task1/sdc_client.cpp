#include <boost/program_options.hpp>
#include <Ice/Ice.h>

namespace po = boost::program_options;
using namespace std;

int main(int argc, char** argv) {
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

}
