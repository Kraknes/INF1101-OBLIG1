# wordfreq
Simple command line (CLI) program to list the frequency of words in a given input file.  

# Directory structure
```
p1-pre/
├── data/
│   └── oxford_dict.txt   - Dictionary dataset
├── include/
│   ├── common.h     - Common functionality
│   ├── defs.h       - Various definitions
│   ├── futil.h      - File utilities
│   ├── list.h       - List interface
│   ├── map.h        - Map interface
│   ├── printing.h   - Printing/assertion macros
│   ├── tests.h      - Exposed test functions
│   └── testutil.h   - Utility for implementing tests
├── src/
│   ├── common.c     - Common functionality
│   ├── futil.c      - File utilities
│   ├── linkedlist.c - Linked list implementation
│   ├── map.c        - Map implementation
│   ├── main.c       - The main program
│   ├── testlist.c   - Tests for the list interface
│   ├── testmap.c    - Tests for the map interface
│   └── testutil.c   - Utility for implementing tests
├── README.md        - Details on how to build and run
└── Makefile         - Makefile for the project
```

# Compiling the program
A makefile is provided to simplify compiling the program. The makefile provides two configurations for compiling the program, _debug_ and _release_.  

## _debug_
Compile with `make` or `make exec`.  
By default, the makefile is set to enable compile-time warnings and symbolic information nescessary for program such as gdb and lldb.
This mode is default for a reason. Unless you are certain the program works as intended, utilize this option.

## _release_
If `DEBUG=0` is set in the makefile, or overridden by `make DEBUG=0`, the program will instead be compiled with minimal warnings and a high degree of optimization.  
The compiled executable will be placed in `bin/release` and object dependencies will be placed in `obj/`. 

Building in release-mode also directs the compiler to completely remove:
* All `printing.h` invocations except for `pr_error` and `PANIC`
* All assertions, either through `assert.h` or `printing.h`


# Usage
`./<exec_path> [--tests, --fpath=...]`

### Runtime arguments (`<arguments>`)
Provide one or more. Order of provided arguments does not matter.
* `--tests` - run tests
* `--fpath=...` - specify a file to run the program with

If the `--tests` flag is provided, these are run before any other actions.  
If an `--fpath` argument is provided, the program builds an index on the file before entering a interactive command line interface. Type terms followed by enter to search for their frequency in the given source file.

### Examples
* `./<exec-path> --tests` - run tests before quitting
* `./<exec-path> --fpath=README.md` - run the program with this README as input
* `./<exec-path> --tests --fpath=src/main.c` - run tests, then run the program with its own source file as input


# printing.h
Provides several printf-like macros that may be utilized:
```c
pr_error(fmt, ...)
pr_warn(fmt, ...)
pr_info(fmt, ...)
pr_debug(fmt, ...)
PANIC(fmt, ...)
assert(assertion)
assertf(assertion, fmt, ...)
```

The compile-time definition of `LOG_LEVEL` controls what prints are included in the compiled program:
```c
enum {
    LOG_LEVEL_PANIC = 0, /* minimum log level. only prints on PANIC. */
    LOG_LEVEL_ERROR,     /* enable pr_error */
    LOG_LEVEL_WARN,      /* enable pr_warn (+ pr_error) */
    LOG_LEVEL_INFO,      /* enable pr_info (+ pr_error, pr_warn) */
    LOG_LEVEL_DEBUG      /* enable pr_debug (+ pr_error, pr_warn, pr_info) */
};
```
If no compile-time definition of `LOG_LEVEL` is provided, it defaults to `LOG_LEVEL_DEBUG`.  

The following options are also available:
* remove colors (define `PRINTING_NCOLOR`)
* removal of meta information such as file/line. (define `PRINTING_NMETA`)

If you prefer utilizing typical `printf` calls, you are obviously free to do so. However, having the ability to toggle a category of prints by a single definition; rather than commenting them out, is quite convenient.
