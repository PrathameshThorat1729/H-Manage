###########################
######### To Edit #########
###########################

PROJECT = H-Manage
EXE = hmanage
CFLAGS = --std=c++17 -Wall
CFLAGS_RELEASE = --std=c++17 -Wall -O2

INCDIR = include
SRCDIR = src/$(PROJECT)

CUSTOM_LIB_INCDIR = include/lib
CUSTOM_LIB_SRCDIR = src/$(PROJECT)/lib

DEBUG_OBJDIR = bin/obj/debug/$(PROJECT)
RELEASE_OBJDIR = bin/obj/release/$(PROJECT)

DEBUG_BUILD_DIR = bin/build/debug/$(PROJECT)
RELEASE_BUILD_DIR = bin/build/release/$(PROJECT)

# Optional
# Note : Do not edit till using
RESDIR = res
LOGDIR = bin/log
LIBSDIR = lib
SLIBSDIR = lib/bin

################################
######### Do Not Touch #########
################################

### Files ###
SRCS = $(wildcard $(SRCDIR)/*.cpp)
CLIB_SRCS = $(wildcard $(CUSTOM_LIB_SRCDIR)/*.cpp)

LIBS = $(patsubst $(LIBSDIR)/lib%.a, -l%, $(wildcard $(LIBSDIR)/lib*.a))
SLIBS = $(patsubst $(SLIBSDIR)/%, -l:%, $(wildcard $(SLIBSDIR)/*))

DEBUG_CLIBS = $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, $(DEBUG_BUILD_DIR)/lib%$(SLIB_EXT), $(CLIB_SRCS))
RELEASE_CLIBS = $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, $(RELEASE_BUILD_DIR)/lib%$(SLIB_EXT), $(CLIB_SRCS))

DEBUG_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(DEBUG_OBJDIR)/%.o, $(SRCS))
RELEASE_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(RELEASE_OBJDIR)/%.o, $(SRCS))
LIB_DEBUG_OBJS = $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, $(DEBUG_OBJDIR)/lib/%.o, $(CLIB_SRCS))
LIB_RELEASE_OBJS = $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, $(RELEASE_OBJDIR)/lib/%.o, $(CLIB_SRCS))
        
### Settings ###
CFLAGS += -I$(INCDIR) -I$(CUSTOM_LIB_INCDIR) -L$(LIBSDIR)
CFLAGS_RELEASE += -I$(INCDIR) -I$(CUSTOM_LIB_INCDIR) -L$(LIBSDIR)
.PHONY: build/debug build/release build lib/debug lib/release lib obj/debug obj/release obj run/build run/release clean/obj/debug clean/obj/release clean/obj clean/build/debug clean/build/release clean/build clean/lib/debug clean/lib/release clean/lib clean
SLIB_EXT =
ifeq ($(OS), Windows_NT)
	SLIB_EXT = .dll
else
	SLIB_EXT = .so
endif

### Debug and release Builds ###
all: build
default: build

# Debug Build
build/debug: $(DEBUG_OBJS) lib/debug
	@mkdir -p $(DEBUG_BUILD_DIR) $(LOGDIR)
	@(cp -rf $(RESDIR)/* $(DEBUG_BUILD_DIR) | true) 2>$(LOGDIR)/copyResDebuglog.txt
	@(cp -rf $(SLIBSDIR)/* $(DEBUG_BUILD_DIR) | true) 2>$(LOGDIR)/copySLibsDebuglog.txt
	$(CXX) $(CFLAGS) -L$(DEBUG_BUILD_DIR) $(DEBUG_OBJS) -o $(DEBUG_BUILD_DIR)/$(EXE) $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, -l%, $(CLIB_SRCS)) $(LIBS) $(SLIBS)

# Release Build
build/release: $(RELEASE_OBJS) lib/release
	@mkdir -p $(RELEASE_BUILD_DIR) $(LOGDIR)
	@(cp -rf $(RESDIR)/* $(RELEASE_BUILD_DIR) | true) 2>$(LOGDIR)/copyResReleaselog.txt
	@(cp -rf $(SLIBSDIR)/* $(RELEASE_BUILD_DIR) | true) 2>$(LOGDIR)/copySLibsReleaselog.txt
	$(CXX) $(CFLAGS_RELEASE) -L$(RELEASE_BUILD_DIR) $(RELEASE_OBJS) -o $(RELEASE_BUILD_DIR)/$(EXE) $(patsubst $(CUSTOM_LIB_SRCDIR)/%.cpp, -l%, $(CLIB_SRCS)) $(LIBS) $(SLIBS)

build: build/debug build/release

# Libs Building Target
lib/debug: $(LIB_DEBUG_OBJS) $(DEBUG_CLIBS)
lib/release: $(LIB_RELEASE_OBJS) $(RELEASE_CLIBS)
lib: lib/debug lib/release

### Libs Build ###
$(DEBUG_BUILD_DIR)/lib%$(SLIB_EXT): $(DEBUG_OBJDIR)/lib/%.o
	@mkdir -p $(DEBUG_BUILD_DIR)
	$(CXX) $(CFLAGS) -shared -fPIC $? -o $@

$(RELEASE_BUILD_DIR)/lib%$(SLIB_EXT): $(RELEASE_OBJDIR)/lib/%.o
	@mkdir -p $(RELEASE_BUILD_DIR)
	$(CXX) $(CFLAGS_RELEASE) -shared -fPIC $? -o $@

### Compiling Objects ###
obj/debug: $(DEBUG_OBJS) $(LIB_DEBUG_OBJS)
obj/release: $(RELEASE_OBJS) $(LIB_RELEASE_OBJS)
obj: obj/debug obj/release

$(DEBUG_OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(DEBUG_OBJDIR)
	$(CXX) $(CFLAGS) $? -c -o $@

$(RELEASE_OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(RELEASE_OBJDIR)
	$(CXX) $(CFLAGS_RELEASE) $? -c -o $@

$(DEBUG_OBJDIR)/lib/%.o: $(CUSTOM_LIB_SRCDIR)/%.cpp
	@mkdir -p $(DEBUG_OBJDIR)/lib
	$(CXX) $(CFLAGS) $? -c -o $@

$(RELEASE_OBJDIR)/lib/%.o: $(CUSTOM_LIB_SRCDIR)/%.cpp
	@mkdir -p $(RELEASE_OBJDIR)/lib
	$(CXX) $(CFLAGS_RELEASE) $? -c -o $@

### Other Targets ###
run/build:
	@./$(DEBUG_BUILD_DIR)/$(EXE)

run/release:
	@./$(RELEASE_BUILD_DIR)/$(EXE)

clean/obj/debug:
	rm -rf $(DEBUG_OBJDIR)/*

clean/obj/release:
	rm -rf $(RELEASE_OBJDIR)/*

clean/obj: clean/obj/debug clean/obj/release

clean/build/debug: clean/obj/debug
	rm -rf $(DEBUG_BUILD_DIR)/*

clean/build/release: clean/obj/release
	rm -rf $(RELEASE_BUILD_DIR)/*

clean/build: clean/build/debug clean/build/release

clean/lib/debug:
	rm -rf $(DEBUG_BUILD_DIR)/lib*$(SLIB_EXT) $(DEBUG_OBJDIR)/lib/*

clean/lib/release:
	rm -rf $(RELEASE_BUILD_DIR)/lib*$(SLIB_EXT) $(RELEASE_OBJDIR)/lib/*

clean/lib: clean/lib/debug clean/lib/release

clean: clean/build clean/obj