
$(info *** Making GPU Monitor Hook ***)

NEXUS_TOP ?= $(shell cd ../../../../../../nexus; pwd)
include $(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build/platform_app.inc

all: copy_to_bin

ifndef B_REFSW_ARCH
B_REFSW_ARCH = mipsel-linux-uclibc
endif

ifeq ($(filter ${B_REFSW_ARCH}, mips-linux mips-uclibc mips-linux-uclibc), ${B_REFSW_ARCH})
# BIG ENDIAN
CFLAGS += -DBSTD_CPU_ENDIAN=BSTD_ENDIAN_BIG -DBIG_ENDIAN_CPU
else
# LITTLE ENDIAN
CFLAGS += -DBSTD_CPU_ENDIAN=BSTD_ENDIAN_LITTLE -DLITTLE_ENDIAN_CPU
endif

V3D_DIR ?= $(shell cd ../../driver; pwd)
V3D_PLATFORM_DIR ?= $(shell cd ../../platform; pwd)

CFLAGS += -I. -I$(V3D_DIR)/interface/khronos/include -I$(V3D_DIR)
ifeq ($(VC5_GPUMON_HOOK),)
CFLAGS += \
        -I./../../../vc5/tools/gpumon_hook
else
CFLAGS += \
        -I$(VC5_GPUMON_HOOK)
endif
## CAUTION: Using higher optimsation levels causes a SEGV when getting state
#CFLAGS += -O0 -fPIC -DPIC -fvisibility=hidden
CFLAGS += -O0 -fPIC -DPIC
CFLAGS += -c $(foreach dir,$(NEXUS_APP_INCLUDE_PATHS),-I$(dir)) $(foreach def,$(NEXUS_APP_DEFINES),-D"$(def)")

CFLAGS += -std=c++0x

LDFLAGS = -shared -Wl,--export-dynamic

.PHONY: copy_to_bin
copy_to_bin : lib/libgpumonitor.so
	$(info Copying libgpumonitor.so to $(NEXUS_BIN_DIR))
	@cp lib/libgpumonitor.so $(NEXUS_BIN_DIR)

lib/libgpumonitor.so : obj/spyhook.o obj/remote.o obj/archive.o obj/packet.o obj/packetreader.o obj/platform.o
	@mkdir -p lib
	$(info Linking $@)
	@$(B_REFSW_CROSS_COMPILE)g++ $(LDFLAGS) -o $@ $^

obj/%.o: %.cpp
	@mkdir -p obj
	$(info Compiling $<)
	@$(B_REFSW_CROSS_COMPILE)g++ -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf lib obj
