#----------------------------------------------------------------------------
#       Make Workspace
#
# $Id: MakeWorkspaceCreator.pm,v 1.19 2006/03/10 18:59:57 elliott_c Exp $
#
# This file was generated by MPC.  Any changes made directly to
# this file will be lost the next time it is generated.
#
# MPC Command:
# /usr/local/bin/MPC/mwc.pl 
#----------------------------------------------------------------------------


all: memtime test_ax_b_c_random

clean depend generated realclean $(CUSTOM_TARGETS):
	@$(MAKE) -f Makefile.memtime $(@)
	@$(MAKE) -f Makefile.test_ax_b_c_random $(@)

.PHONY: memtime
memtime:
	@$(MAKE) -f Makefile.memtime generated all

.PHONY: test_ax_b_c_random
test_ax_b_c_random:
	@$(MAKE) -f Makefile.test_ax_b_c_random generated all

project_name_list:
	@echo memtime
	@echo test_ax_b_c_random
