#############################################################################
#    (c)2014 Broadcom Corporation
#
# This program is the proprietary software of Broadcom Corporation and/or its licensors,
# and may only be used, duplicated, modified or distributed pursuant to the terms and
# conditions of a separate, written license agreement executed between you and Broadcom
# (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
# no license (express or implied), right to use, or waiver of any kind with respect to the
# Software, and Broadcom expressly reserves all rights in and to the Software and all
# intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
# HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
# NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
# secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
# and to use this information only in connection with your use of Broadcom integrated circuit products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
# WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
# THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
# LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
# OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
# USE OR PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
# LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
# EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
# USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
# ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
# LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
# ANY LIMITED REMEDY.
#
#############################################################################

NEXUS_PLATFORM ?= $(PLATFORM)
NEXUS_TOP := $(shell cd ../../../../../../nexus; pwd)
MAGNUM_TOP := $(shell cd ../../../../../../magnum; pwd)
BSEAV_TOP := $(shell cd ../../../../../../BSEAV; pwd)
SAGE_PUBLIC_TOP  := $(BSEAV_TOP)/lib/security/sage

B_REFSW_OBJ_DIR ?= obj.${NEXUS_PLATFORM}
B_REFSW_OBJ_ROOT ?= ${NEXUS_TOP}/../${B_REFSW_OBJ_DIR}
ENGINE_OBJ_ROOT = ${B_REFSW_OBJ_ROOT}/BSEAV/lib/security/sage/tools/engine

# Include nexus definitions
include $(NEXUS_TOP)/platforms/common/build/nexus_platforms.inc
include $(NEXUS_TOP)/platforms/common/build/platform_app.inc
include $(NEXUS_TOP)/build/nexus_defs.inc
include $(BSEAV_TOP)/lib/openssl/openssl.inc
include ${BSEAV_TOP}/lib/security/sage/utility/utility.inc

# Sanity check that we received a valid platform
ifndef BCHP_CHIP
$(error Unsupported platform $(NEXUS_PLATFORM))
endif

LIB_BASENAME := rsa_sage_engine

# Convert include paths into single variable
NEXUS_APP_INCLUDE_PATHS := $(foreach module, $(NEXUS_MODULES), $(NEXUS_$(module)_PUBLIC_INCLUDES))
NEXUS_APP_DEFINES := $(foreach module, $(NEXUS_MODULES), $(NEXUS_$(module)_DEFINES))
NEXUS_APP_DEFINES += $(foreach module, $(NEXUS_MODULES),NEXUS_HAS_$(module))

# Convert magnum includes into the same variable
NEXUS_APP_INCLUDE_PATHS += $(foreach module, $(MAGNUM_MODULES), $($(module)_INCLUDES))
NEXUS_APP_DEFINES += $(foreach module, $(MAGNUM_MODULES), $($(module)_DEFINES))


# add SAGElib related includes
include ${MAGNUM_TOP}/syslib/sagelib/bsagelib_public.inc
include ${MAGNUM_TOP}/syslib/sagelib/bsagelib_tools.inc
APP_INCLUDES += ${BSAGELIB_INCLUDES} ${BSAGELIB_TOOLS_INCLUDES}

# Custom include paths
CFLAGS += -I./include \
          -I$(SAGE_PUBLIC_TOP)/include \

CFLAGS += $(addprefix -I,${BSAGELIB_INCLUDES})
CFLAGS += $(addprefix -I,${UTILITY_INCLUDES})
#openssl include path
CFLAGS += $(B_OPENSSL_CFLAGS)

CFLAGS += -DPIC -fpic
CFLAGS += $(addprefix -I, ${NEXUS_APP_INCLUDE_PATHS})

# This is the minimum needed to compile and link with Nexus
CFLAGS += $(NEXUS_CFLAGS) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES))

#allow c++ style comments
CFLAGS += -std=c99
ifeq ($(ANDROID_BUILD),y)
CFLAGS += -EL -DANDROID
endif

F_SRCS := $(foreach enginesrc, $(shell ls ${BSEAV_TOP}/lib/security/sage/tools/engine/src/*.c), $(notdir ${enginesrc}))
vpath %.c ${BSEAV_TOP}/lib/security/sage/tools/engine/src
vpath %.h ${BSEAV_TOP}/lib/security/sage/tools/engine/src ${BSEAV_TOP}/lib/security/sage/srai/include

ifeq ($(NEXUS_MODE),proxy)
vpath %.c $(foreach SDIR, $(BSAGELIB_TOOLS_SOURCES), $(dir $(SDIR)))
vpath %.h $(foreach SDIR, $(BSAGELIB_TOOLS_SOURCES), $(dir $(SDIR)))
F_SRCS += $(foreach depSRC, ${BSAGELIB_TOOLS_SOURCES}, $(notdir ${depSRC}))
endif

F_PUBLIC_LIBS += lib${LIB_BASENAME}.a lib${LIB_BASENAME}.so
ifneq ($(NEXUS_BIN_DIR),)
D_FOR_LIB_INSTALL := $(NEXUS_BIN_DIR)
else
$(error define NEXUS_BIN_DIR for install dir)
endif
F_OBJS := $(patsubst %.c,%.o, ${F_SRCS})
END_OBJS := $(addprefix ${ENGINE_OBJ_ROOT}/,${F_OBJS})

F_INSTALLED_LIBS := $(addprefix ${D_FOR_LIB_INSTALL}/, ${F_PUBLIC_LIBS})

############################################################################
#                              MAIN TARGETS
#vvvv#####################vvvvvvvvvvvvvvvvvvvvvvv#####################vvvv##
prep_folder:
	@ mkdir -p ${ENGINE_OBJ_ROOT}/

prep_openssl:
	@echo [..... prep_openssl with j option];
	$(Q_)$(MAKE) -j8 -C $(B_OPENSSL_BUILD_DIR) > /dev/null

all: prep_folder prep_openssl ${F_PUBLIC_LIBS}

clean:
	@ echo clean objects [${F_OBJS}]
	@ rm -f ${F_OBJS}
	@ echo clean libraries [${F_PUBLIC_LIBS}]
	@ rm -f ${F_INSTALLED_LIBS} ${ENGINE_OBJ_ROOT}/lib${LIB_BASENAME}.so ${ENGINE_OBJ_ROOT}/lib${LIB_BASENAME}.a

re: clean install

#^^^^#####################^^^^^^^^^^^^^^^^^^^^^^^#####################^^^^##


############################################################################
#                             BUILD RULES
#vvvv#####################vvvvvvvvvvvvvvvvvvvvvvv#####################vvvv##
lib${LIB_BASENAME}.so: ${F_OBJS}
	@ echo [Linking shared library .... ${ENGINE_OBJ_ROOT}/$$@ ]
	@ ${CC} -shared -o ${ENGINE_OBJ_ROOT}/$@ ${LDFLAGS} ${END_OBJS} -Wl,-dy
	${STRIP_COMMAND}

lib${LIB_BASENAME}.a: ${F_OBJS}
	@ echo [Linking static library .... ${ENGINE_OBJ_ROOT}/$@ ]
	@ ${AR} rc ${ENGINE_OBJ_ROOT}/$@ ${END_OBJS}
	${STRIP_COMMAND}

idirs:
	@ [ -d ${D_FOR_LIB_INSTALL} ] || mkdir -p ${D_FOR_LIB_INSTALL}

install: all idirs ${F_INSTALLED_LIBS}

${D_FOR_LIB_INSTALL}/%.a : %.a
	install -m 0755 ${ENGINE_OBJ_ROOT}/$< $@

${D_FOR_LIB_INSTALL}/%.so : %.so
	install -m 0755 ${ENGINE_OBJ_ROOT}/$< $@

%.o: %.c
	@ echo [Compiling .... $< ]
	@ ${CC} ${CFLAGS} -c -o ${ENGINE_OBJ_ROOT}/$@ $<
