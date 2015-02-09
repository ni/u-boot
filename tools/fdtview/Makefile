hostprogs-y := fdtview

fdtview-objs := crc32.o ctype.o md5.o sha1.o sha256.o image.o image-fit.o \
		fdt.o fdt_ro.o fdt_rw.o fdt_strerror.o fdt_wip.o \
		fdtview.o

always := $(hostprogs-y)

# fw_printenv is supposed to run on the target system, which means it should be
# built with cross tools. Although it may look weird, we only replace "HOSTCC"
# with "CC" here for the maximum code reuse of scripts/Makefile.host.
#HOSTCC = $(CC)

#
# Use native tools and options
# Define __KERNEL_STRICT_NAMES to prevent typedef overlaps
#
HOST_EXTRACFLAGS   += -idirafter $(srctree)/include \
		-idirafter $(srctree)/include/u-boot \
		-idirafter $(objtree)/include \
	        -I$(srctree)/lib/libfdt \
		-I$(srctree)/tools \
		-DUSE_HOSTCC -D__KERNEL_STRICT_NAMES
