#include "app_start_parameters.hpp"
#include "exception.hpp"
#include "file_receiver.hpp"
#include "file_sender.hpp"
#include "file_transfer.hpp"

using namespace std;

class Application {
private:
  unique_ptr<ConnectionProrived> connection_provider;
  unique_ptr<IFileTransfer> file_transfer;

  void CreateConnectionProvider(AppStartParameters &start_parameters);
  void CreateFileTransfer(AppStartParameters &start_parameters);
public:
  Application(RawStartArgument raw_arguments);
  void Run();
};
