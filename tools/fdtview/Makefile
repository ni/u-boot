
include $(TOPDIR)/config.mk

# Source files which exist outside the tools/fdtview directory
EXT_SRC_FILES-y += lib/crc32.c
EXT_SRC_FILES-y += lib/ctype.c
EXT_SRC_FILES-y += lib/md5.c
EXT_SRC_FILES-y += lib/sha1.c
EXT_SRC_FILES-y += common/image.c

# Source files located in the tools/fdtview directory
SRC_FILES-y += fdtview.c

# Flattened device tree objects
LIBFDT_SRC_FILES-y += fdt.c
LIBFDT_SRC_FILES-y += fdt_ro.c
LIBFDT_SRC_FILES-y += fdt_rw.c
LIBFDT_SRC_FILES-y += fdt_strerror.c
LIBFDT_SRC_FILES-y += fdt_wip.c

HOSTSRCS	+= $(addprefix $(SRCTREE)/,$(EXT_SRC_FILES-y))
HOSTSRCS	+= $(addprefix $(SRCTREE)/tools/fdtview/,$(SRC_FILES-y))
HOSTSRCS	+= $(addprefix $(SRCTREE)/lib/libfdt/,$(LIBFDT_SRC_FILES-y))

$(warning $(HOSTSRCS))
#
# Use native tools and options
# Define __KERNEL_STRICT_NAMES to prevent typedef overlaps
#
HOSTCPPFLAGS   = -idirafter $(SRCTREE)/include \
		-idirafter $(OBJTREE)/include2 \
		-idirafter $(OBJTREE)/include \
	        -I $(SRCTREE)/libfdt \
		-I $(SRCTREE)/tools \
		-DUSE_HOSTCC -D__KERNEL_STRICT_NAMES

all :	$(obj)fdtview$(SFX)

$(obj)fdtview$(SFX) : $(HOSTSRCS)
	$(HOSTCC) $(HOSTCFLAGS_NOPED) $(HOSTLDFLAGS) -o $@ $(HOSTSRCS)
	$(HOSTSTRIP) $@

clean:
	rm -rf $(obj)*.o $(obj)fdtview

#########################################################################

include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################
