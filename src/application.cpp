#include "application.hpp"

Application::Application(RawStartArgument raw_argument){
  AppStartParameters start_parameters =
      StartArgumentsPocessor::ProcessStartArguments(raw_argument);

  CreateConnectionProvider(start_parameters);
  CreateFileTransfer(start_parameters);
}

void Application::Run(){
  file_transfer->Run();
}

void Application::CreateConnectionProvider(AppStartParameters &start_parameters){
  if (start_parameters.connect_mode == ConnectMode::host) {
    connection_provider = unique_ptr<ConnectionProrived>(
        new HostConnectionProvider(start_parameters.port));
  } else {
    connection_provider =
        unique_ptr<ConnectionProrived>(new ClientConnectionProvider(
            start_parameters.ip, start_parameters.port));
  }
}

void Application::CreateFileTransfer(AppStartParameters &start_parameters){
  if (start_parameters.transfer_mode == AppMode::receive) {
    file_transfer =
        unique_ptr<IFileTransfer>(new FileReceiver(connection_provider.get()));
  } else {
    file_transfer = unique_ptr<IFileTransfer>(
        new FileSender(start_parameters.files, connection_provider.get()));
  }
}
