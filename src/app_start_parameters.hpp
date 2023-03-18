#pragma once
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
namespace fs = filesystem;

#define APP_PORT 5349

struct RawStartArgument {
  int argc;
  char **argv;
};

enum class AppMode { send, receive };
enum class ConnectMode { host, client };

struct AppStartParameters {
  AppMode transfer_mode = AppMode::receive;
  ConnectMode connect_mode = ConnectMode::host;
  int port = APP_PORT;
  string ip;
  vector<fs::path> files;
  fs::path dest_folder;
};

struct FlagArgumentInfo {
  const char *flag;
  int arguments_count;
};

class StartArgumentsPocessor {
private:
  static void SplitFlaggedArguments(RawStartArgument raw_args,
                                    map<string, vector<string>> &flagged_args,
                                    vector<string> &unflagged_args);
  static bool FindFlagInfo(string flag, FlagArgumentInfo &flag_info);
  static void ProcessUnflaggedArguments(AppStartParameters &parameters,
                                        vector<string> &args);
  static void ProcesRecevieDestination(AppStartParameters &parameters,
                                        vector<string> args);
  static void ProcesToSendFiles(AppStartParameters &parameters,
                                 vector<string> args);

public:
  StartArgumentsPocessor() = delete;

  static AppStartParameters ProcessStartArguments(RawStartArgument raw_args);
};
