This is the WinCore project for ZeC

Following steps neded to build the project:
- Clone the WinCore package from https://devstack.vwgroup.com/bitbucket/scm/zecms/ebwincore.git repo to a path on your PC
- Substitute the path of the clone to the S drive in cmd shell: subst s: path_of_the_WinCore_Clone
- Clone the project (this repo) to any location on your PC.
- Start tresos (S:\bin\tresos_gui.exe)
- Import the ZeC project
    File-Import-General-Existing project into workspace
- Execute the following wizards:	
  	- "Create ECU Extract"
    - "Calculate Service Needs"
- Generate the project code.
- Open a cmd window at "project_directory\util"
- execute launch.bat
- execute make -j
- Now the WINDOWS_WIN32X86_ZEC_VR.exe file should be available on "project_directory\output\bin"

