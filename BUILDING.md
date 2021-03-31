To build this library you have to enable the following compiler options: position independent code, shared library, (maybe) O1/O2 and set a global preprocessor define,
which depends on your system, for windows use  \_SYSTEM_WINDOWS, for linux \_SYSTEM_LINUX and for OSX \_SYSTEM_MACOSX. These are required for a feature with which this
entire project is buildable on any system with no code changes required.
