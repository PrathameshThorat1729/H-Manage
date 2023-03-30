### Directories ##
PROJECT = H-Manage
SRCDIR = src/$(PROJECT)
INCDIR = include/
B_OBJDIR = bin/obj/debug/$(PROJECT)
R_OBJDIR = bin/obj/release/$(PROJECT)

### Files ###
EXE = hmanage
SRCS = $(wildcard $(SRCDIR)/*.cpp)
L_SRCS = $(wildcard $(SRCDIR)/lib/*.cpp)

B_LIBS = $(patsubst $(SRCDIR)/lib/%.cpp, bin/build/debug/$(PROJECT)/lib%$(LIB_EXT), $(L_SRCS))
R_LIBS = $(patsubst $(SRCDIR)/lib/%.cpp, bin/build/release/$(PROJECT)/lib%$(LIB_EXT), $(L_SRCS))

B_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(B_OBJDIR)/%.o, $(SRCS))
R_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(R_OBJDIR)/%.o, $(SRCS))
L_B_OBJS = $(patsubst $(SRCDIR)/lib/%.cpp, $(B_OBJDIR)/lib/%.o, $(L_SRCS))
L_R_OBJS = $(patsubst $(SRCDIR)/lib/%.cpp, $(R_OBJDIR)/lib/%.o, $(L_SRCS))

B_BIN = bin/build/debug/$(PROJECT)
R_BIN = bin/build/release/$(PROJECT)

### Settings ###
CFLAGS = -I$(INCDIR) --std=c++17 -Wall
.PHONY: clean build build/lib release release/lib
LIB_EXT =
ifeq ($(OS), Windows_NT)
	LIB_EXT = .dll
else
	LIB_EXT = .so
endif

### Debug and release Builds ###

# Debug Build
build: $(B_OBJS) build/lib
	@mkdir -p bin/build bin/build/debug bin/build/debug/$(PROJECT)
	$(CXX) $(CFLAGS) $(B_LIBS) $(B_OBJS) -o $(B_BIN)/$(EXE)

# Release Build
release: CFLAGS += -O2
release: $(R_OBJS) release/lib
	@mkdir -p bin/build bin/build/release bin/build/release/$(PROJECT)
	$(CXX) $(CFLAGS) $(R_LIBS) $(R_OBJS) -o $(R_BIN)/$(EXE)

# Libs Building Target
build/lib: $(L_B_OBJS) $(B_LIBS)
release/lib: $(L_R_OBJS) $(R_LIBS)

### Libs Build ###
$(B_BIN)/lib%$(LIB_EXT): $(SRCDIR)/lib/%.cpp
	@mkdir -p bin/build bin/build/debug bin/build/debug/$(PROJECT)
	$(CXX) $(CFLAGS) -shared -fPIC $? -o $@

$(R_BIN)/lib%$(LIB_EXT): $(SRCDIR)/lib/%.cpp
	@mkdir -p bin/build bin/build/release bin/build/release/$(PROJECT)
	$(CXX) $(CFLAGS) -O2 -shared -fPIC $? -o $@

### Compiling Objects ###
$(B_OBJDIR)/%.o $(R_OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p bin
	@mkdir -p bin/obj bin/obj/debug bin/obj/debug/$(PROJECT)
	@mkdir -p bin/obj bin/obj/release bin/obj/release/$(PROJECT)
	$(CXX) $(CFLAGS) $? -c -o $@

$(B_OBJDIR)/lib/%.o $(R_OBJDIR)/lib/%.o: $(SRCDIR)/lib/%.cpp
	@mkdir -p bin
	@mkdir -p bin/obj bin/obj/debug bin/obj/debug/$(PROJECT) bin/obj/debug/$(PROJECT)/lib
	@mkdir -p bin/obj bin/obj/release bin/obj/release/$(PROJECT) bin/obj/release/$(PROJECT)/lib
	$(CXX) $(CFLAGS) $? -c -o $@

### Other Targets ###
clean:
	rm -rf bin/build/debug/* bin/build/release/* bin/obj/debug/* bin/obj/release/*