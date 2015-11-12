=================
  Dependencies
=================

To build batajelo, please install the dependencies specified in the
install.* files.

The following libraries, that may or may not be packaged for your system,
are also required:
- litesql               https://dev.louiz.org/projects/litesql/
- SFML 2.1              http://www.sfml-dev.org/


=================
     CMake
=================

To configure the project, check for dependencies and create the Makefile, you need cmake, and use it like this:

$ cmake .

To compile only the server (for example if you want to run batajelo_server
on a server, without having to install the graphical dependencies), run

$ cmake . -DBUILD_CLIENT=false

You can also only build the client if you want, but this doesn’t avoid any
dependency, it just avoid compiling a few files.

$ cmake . -DBUILD_SERVER=false

Once this is done, with no error, you can compile the project:

$ make