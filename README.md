Utility Tool Library for C++
============================

The goal of this library is to provide some common utilities. It is
designed for my own personal use but others may use it, too.

Current Features
----------------

  * Argument parser

  * Simple Logging API

To-do
-----

  * Implement a more generic version of
    `utl::Arguments::getNextArgument()`. The function should accept a
    second parameter which specifies a reader.

  * Don't use the legacy `printf` functions to format log messages. Use
    a type safe implementation with templates.

  * Implement configurations for the logging API.

Argument parser
---------------

The argument parser is a single class (`utl::Arguments`) which can parse
GUN style arguments from the command line.

```{.cpp}
#define OPT_DEBUG 256

int main(int argc, char *argv[])
{
    Arguments args(argc, argv);
    args.registerOption("--quiet", 'q');
    args.registerOption("--silent", 'q');
    args.registerOption("--debug", OPT_DEBUG);

    std::string inputFile = "-";
    std::string outputFile = "-";
    bool debug = false;
    bool quiet = false;

    while (int opt = args.getNextOption()) {
        switch (opt) {
        case 'o':
            if (!args.getNextArgument(outputFile)) {
                cerr << "Missing argument for " << args.getOptionName()
                     << "." << endl;
                return EXIT_FAILURE;
            }
            break;
        case 'q':
            quiet = true;
            break;
        case OPT_DEBUG:
            debug = true;
            break;
        case -2:
            cerr << "Ambiguous option: " << args.getOptionName() << endl;
            return EXIT_FAILURE;
        default:
            cerr << "Unknown option: " << args.getOptionName() << endl;
            return EXIT_FAILURE;
        }
    }

    if (args.getArgumentsLeft() <= 1) {
        cerr << "Invalid amount of arguments." << endl;
        return EXIT_FAILURE;
    }
    args.getNextArgument(inputFile);

    // ...
}
```

The function `utl::Arguments::getNextArgument()` is a little bit more
flexible. Instead of reading strings, you can also read an int for
example:

```{.cpp}
int arg;
args.getNextArgument(arg);
```

The function will throw an exception if the argument is not an integer.
You can pass any type which implements the `<<`-operator for input
streams.

Logging API
-----------

To initialize the logging API, you may want to add some handlers.  A
Handler is a class derived from `utl::LogHandler`. It decides what do
with the messages. The class `utl::ConsoleLogHandler` is currently the
only implementation in this library. But you can add your own
implementation as well. The following code shows how you could
initialize your logging API:

```{.cpp}
#include <memory>

#include <utl/ConsoleLogHandler.h>
#include <utl/Logger.h>

int main(int argc, char *argv[])
{
    Logger::getRoot().addHandler(std::make_shared<ConsoleLogHandler>());

    // ...
}
```

After this, you can use the API like this:

```{.cpp}
#define UTL_LOGGER somelogger
#include <utl/logging.h>

void someFunction()
{
    utl::fine("someFunction called.");
    // ...
    bool couldOpen;
    std::string file;
    // ...
    if (!couldOpen)
        utl::severe("Could not open file: %s", file.c_str());
}
```

The macro `UTL_LOGGER` sets the logger which is used in this file. You
can get the instance of it with `utl::Logger::get()`. Every logger
created with this function has the root logger as parent. This mean
every message is (also) handelt by our `ConsoleLogHandler`.
