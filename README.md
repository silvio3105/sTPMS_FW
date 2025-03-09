
# About

This repo contains a template application projects(bootloader/firmware). Template includes:
- Build template for multiplayer Make files
- [Simple Debug](https://github.com/silvio3105/sDebug) and RTT libraries
- RTX5 files with CMSIS-RTOS2 API
- Readme file
- License file
- Git ignore file
- Project folder structure


# Project folder structure

- 📂 **{Project_name}**: Root folder.
    - 📂 **.builds**: Folder with compiler and linker files from building process.
        - 📂 **{Build_name}**: Folder for build type.
    - 📂 **.git**: Git folder.
    - 📂 **.jlink**: Folder with J-Link scripts for flash and erase.
    - 📂 **.outputs**: Folder with build executables(BIN and HEX files + MAP file).
    - 📂 **.releases**: Folder with stable releases(one release per folder, BIN and HEX files + MAP file).
        - 📂 **RC**: Folder with release candidate releases(one release per folder, BIN and HEX files + MAP file).
    - 📂 **.vscode**: Folder with VS Code config files.
        - c_cpp_properties.json: C/C++ config.
        - launch.json: File with configs for debugging.
    - 📂 **Application**: Folder with application layer source files.
        - 📂 **Inc**: Folder with application layer header files.
            - main.h: Legacy main header file.
            - Main.hpp: Main header file.
        - Main.cpp: Main source file with application entry point.
    - 📂 **Builds**: Folder with Make file for each build type.
        - {Build_name}.mk: Make file for build type.
    - 📂 **CMSIS**: Folder with CMSIS-related files.
    - 📂 **Config**: Folder with application configuration files.
        - AppConfig.hpp: Header file with application config(common to all hardware builds).
        - AppConfig.mk: Make file with application build config(common to all hardware builds).
    - 📂 **Documentation**: Folder with application documentation generated with Doxygen and files used for documentation.
    - 📂 **Drivers**: Folder with driver source files.
        - 📂 **Inc**: Folder with driver header files.
    - 📂 **Hardware**: Folder with application-related hardware config header files and MCU SDK files.
        - 📂 **{HW_name}**: Folder with MCU files.
            - 📂 **Inc**: Folder with MCU SDK header files.
            - 📂 **Linker**: Folder with MCU linker script files. 
            - 📂 **Src**: Folder with MCU SDK source files. 
            - 📂 **Startup**: Folder with MCU startup files.
            - 📂 **SVD**: Folder with MCU system view description file. 
            - {HW_name}.mk: Make file for this hardware build.
            - {HW_name}.hpp: Header file with hardware build config.
    - 📂 **Libraries**: Folder with library source files.
        - 📂 **Inc**: Folder with library header files.
    - 📂 **Make**: Folder with Make files.
        - Backend.mk: File with build process and Make actions, shared across all builds.
        - Config.mk: File with Make config, shared across all builds.
    - 📂 **Modules**: Folder with application modules source files.
        - 📂 **Inc**: Folder with application modules header files.
    - 📂 **RTOS**: Folder with RTOS-related files.
        - 📂 **{RTOS_name}**: RTOS name(eg., RTX5, FreeRTOS).
          	- 📂 **Inc**: Folder with RTOS header files.
            - 📂 **IRQ**: Folder with RTOS IRQ files.  
    - .gitignore: List of items for Git to ignore.
    - BatchBuild.bat: Batch script file for batch build.
    - Doxyfile: Doxygen project file.
    - LICENSE: Project license.
    - README.md: Project readme file.


# Build

ARM-GCC and Make are used for building the project. To build the project, type `make -f Builds/{Build_name}.mk` into terminal(CMD or PowerShell). Each build has its own build folder in `.builds` folder. Adding `-j{X}`, where `{x}` is number of jobs to create(~1.5 * number of CPU threads), will speed up the build process(eg., `make -f Builds/{Build_name}.mk -j48`). Every build will create `.bin` and `.hex` files(a.k.a. application executables) and copy them together with build `.map` file to `.outputs` folder.

It's possible to add different options to `make` command. Supported options are:
- `flash`: Flash the application onto MCU.
- `erase`: Erase application part in MCU flash.
- `erase_all`: Erase whole MCU flash.
- `reset`: Reset the MCU.
- `clean`: Delete the build folder from `.builds`, J-Link script files from `.jlink` folder and application executables from `.output` folder.
- `clean_jlink`: Delete J-Link script files from `.jlink` folder.
- `rtos_cfg`: Open the RTOS config file in CMSIS Config Wizard Java application.

### Structure

- **Build file**: Entry make file. Contains build specific config(eg., disabled debug, different optimization). Located in `Builds` folder.
- **Hardware file**: Make file with hardware configuration. Contains hardware-related files, defines etc.. Shared between all builds for same hardware. Located in `Hardware/{HW_name}` folder.
- **Application file**: Make file with application configuration. Contains application-related files, defines etc.. Shared between all builds. Located in `Config` folder.
- **Backend**: Make file with build process. Contains options for `make` command. Located in `Make` folder. Shared between all builds and does not contain application or hardware configuration.
- **Config**: Make file with configuration for build process. Located in `Make` folder. Shared between all builds and does not contain application or hardware configuration.

### Batch build

To do batch build, execute `BatchBuild.bat` batch script by typing `&.\BatchBuild.bat` to terminal.
To add new builds to batch build script, write make command as described above.




# License

Copyright (c) 2025, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all functional textual files.

---

Copyright (c) 2025, silvio3105
