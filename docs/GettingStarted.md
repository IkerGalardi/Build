# Getting started

The main data structure of the build system is `BldProject`, which
represents and stores all the configuration and source files tied to a
executable or library.

In order to define a new project, we will be using the `BldNewProject`
function as seen in the next example:

```c
BldProject *exe = BldNewProject("ProjectName",
                                BLD_EXECUTABLE,
                                BLD_LANGUAGE_DEFAULT);
```

After creating the project, we need to configure it by adding source
files and configure additional include directories if necessary. For
that, we will use the `BldAddSources` and `BldAddIncludePaths`
functions as seen in the next example:

```c
BldAddSources(exe, "src/main.c", "src/utils.c");
BldAddIncludePaths(exe, "include");
```

After configuring the project, we need to call to the `BldGenerate`
function to actually kickstart the project generation. This function
takes a project as an argument, which will be the main target (the
project built after calling `make all` for example).

```c
BldGenerate(exe);
```

Once the build configuration file is created, simply compile and run
the file in order to generate the makefile. We can compile and run the
binary using the next command on unix-like systems:

```bash
cc Build.c -o Build -lBuild && ./Build
```

For this command to work, the library itself needs to be installed on
a path that the dynamic linker knows.

Congrats! You have built your first program with Build.
