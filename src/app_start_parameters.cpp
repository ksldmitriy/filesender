#include "app_start_parameters.hpp"
#include "exception.hpp"

const FlagArgumentInfo flags_info[]{{"-s", 0}, {"-r", 0}, {"-a", 1}, {"-p", 1}};

AppStartParameters
StartArgumentsPocessor::ProcessStartArguments(RawStartArgument raw_args) {
  map<string, vector<string>> flagged_args;
  vector<string> unflagged_args;
  SplitFlaggedArguments(raw_args, flagged_args, unflagged_args);

  AppStartParameters parameters;

  for (auto &arg : flagged_args) {
    string flag = arg.first;
    vector<string> &params = arg.second;

    if (flag == "-s") {
      parameters.transfer_mode = AppMode::send;
    } else if (flag == "-r") {
      parameters.transfer_mode = AppMode::receive;
    } else if (flag == "-a") {
      parameters.connect_mode = ConnectMode::client;
      parameters.ip = params[0];
    } else if (flag == "-p") {
      parameters.port = stoi(params[0]);
    }
  }

  ProcessUnflaggedArguments(parameters, unflagged_args);

  return parameters;
}

void StartArgumentsPocessor::ProcessUnflaggedArguments(
    AppStartParameters &parameters, vector<string> &args) {
  if (parameters.transfer_mode == AppMode::receive) {
    ProcesRecevieDestination(parameters, args);
  } else {
    ProcesToSendFiles(parameters, args);
  }
}

void StartArgumentsPocessor::ProcesRecevieDestination(
    AppStartParameters &parameters, vector<string> args) {
  if (args.size() > 1) {
    string message;
    message += to_string(args.size());
    message += "destination folders set";
    ArgumentsException e(message);
    throw e;
  }

  if (args.size() == 0) {
    parameters.dest_folder = ".";
  } else if (args.size() == 1) {
    parameters.dest_folder = args[0];
  }

  if (!fs::exists(parameters.dest_folder)) {
    ArgumentsException e(string("destination folder \"") +
                         parameters.dest_folder.string() + "\" doesnt exits");
    throw e;
  }
}

void StartArgumentsPocessor::ProcesToSendFiles(AppStartParameters &parameters,
                                               vector<string> args) {
  for (string &f : args) {
    if (!fs::exists(f)) {
      ArgumentsException e(f + " doesn exist");
      throw e;
    }

    parameters.files.push_back(f);
  }
}

void StartArgumentsPocessor::SplitFlaggedArguments(
    RawStartArgument raw_args, map<string, vector<string>> &flagged_args,
    vector<string> &unflagged_args) {
  flagged_args.clear();
  unflagged_args.clear();

  // conver args to string
  vector<string> args;
  for (int f = 1; f < raw_args.argc; f++) {
    args.push_back(raw_args.argv[f]);
    cout << raw_args.argv[f] << endl;
  }

  // split arguments
  for (int f = 0; f < args.size(); f++) {
    FlagArgumentInfo flag_info;
    bool is_flag = FindFlagInfo(args[f], flag_info);

    if (is_flag) {
      f++;
      vector<string> parameters;
      for (int i = 0; i < flag_info.arguments_count; i++, f++) {
        if (f >= args.size()) {
          cout << "invalid args count" << endl;
          // error
        }

        parameters.push_back(args[f]);
      }
      flagged_args[flag_info.flag] = parameters;
      f--;
    } else {
      unflagged_args.push_back(args[f]);
    }
  }
}

bool StartArgumentsPocessor::FindFlagInfo(string flag,
                                          FlagArgumentInfo &flag_info) {
  for (int f = 0; f < sizeof(flags_info) / sizeof(*flags_info); f++) {
    if (flags_info[f].flag == flag) {
      flag_info = flags_info[f];
      return true;
    }
  }

  return false;
}
