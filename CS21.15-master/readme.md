# Seattle University Capstone 2021  

Project Sponsor: SDI Engineering  
Student Team: Oleg Kovalenko, Jack Moss, Patrick Manacorda, Chris Fong  
SU Faculty: Michael McKee  

### Intro

SDI Engineering is exploring ways to improve UX for their proprietary GearSim software, used by clients like Boeing and Airbus to analyze landing gear simulations.

The CS 21.15 Seattle University students have designed and implemented a UI written in C++/Qt to reflect SDI's software API needs for GearSim's user interface. The focus is on writing well documented, highly extensible, and easily maintainable code.

Qt is an MVC framework and the group has taken on a steep learning curve to write idiomatic and generic code that can be the structural backbone of GearSim's UI, no matter how the look-and-feel and user workflow change over time.

The design of the UI is focused on addressing the complexity and scale of data input in GearSim, which can be overwhelming. The project's software design and architecture structure data entry into a linear, bite sized process.

### Project Status

 * Qt6 + QtCharts built from source
 * Integrated development workflow with Visual Studio + Git for a Windows 10 target
 * NestedTabBar: self contained generic Qt Widget well suited for splitting up data input
 * QJsonModel: a custom Qt model (MVC) that implements the rowsInserted and dataChanged signals, enabling short and expressive subclasses of QSortFilterProxyModel to carve up a JSON file into bite sized widgets
 * GenericPage: A flexible yet simple class that reflects the linear design of GearSim, without limiting it
 * JsonPager and MultiPager: Generic widgets that split up data into bite sized widgets for user input (from a proxy model)
 * Explorer: with NestedTabBar, allows users to select from a menu that populates tabs in other widgets
 * DataEntryPage: with the Data* classes, creates a sophisticated user input experience for scalar and tabular / graphical data
 * Linear Workflow Demo: A set of pages that incrementally implement a linear workflow for the GearSim UI
 * Multitab Workflow Demo: An experimental single page UI that implements a hierarchical workflow for GearSim based on NestedTabBar
 * Multithreaded Demo: Generalized code that SDI can use with the simulation object files to run simulations in parallel, and display the progress of each simulation to the user in the UI
 * ModuleLoader: A recursive descent .json parser that creates a QJsonModel for all the variables that the user needs to specify to run the simulation they have chosen

### Core Design / Architecture Overview

The main idea behind the project's software architecture is to accomplish SDI's stated objective of writing clean, generalized, and maintainable software that can form the basis for a GearSim port to Qt. To accomplish this, the team learned Qt and Model-View-Controller programming from scratch and prototyped a UI from first principles (no compatibility with any existing GearSim software) by methodically applying idiomatic Qt design patterns.

The following informal sequence diagram demonstrates the core data flow and user interaction so far as a GearSim user choosing and filling out a simulation, so it can be run on the Simulink backend (SDI's Simulink backend is not part of the project, but there is a dummy object file representing it that SDI can fill in to call their backend functionality).

The user begins by selecting some variables that determine what kind of simulation they will be running, such as choosing an airplane and a type of simulation (such as a drop test). Then the ModuleLoader creates a QJsonModel (a model in the Model-View-Controller sense) that contains everything the user needs to fill out to run the simulation. In GearSim, there are hundreds of combinations of planes and simulations, so writing a generic and maintainable component based implementation of ModuleLoader was a high priority for the project. QJsonModel is a highly customized fork of https://github.com/dridk/QJsonModel, implementing the rowsInserted signal for use with filters. Filters chop up the source QJsonmodel into filtered Proxy Models that propagate any user input to the source model, and they support chaining (proxies of proxies). The project makes heavy use of proxy models to write simple and short code that hands off chunks of the source model to views where the user can fill in bite sized quantities of information at a time, and the code is completely generic and extensible. The data inputs can change completely without needing to alter any source code, only the modules.json file that ModuleLoader uses to create the original source model needs to be updated. Once the user has finished filling in all the data for the simulation, all their inputs are automatically propagated to the source model, which the simulation code uses to extract all the data and run the simulation.

![sequence_diagram](https://user-images.githubusercontent.com/10703211/116953308-8561b180-ac52-11eb-87e0-e2a05a4e8313.png)

_The informal sequence diagram above was generated in about five minutes using mermaid: https://mermaid-js.github.io/mermaid-live-editor/_

### How to Build and Run the Project (does not depend on local Qt installation to work)

1. Download the latest binary executable, found here: https://drive.google.com/file/d/1i0mY83CCtmvUNMvq8MFHtzqJsPfConVO/view?usp=sharing
2. Unzip the download
3. Run the .exe found inside the unzipped download folder

### How to Run a Developer Build (assuming you have already have a binary for the Qt library)

_Detailed Project Developer Workflow Documentation:_ https://docs.google.com/document/d/195uBO7lC0gT_Rt1oRrgXRkfn_7_bjOkOTZDJWwsHt5Y/edit?usp=sharing

_The following steps are an overview. See the google drive link directly above to see detailed instructions with screen shots on every detail of this process. This whole process can take 5-15 minutes._

1. Download (or build from source, see below) a Qt6 build that includes the optional Charts module (download here: https://drive.google.com/file/d/1V7t8yOrDx4tnvPJrcmbZMqiE9yn7sKKP/view?usp=sharing)
2. Update Visual Studio 2019
3. Install Qt VS Tools
4. Add a Qt build to Qt VS Tools' Qt Version Tracker
5. Create an empty Qt Project in VS
6. Remove any starting files in the project
7. Set the project properties:
	1. General/C++ Language Standard: C++17
	2. Qt Project Settings/Qt Modules: core;gui;widgets;charts
	3. Linker/System/Subsytem: console
8. Add all .cpp and .h files to the Source Files in Solution Explorer
9. Add all resources files to the Resources in Solution Explorer
10. Configure PROJECTNAME.qrc to refer to all the resource files

### How to Build Qt6 with the Charts Module Installed (version 6.1)

#### Build Requirements:  

_If you are unfamiliar with how to edit your system path variable, check the end of this readme for a screen shot tutorial_

1. You need ~30 gb free space
2. Set your computer to never sleep (Power & Sleep settings)
3. Make sure python (version >= 2.6.x) is installed and in your path (probably in "C:\Users\YOURUSER\AppData\Local\Programs\Python\Python37\python.exe")
4. Make sure perl (version >= 5.14) is installed and in your path (probably in "C:\Users\YOURUSER\AppData\Local\Programs\Python\Python37\python.exe")
5. Make sure cmake is installed and in your path (probably in "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe")
6. Make sure windows ports of flex and bison versions 2.55 or greater are installed and in your path. (Download from here: https://sourceforge.net/projects/winflexbison/ You'll have to rename "win_flex.exe" to "flex.exe", and "win_bison.exe" to "bison.exe")
7. Open a command line prompt as administrator

![_cmd](https://user-images.githubusercontent.com/10703211/116955522-70881c80-ac58-11eb-8466-4d2d70c1a466.png)

8. cd to whatever directory has vcvarsall.bat in it (probably "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat")
9. run vcvarsall.bat x86_amd64 (use this command line prompt for the build instructions)

![_vcvarsall](https://user-images.githubusercontent.com/10703211/116955602-a0cfbb00-ac58-11eb-9ec2-d2bbf048e526.png)

#### Qt6 Build Steps:

_Current Qt6 build:_ https://drive.google.com/file/d/1V7t8yOrDx4tnvPJrcmbZMqiE9yn7sKKP/view?usp=sharing

1. cd C:\\
2. git clone git://code.qt.io/qt/qt5.git qt6  
3. cd qt6  
4. git checkout 6.1  
5. perl init-repository --module-subset=default,qtcharts

![_coffee](https://user-images.githubusercontent.com/10703211/116955632-b3e28b00-ac58-11eb-8c70-d09604c14481.png)

![_smoothly](https://user-images.githubusercontent.com/10703211/116955663-c8268800-ac58-11eb-963b-6217fd9c714b.png)

6. cd qtcharts
7. git checkout 6.1  
8. cd ..  
9. cd qtbase  
10. git checkout 6.1  
11. cd ..

![_quick](https://user-images.githubusercontent.com/10703211/116955673-d07ec300-ac58-11eb-9f5a-b3e42667d461.png)

(those are the minimums to satisfy dependencies. feel free to repeat steps 6-8 for all submodules (a list of all the submodules you have checked out is in the output from init-repository, here is what it was for me): qt5compat, qtbase, qtcharts, qtdeclarative, qtdoc, qtqa, qtquick3d, qtquickcontrols2, qtquicktimeline, qtrepotools, qtshadertools, qtsvg, qttools, qttranslations, qtwayland) 

12. mkdir qt6-build
13. cd qt6-build
14. ..\qt6\configure.bat\ -prefix C:\Qt6 -platform win32-msvc
15. cmake --build . --parallel && cmake --install .

![_end](https://user-images.githubusercontent.com/10703211/116956474-f73df900-ac5a-11eb-8d67-c5c82594deea.png)

DO NOT attempt to build using mingw. There is a bug in mingw's gcc that specifically occurs when building Qt.  

References:  
1. https://doc.qt.io/qt-5/windows-building.html  
2. https://doc.qt.io/qt-5/configure-options.html  
3. https://bugreports.qt.io/browse/QTBUG-47834  
4. https://wiki.qt.io/Building_Qt_6_from_Git  

#### Creating a Binary Deliverable:

_Current GearSim.zip:_ https://drive.google.com/file/d/1i0mY83CCtmvUNMvq8MFHtzqJsPfConVO/view?usp=sharing

1. Open Visual Studio project in File Explorer

![open_in_file_explorer](https://user-images.githubusercontent.com/50254044/117556071-d76f5200-b019-11eb-975b-dd1cc0931798.png)

2. Go to /x64/

![x64](https://user-images.githubusercontent.com/50254044/117555737-9a559080-b016-11eb-8ad5-20f2030c5691.png)

3. Copy Debug Folder to a location outside of Project (i.e. Desktop)

![debug](https://user-images.githubusercontent.com/50254044/117555728-988bcd00-b016-11eb-8b91-078422c23b51.png)

![copy_to_desktop](https://user-images.githubusercontent.com/50254044/117555727-988bcd00-b016-11eb-8afd-c11d7691e2f0.png)

4. Find the required .dll files by first running the .exe in the debug  folder.

![dll_warnings](https://user-images.githubusercontent.com/50254044/117556032-9119f300-b019-11eb-825b-2bb5be5303e5.png)

These files should be within your local QT6 installation in the bin folder. 
(i.e. Qt6Chartsd.dll, Qt6Cored.dll, Qt6Guid.dll, Qt6OpenGLd.dd,Qt6OpenGLd.dll, Qt6OpenGLWidgetsd.dll, Qt6Widgets.dll)

![bin_qt6](https://user-images.githubusercontent.com/50254044/117555723-975aa000-b016-11eb-93c3-b3add1b413bb.png)

![find_dlls](https://user-images.githubusercontent.com/50254044/117555732-99246380-b016-11eb-962c-19f455e404c9.png)

5. Copy in required .dll files for build 

![copy_into_debug](https://user-images.githubusercontent.com/50254044/117555724-97f33680-b016-11eb-9419-865ed65550ad.png)

6. In your QT6 installation folder, copy the plugins folder to the Debug Folder

![copy_plugins_folder](https://user-images.githubusercontent.com/50254044/117555725-97f33680-b016-11eb-97c7-d1e973fe56c9.png)

7. You should now be able to run the .exe file in the folder.

![run_gearsim_exe](https://user-images.githubusercontent.com/50254044/117556015-5748ec80-b019-11eb-8f92-7202a9c8cc6f.png)

8. Rename folder and compress into .zip - **Right Click folder -> Send to -> Compressed (zipped) Folder**

![debug_to_zip_file](https://user-images.githubusercontent.com/50254044/117555729-988bcd00-b016-11eb-96cc-8e33debe6dff.png)

#### How to edit your system path variable

1. Start -> "path"

![_path1](https://user-images.githubusercontent.com/10703211/116957079-8e578080-ac5c-11eb-9c17-eb4b4c165b0a.png)

2. Environment Variables

![_path2](https://user-images.githubusercontent.com/10703211/116957083-91527100-ac5c-11eb-83e9-c8eb9d14f71e.png)

3. **System Variables** -> Path -> Edit

![_path3](https://user-images.githubusercontent.com/10703211/116957091-944d6180-ac5c-11eb-8a84-91cfad1ee8bc.png)

4. New path (example is location of flex.exe and bison.exe)

![_path4](https://user-images.githubusercontent.com/10703211/116957093-96afbb80-ac5c-11eb-9893-fdbc918841f3.png)

5. Check that the path you added contains the .exes you need (in this case, flex.exe and bison.exe)

![_path5](https://user-images.githubusercontent.com/10703211/116957099-98797f00-ac5c-11eb-967c-d6e4eca68db0.png)
