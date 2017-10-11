#
# Macos Coverity targets
#
# Author: Prakash Dhavali
# Contact: hnd-software-scm-list
#
# $Id$
#
# NOTE: Do not update this makefile or add any new targets. Contact
# NOTE: prakashd for any questions
#

BRAND            ?= macos-coverity
COVERITY_RULES   ?= coverity-rules.mk
COVERITY_TARGETS ?= coverity-target-info.mk
SVNROOT          ?= http://svn.sj.broadcom.com/svn/wlansvn/proj
SVNCMD           ?= svn --non-interactive

include $(COVERITY_TARGETS)
include $(COVERITY_RULES)

# Checkout central trunk version of coverity target descriptions and rules
%.mk:
	$(SVNCMD) export $(if $(SVNCUTOFF),-r {"$(SVNCUTOFF)"}) \
 		$(SVNROOT)/trunk/src/tools/release/$@ $@
