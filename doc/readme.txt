Implementation type "qt"
========================
    - seems to require
        Qt5Core, Qt5Gui, Qt5Network, Qt5Qml, Qt5Widgets
                
        So in order to run the tests successfully from within a comprising
        project, you might need to copy the two files
            qtdeploy_autodepends_bin_win_uiwraptest.bat
            qtdeploy_autodepends_bindbg_win_uiwraptest.bat
        to your project's root dir.
        Compiling and testing the library standalone should just work,
        but this is untested.
