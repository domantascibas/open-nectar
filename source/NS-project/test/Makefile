#---------
#
# CppUTest Examples Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

.PHONY: check_paths

check_paths:
	@echo "\nRepository dir\t" $(REPOSITORY_DIR)
	@echo "\nProject dir\t" $(PROJECT_DIR)
	@echo "\nMakefile dir\t" $(MAKEFILE_DIR)
	@echo "\nTest dir\t" $(TEST_DIR)
	@echo "\nSrc dir  \t" $(SRC_DIR)
	@echo "\nSrc dirs  \t" $(SRC_DIRS)
	@echo "\nInclude dirs  \t" $(INCLUDE_DIRS)
	@echo "\nCpputest home  \t" $(CPPUTEST_HOME)

#--- Inputs ----#
MAKEFILE_DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
PROJECT_DIR = $(realpath $(MAKEFILE_DIR)../../..)
TEST_DIR = $(PROJECT_DIR)/source/NS-project/test
SRC_DIR = $(PROJECT_DIR)/source/src

COMPONENT_NAME = NectarSun
CPPUTEST_HOME = $(PROJECT_DIR)/source/cpputest
# SUBMODULES_DIR = $(realpath $(MAKEFILE_DIR)../../..)/submodules

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.

# CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
SRC_DIRS = \
	../../src/target-specific/NS-power/main-controller\
	../../src/target-specific/NS-main/relay-state\
	../../src/modules/generator-controller

SRC_FILES = \
	../../src/target-specific/NS-power/data.cpp\
	../../src/common/version.c\
	../../src/common/error_handler.cpp

TEST_SRC_DIRS = \
	AllTests\
	common

INCLUDE_DIRS = \
	.\
	common\
	$(CPPUTEST_HOME)/include\
	$(CPPUTEST_HOME)/include/CppUTest\
	../../../submodules/nectar_serial/src\
	../../src/target-specific/NS-power\
	../../src/target-specific/NS-power/main-controller\
	../../src/target-specific/NS-main/relay-state\
	../../src/modules/generator-controller\
	../../src/common
	
include $(CPPUTEST_HOME)/build/MakefileWorker.mk
