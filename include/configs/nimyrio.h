/*
 * See zynq-common.h for Zynq common configs
 */

#ifndef __CONFIG_NIMYRIO_H
#define __CONFIG_NIMYRIO_H

/*
 * High Level Configuration Options
 */
#define CONFIG_NIMYRIO /* Board */

#if defined(CONFIG_TARGET_NIMYRIO_BO)
#define CONFIG_BOARD_ONLY
#elif defined(CONFIG_TARGET_ROBORIO)
#define CONFIG_ROBORIO
#elif \
	defined(CONFIG_TARGET_ELVISRIOCM) || \
	defined(CONFIG_TARGET_ELVISRIOCM_MFG)
#define CONFIG_ELVISRIOCM
#endif

#if defined(CONFIG_BOARD_ONLY)
#define CONFIG_DEVICE_CODE "76D3"
#define CONFIG_FPGA_DEVICE_CODE "76D3"
#define CONFIG_DEVICE_DESC "myRIO-1950"
#elif defined(CONFIG_ROBORIO)
#define CONFIG_DEVICE_CODE "76F2"
#define CONFIG_FPGA_DEVICE_CODE "77A9"
#define CONFIG_DEVICE_DESC "roboRIO"
#elif defined(CONFIG_ELVISRIOCM)
#define CONFIG_DEVICE_CODE "7885"
#define CONFIG_FPGA_DEVICE_CODE "7885"
#define CONFIG_DEVICE_DESC "ELVIS RIO CM"
#else
#define CONFIG_DEVICE_CODE "762F"
#define CONFIG_FPGA_DEVICE_CODE "762F"
#define CONFIG_DEVICE_DESC "myRIO-1900"
#endif
#define CONFIG_TARGET_CLASS "cRIO"
#define CONFIG_PREFIXED_DEVICE_DESC "NI " CONFIG_DEVICE_DESC

/* For all myRIOs, pretend to be the packaged myRIO over USB */
#define CONFIG_NI_USB_PID "0x762F"
#define CONFIG_NI_USB_VID "0x3923"

#define CONFIG_NAND_ZYNQ
#define CONFIG_ENV_IS_IN_UBI

#include <configs/zynq-common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#define CONFIG_OF_BOARD_SETUP
#define CONFIG_REGINFO		/* Again, debugging */
/* check for input to stop even if delay is 0 */
#define CONFIG_ZERO_BOOTDELAY_CHECK
#undef CONFIG_BOOTDELAY
/* -1 to Disable autoboot, -2 to force boot */
#define CONFIG_BOOTDELAY	-2
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_U_BOOT_ONLY
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SYS_DEVICE_NULLDEV

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "hush> "
#undef CONFIG_SYS_MAXARGS
#define CONFIG_SYS_MAXARGS 64

#define CONFIG_CMD_UNZIP
#define CONFIG_CRC32_VERIFY
#define CONFIG_CMD_MD5SUM
#define CONFIG_MD5SUM_VERIFY
#define CONFIG_CMD_SHA1SUM
#define CONFIG_SHA1SUM_VERIFY
#define CONFIG_CMD_INI
#define CONFIG_INI_CASE_INSENSITIVE

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#if defined(CONFIG_ROBORIO)
#define CONFIG_NI_BOARD_NAME "roboRIO"
#elif defined(CONFIG_ELVISRIOCM)
#define CONFIG_NI_BOARD_NAME "ELVIS RIO CM"
#else
#define CONFIG_NI_BOARD_NAME "myRIO"
#endif

#ifdef CONFIG_MFG
#define CONFIG_NI_BOARD_NAME_SUFFIX " Manufacturing"
#else
#define CONFIG_NI_BOARD_NAME_SUFFIX
#endif

#define CONFIG_NI_BOARD_NAME_PREFIX

#define CONFIG_IDENT_STRING \
	"\nNational Instruments" \
	CONFIG_NI_BOARD_NAME_PREFIX " " \
	CONFIG_NI_BOARD_NAME \
	CONFIG_NI_BOARD_NAME_SUFFIX

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#ifdef CONFIG_ENV_IS_IN_FLASH
#undef CONFIG_ENV_IS_IN_FLASH /* zynq-common.h could have defined this */
#endif

#define CONFIG_ENV_UBI_PART "boot-config"
#define CONFIG_ENV_UBI_VOLUME "u-boot-env1"
#define CONFIG_ENV_UBI_VOLUME_REDUND "u-boot-env2"
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x20000

#define CONFIG_CMD_ENV_FLAGS

#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL

/* HW to use */
#ifndef CONFIG_DM_SERIAL
#undef CONFIG_ZYNQ_SERIAL
#ifdef CONFIG_ROBORIO
#define CONFIG_ZYNQ_SERIAL
#endif
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_CLK 58824000
#define CONFIG_SYS_NS16550_REG_SIZE 1
#define CONFIG_SYS_NS16550_COM1 0x80000000
#define CONFIG_SYS_NS16550_COM2 0x80000010
#define CONFIG_CONS_INDEX 1 /* not actually used */
#endif
#define CONFIG_CONSOLE_LINUX_DEV "ttyS0"
#define CONFIG_CONSOLE_UBOOT_DEV "eserial0"

#ifdef CONFIG_ROBORIO
#define CONFIG_NET_MULTI
#define CONFIG_MII
#endif /* CONFIG_ROBORIO */

#define CONFIG_MENU
#define CONFIG_CMD_PXE
#define CONFIG_BOOTP_MAY_FAIL
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_PXE_PLATFORM_DEFAULT	"default-" CONFIG_TARGET_CLASS, \
					"default-" CONFIG_DEVICE_CODE
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_HARD_I2C
#define CONFIG_ZYNQ_I2C

/* USB Host support */
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_USB_ULPI
#define CONFIG_USB_ULPI_VIEWPORT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_ZYNQ
#define CONFIG_USB_EHCI_TXFIFO_THRESH 8
#define CONFIG_EHCI_IS_TDI

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_SIZE (256 * 1024 * 1024)
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE

/*
 * NAND Flash settings
 */
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_NAND_ZYNQ_USE_FSBL_TIMINGS
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_RESERVE_END 524288
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"11M(u-boot)ro," \
		"70M(boot-config)," \
		"-(root)"

#if defined(CONFIG_MFG)
#define CONFIG_NAND_BBT_NO_MARK
#endif

/* UBI */
#define CONFIG_RBTREE
#define CONFIG_CMD_UBI

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

#define CONFIG_MTD_UBOOT_OFFSET		0x20000
#define CONFIG_BOARD_SIZE_LIMIT		0x100000
#define CONFIG_BOOT_BIN_SIZE_LIMIT	0x120000
#define CONFIG_BOOTFS_VOLUME_SIZE	0x3600000

#define CONFIG_BACKUP_PAGE		0xB1F800
#define CONFIG_BACKUP_USBGADGETETHADDR_OFFSET	0x7ea
#define CONFIG_BACKUP_SERIAL_OFFSET	0x7f0
#define CONFIG_BACKUP_ETHADDR_OFFSET	0x7f4
#define CONFIG_BACKUP_ETH1ADDR_OFFSET	0x7fa

#define CONFIG_DEFAULT_NVS "begin-base64 444 /lib/firmware/ti-connectivity/wl1271-nvs.bin.gz`H4sICK4CAAAAA3dsMTI3MS1udnMuYmluAGPMDWEAAsZCMIUAjDMZv0OZzP+/`/2PAA57/YWVn5+Tm5RcWkZBWUmXQZzBksGBwYvBkCGCIYohhiGdIYIhg8Mdn`xIgGf//9+w+kGCGACA1MwthEVzEDzWD6DzSKiZUBbB4DJ4uEAAfD9/fP74ME`+O1R+QwM6HwgePONgUfixx8GDoEPPxi4RCj0GgiUMzYwyMorqalrSskrqaqq`i8nKKaioBuABjExAwCAqKiooKsonp0sFNwwdAACjiUGVkAMAAA==`====`"

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x4000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#define FDT_HIGH "0x7FFFFFF"
#define INITRD_HIGH "0x7FF7FFF"
#define VERIFY_ADDR "0x8000000"

/* This address is 32k before the load address of the initrd */
#define FDT_ADDR "0x7FF8000"

#include "niresetenv.h"

#if defined(CONFIG_MYRIO)

/* myRIO uses ethaddr for the USB Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(ethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(ethaddr)

/* myRIO uses eth1addr for the WiFi Ethernet MAC. */
#define WIFIETHADDR_SAVE ENV_SAVE(eth1addr)
#define WIFIETHADDR_RESTORE ENV_RESTORE(eth1addr)

/* myRIO doesn't have wired Ethernet. */
#define ETHADDR_SAVE
#define ETHADDR_RESTORE

/* myRIO has a wl12xx */
#define WL12XXNVS_SAVE ENV_SAVE(wl12xxnvs)
#define WL12XXNVS_RESTORE ENV_RESTORE(wl12xxnvs)

#elif defined(CONFIG_ROBORIO)

/* roboRIO uses usbgadgetethaddr for the USB Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(usbgadgetethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(usbgadgetethaddr)

/* roboRIO doesn't have WiFi. */
#define WIFIETHADDR_SAVE
#define WIFIETHADDR_RESTORE

/* roboRIO uses ethaddr for wired Ethernet. */
#define ETHADDR_SAVE ENV_SAVE(ethaddr)
#define ETHADDR_RESTORE ENV_RESTORE(ethaddr)

/* roboRIO does not have a wl12xx */
#define WL12XXNVS_SAVE
#define WL12XXNVS_RESTORE

#elif defined(CONFIG_ELVISRIOCM)

/* elRIO uses usbgadgetethaddr for the USB Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(usbgadgetethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(usbgadgetethaddr)

/* elRIO doesn't have WiFi. */
#define WIFIETHADDR_SAVE
#define WIFIETHADDR_RESTORE

/* elRIO doesn't have wired Ethernet. */
#define ETHADDR_SAVE
#define ETHADDR_RESTORE

/* elRIO does not have a wl12xx */
#define WL12XXNVS_SAVE
#define WL12XXNVS_RESTORE

#endif

#include "nicommonenv.h"

#define CONFIG_ENV_FLAGS_LIST_STATIC \
	READONLY_DEFAULT_ENV_FLAGS \
	READONLY_MFG_ENV_VARS \
	NET_TYPE_ENV_VARS

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"serverip=192.168.1.185\0" \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"dcache off; " \
	"usb start; " \
	"usb reset; " \
	"dhcp; " \
	"run nc; " \
	"setenv silent;"

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#define CONFIG_PREBOOT \
	REAL_PREBOOT

#endif

#endif /* __CONFIG_NIMYRIO_H */
