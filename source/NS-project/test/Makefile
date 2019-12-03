#---------
#
# CppUTest Examples Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = NectarSun
CPPUTEST_HOME = ../../cpputest

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.

# CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
SRC_DIRS = \
	common

TEST_SRC_DIRS = \
	AllTests

INCLUDE_DIRS =\
  .\
  common\
  $(CPPUTEST_HOME)/include\
  $(CPPUTEST_HOME)/include\CppUTest\MemoryLeakDetectorNewMacros.h
#   $(CPPUTEST_HOME)/include\CppUTest\MemoryLeakDetectorMallocMacros.h

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
