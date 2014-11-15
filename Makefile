#
# OMNeT++/OMNEST Makefile for sim
#
# This file was generated with the command:
#  opp_makemake -f
#

# Name of target to be created (-o option)
TARGET = sim$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/blockproxy.o \
    $O/browser.o \
    $O/centralroute.o \
    $O/coordinator.o \
    $O/filesystem.o \
    $O/gate.o \
    $O/gateroute.o \
    $O/iptools.o \
    $O/logging.o \
    $O/proxy.o \
    $O/queue.o \
    $O/regionserver.o \
    $O/rng.o \
    $O/route.o \
    $O/server.o \
    $O/utils.o \
    $O/video.o \
    $O/paket_m.o \
    $O/report_m.o

# Message files
MSGFILES = \
    paket.msg \
    report.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f sim sim.exe libsim.so libsim.a libsim.dll libsim.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/blockproxy.o: blockproxy.cc \
  ./lrucache.h \
  ./utils.h \
  ./filesystem.h \
  ./blockproxy.h \
  ./report_m.h \
  ./types.h \
  ./logging.h \
  ./server.h \
  ./paket_m.h
$O/browser.o: browser.cc \
  ./utils.h \
  ./paket_m.h \
  ./logging.h \
  ./types.h \
  ./report_m.h \
  ./common.h
$O/centralroute.o: centralroute.cc \
  ./paket_m.h \
  ./common.h \
  ./report_m.h \
  ./route.h \
  ./types.h
$O/coordinator.o: coordinator.cc \
  ./paket_m.h \
  ./logging.h \
  ./types.h \
  ./report_m.h \
  ./common.h \
  ./filesystem.h \
  ./iptools.h \
  ./utils.h
$O/filesystem.o: filesystem.cc \
  ./types.h \
  ./common.h \
  ./filesystem.h
$O/gate.o: gate.cc \
  ./paket_m.h \
  ./video.h \
  ./logging.h \
  ./types.h \
  ./common.h \
  ./report_m.h \
  ./utils.h \
  ./iptools.h
$O/gateroute.o: gateroute.cc \
  ./report_m.h \
  ./common.h \
  ./types.h \
  ./route.h \
  ./paket_m.h \
  ./iptools.h
$O/iptools.o: iptools.cc \
  ./types.h
$O/logging.o: logging.cc \
  ./iptools.h \
  ./types.h \
  ./common.h \
  ./paket_m.h \
  ./logging.h
$O/paket_m.o: paket_m.cc \
  ./types.h \
  ./paket_m.h
$O/proxy.o: proxy.cc \
  ./types.h \
  ./common.h \
  ./report_m.h \
  ./paket_m.h \
  ./video.h \
  ./server.h \
  ./logging.h \
  ./utils.h \
  ./lrucache.h \
  ./blockproxy.h
$O/queue.o: queue.cc \
  ./types.h \
  ./logging.h \
  ./paket_m.h
$O/regionserver.o: regionserver.cc \
  ./types.h \
  ./report_m.h \
  ./common.h \
  ./paket_m.h \
  ./logging.h \
  ./video.h \
  ./server.h \
  ./utils.h \
  ./filesystem.h \
  ./iptools.h \
  ./lrucache.h
$O/report_m.o: report_m.cc \
  ./report_m.h \
  ./types.h
$O/rng.o: rng.cc
$O/route.o: route.cc \
  ./route.h \
  ./types.h \
  ./common.h \
  ./report_m.h \
  ./paket_m.h \
  ./iptools.h
$O/server.o: server.cc \
  ./lrucache.h \
  ./utils.h \
  ./iptools.h \
  ./filesystem.h \
  ./logging.h \
  ./server.h \
  ./paket_m.h \
  ./report_m.h \
  ./common.h \
  ./types.h
$O/utils.o: utils.cc \
  ./paket_m.h \
  ./report_m.h \
  ./common.h \
  ./types.h
$O/video.o: video.cc \
  ./video.h \
  ./types.h \
  ./common.h
