mod_bumpy_life.la: mod_bumpy_life.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_bumpy_life.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_bumpy_life.la
