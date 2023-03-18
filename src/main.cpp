#include "app_start_parameters.hpp"
#include "application.hpp"
#include "exception.hpp"

int main(int argc, char **argv) {
  try {
    RawStartArgument arguments = {argc, argv};

    Application app(arguments);

    app.Run();
  } catch (IException &e) {
    e.Print();
  }
}
