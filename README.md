# tree_plus

A replica of the built in `tree` command with some extra options. Default behaviour is to give the entire tree, including hidden folders and excluding files, starting from the working directory. The program has no special handling of symlinks, junctions, restricted folders, etc. so running this on your entire C: drive will most likely fail. The intended use is to quickly get an overview of a small file tree. 


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

No arguments 
```
PS C:\...\tree> tree_plus
    .
    ├── .git
    │   ├── hooks
    │   ├── info
    │   ├── logs
    │   │   └── refs
    │   │       ├── heads
    │   │       │   ├── 7-feature
    │   │       │   └── feature
    │   │       └── remotes
    │   │           └── origin
    │   │               ├── 11-feature
    │   │               ├── 7-feature
    │   │               └── feature
    │   ├── objects
    .
    .
    .

    │           │   └── tmp
    │           └── CMakeFiles
    │               ├── 4.1.1
    │               ├── catch2-populate.dir
    │               └── pkgRedirects
    ├── include
    ├── src
    └── tests
```

Excluding .git, .vscode and build to make it clearer.

```    
PS C:\...\tree> tree_plus -EX .git .vscode build
    .
    ├── .github
    │   ├── ISSUE_TEMPLATE
    │   └── workflows
    ├── include
    ├── src
    └── tests
```

Including files in the tree.

```
PS C:\...\tree> tree_plus -F -EX .git .vscode build
    .
    ├── .clang-tidy
    ├── .github
    │   ├── ISSUE_TEMPLATE
    │   │   ├── bug_report.md
    │   │   └── feature_request.md
    │   ├── PULL_REQUEST_TEMPLATE.md
    │   └── workflows
    │       └── ci.yml
    ├── .gitignore
    ├── CMakeLists.txt
    ├── include
    │   ├── args.hpp
    │   ├── chars.hpp
    │   └── filetree.hpp
    ├── README.md
    ├── src
    │   ├── args.cpp
    │   ├── chars.cpp
    │   ├── filetree.cpp
    │   └── main.cpp
    └── tests
        ├── CMakeLists.txt
        ├── test_filetree.cpp
        ├── test_platform_linux.cpp
        └── test_platform_windows.cpp
```

Excluding the contents of tests, src and include.

```
PS C:\...\tree> tree_plus -F -EX .git .vscode build -EX_S src tests .github include
    .
    ├── .clang-tidy
    ├── .github
    ├── .gitignore
    ├── CMakeLists.txt
    ├── include
    ├── README.md
    ├── src
    └── tests
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

The tree command in Linux/MacOS apparently already supports exclusion of files and folders from `tree`. Just google how to use that properly instead. 