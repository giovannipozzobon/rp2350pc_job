# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      builder.py
#      Purpose :   Generates a CMakeList.txt for a collection of modules.
#      Date :      2nd July 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,re,sys

# *******************************************************************************************
#
#                                   A single module
#
# *******************************************************************************************

class Module(object):
    def __init__(self,moduleName):
        assert os.path.isdir(moduleName),"Module directory {0} does not exist".moduleName
        self.moduleName = moduleName
        self.includeFile = moduleName+"_module.h"
        self.dependencyPosition = 0
        self.dependencies = [x.strip() for x in open(moduleName+os.sep+"dependencies.info").readlines() if not x.startswith("#") and x.strip() != ""]

    def __str__(self):
        return "[{0}] inc={1} dep={2}:{3}".format(self.moduleName,self.includeFile,self.dependencyPosition,",".join(self.dependencies))

# *******************************************************************************************
#
#                                 A collection of modules
#
# *******************************************************************************************

class ModuleSet(object):
    def __init__(self):
        self.modules = {}
        self.addModule("common")

    def addModule(self,module):
        if module not in self.modules:
            self.modules[module] = Module(module)

    def renderCMakeList(self,h,projectName = "newproject"):
        self.sortedModules = [x for x in self.modules.keys()]
        self.sortedModules.sort(key = lambda x:self.modules[x].dependencyPosition)
        header = "cmake_minimum_required(VERSION 3.12) |include(pico_sdk_import.cmake) |project(filetest) |pico_sdk_init() |option(USE_DEBUG \"Build with debug support\" ON) |if (USE_DEBUG) |  add_definitions(-DDEBUG) |endif() |include_directories(include)"
        header = header.replace("filetest",projectName)
        h.write("\n\n".join(header.split("|")))

        h.write("\n")
        h.write("\n".join(["include_directories(${{MODULEDIR}}/{0}/include)".format(x) for x in self.sortedModules]))
        h.write("\n\n")

        h.write("file(GLOB_RECURSE C_SOURCES \"source/*.[cs]\"\n")
        for m in self.sortedModules:
            h.write("file(GLOB_RECURSE {0}_MODULE_SOURCES \"${{MODULEDIR}}/{1}/source/*.[cs]\")\n".format(m.upper(),m))
        h.write("\nadd_executable({0}\n\tmain.c ${{C_SOURCES}}\n".format(projectName))
        h.write("\t"+" ".join(["{0}_MODULE_SOURCES".format(c) for c in self.sortedModules]))
        h.write("\n)\n")

        libs = "pico_stdlib,pico_multicore,hardware_dma,pico_sync"
        if "usb" in self.modules:
            libs += ",tinyusb_host,tinyusb_board"
        h.write("\ntarget_link_libraries({0} PUBLIC\n".format(projectName))        
        for l in libs.split(","):
            h.write("\t{0}\n".format(l))
        h.write(")\n")
        h.write("\npico_add_extra_outputs({0})\n".format(projectName))

if __name__ == "__main__":
    ms = ModuleSet()
    ms.addModule("dvi")
    ms.addModule("usb")
    ms.renderCMakeList(sys.stdout)


