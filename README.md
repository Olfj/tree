# tree_plus

A replica of the built in `tree` command with some extra options. Default behaviour is to give the entire tree starting from the working directory, excluding files (i.e. the same behaviour as `tree`).

### Usage:

`tree_plus [Options]`

### Options:
```
-path, -P               [path]          Path to the tree root, relative to current directory 
                                        (Default: current directory, ".").

-include_files, -F                      Include files in the tree. 

-exclude, -EX           [[name1] ...]   All folders, file extensions or complete file 
                                        paths to exclude from the tree. Complete file paths should
                                        be given starting from the working directory.

-exclude_sub, -EX_S     [[name1] ...]   All folders or complete file paths that should have their content 
                                        excluded so that the folder will be included but not any of its 
                                        contents. Complete file paths should be given starting from the 
                                        working directory.

-target, -T             [path]          The target path for a txt file containing the directory tree.

-h, --help, -?                          Show help message and exit.
```

### Examples:

```    
C:\...\tree_plus> tree_plus -EX .git build
    .
    ├── include
    └── src
```

```
C:\...\tree_plus> tree_plus -F -EX .git build
    .
    ├── .gitignore
    ├── CMakeLists.txt
    ├── include
    │   ├── args.hpp
    │   ├── chars.hpp
    │   └── filetree.hpp
    ├── README.md
    └── src
        ├── args.cpp
        ├── chars.cpp
        ├── filetree.cpp
        └── main.cpp
```

```
C:\...\tree_plus> tree_plus -F -EX .git build -EX_S src include
    .
    ├── .gitignore
    ├── CMakeLists.txt
    ├── include
    ├── README.md
    └── src
```

### Windows:

To use on Windows, build project, add `tree_plus.exe` to a folder of your choice and add that folder to `PATH`.

Build using `cmake` and `MINGW` from PowerShell.

```
    mkdir build
    cd .\build\
    cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .
```

Then move to a reasonable location, for example:

```
    mkdir $env:USERPROFILE\tree_plus
    cp ./bin/tree_plus.exe $env:USERPROFILE\tree_plus
```

Then add to path 

```
    [Environment]::SetEnvironmentVariable("PATH", $env:PATH + ";$env:USERPROFILE\tree_plus", [EnvironmentVariableTarget]::User)
```

### Linux /MacOS

TODO