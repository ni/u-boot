/*
 * See zynq_common.h for Zynq common configs
 */

#ifndef __CONFIG_NISOMZYNQ_H
#define __CONFIG_NISOMZYNQ_H

/*
 * High Level Configuration Options
 */
#define CONFIG_DEVICE_CODE "775E"
#define CONFIG_FPGA_DEVICE_CODE "775E"
#define CONFIG_DEVICE_DESC "sbRIO-9651"
#define CONFIG_PREFIXED_DEVICE_DESC "NI " CONFIG_DEVICE_DESC
#define CONFIG_TARGET_CLASS "cRIO"

#define CONFIG_NI_USB_PID "0x770D"
#define CONFIG_NI_USB_VID "0x3923"

#define CONFIG_ZYNQ_GEM
#define CONFIG_NAND_ZYNQ
#include <configs/zynq-common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#define CONFIG_OF_BOARD_SETUP
#define CONFIG_CMD_DATE		/* RTC? */
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

#define CONFIG_NI_BOARD_NAME "sbRIO-9651"

#ifdef CONFIG_MFG
#define CONFIG_NI_BOARD_NAME_SUFFIX " Manufacturing"
#else
#define CONFIG_NI_BOARD_NAME_SUFFIX
#endif

#ifdef CONFIG_PROTO
#define CONFIG_NI_BOARD_NAME_PREFIX " Prototype"
#else
#define CONFIG_NI_BOARD_NAME_PREFIX
#endif

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

#define CONFIG_ENV_IS_IN_UBI
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
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_CLK 58824000
#define CONFIG_SYS_NS16550_REG_SIZE 1
#define CONFIG_SYS_NS16550_COM1 0x80000000
#define CONFIG_SYS_NS16550_COM2 0x80000010
#define CONFIG_SYS_NS16550_COM3 0x80000020
#define CONFIG_CONS_INDEX 1 /* not actually used */
#endif
#define CONFIG_CONSOLE_LINUX_DEV "ttyS0"
#define CONFIG_CONSOLE_UBOOT_DEV "eserial0"

/* Use both GEM interfaces */
#define CONFIG_NET_MULTI
#define CONFIG_ZYNQ_GEM

#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_PHYLIB
#define CONFIG_MENU
#define CONFIG_CMD_PXE
#define CONFIG_BOOTP_MAY_FAIL
#define CONFIG_BOOTP_SERVERIP
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SEND_HOSTNAME

#ifdef CONFIG_MFG
#define CONFIG_ETHADDR 00:80:2f:00:00:00
#endif

#define CONFIG_ZYNQ_GPIO

#define CONFIG_CMD_I2C
/* not really used */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_ZYNQ_I2C
#define CONFIG_RTC_DS3231
#define CONFIG_SYS_I2C_RTC_ADDR 0x68

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
#define XPSS_USB0_OTGCSR 0xE00021A4

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_SIZE (512 * 1024 * 1024)
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE

/*
 * NAND Flash settings
 */
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
#define CONFIG_ENV_FORCE_DEFAULT
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
#define CONFIG_BACKUP_SERIAL_OFFSET	0x7e4

/* Primary MAC Address */
#define CONFIG_BACKUP_ETHADDR_OFFSET	0x7e8

/* Secondary MAC Address */
#define CONFIG_BACKUP_ETH1ADDR_OFFSET	0x7ee

/* USB (Gadget) Ethernet MAC Address */
#define CONFIG_BACKUP_USBGADGETETHADDR_OFFSET	0x7f4

/* WiFi MAC Address */
#define CONFIG_BACKUP_ETH3ADDR_OFFSET	0x7fa

#define CONFIG_MMC

#ifdef CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
#define CONFIG_MMC_SDMA
#define CONFIG_DOS_PARTITION
#endif

#define CONFIG_DEFAULT_NVS "begin-base64 444 /lib/firmware/ti-connectivity/wl1271-nvs.bin.gz`H4sICK4CAAAAA3dsMTI3MS1udnMuYmluAGPMDWEAAsZCMIUAjDMZv0OZzP+/`/2PAA57/YWVn5+Tm5RcWkZBWUmXQZzBksGBwYvBkCGCIYohhiGdIYIhg8Mdn`xIgGf//9+w+kGCGACA1MwthEVzEDzWD6DzSKiZUBbB4DJ4uEAAfD9/fP74ME`+O1R+QwM6HwgePONgUfixx8GDoEPPxi4RCj0GgiUMzYwyMorqalrSskrqaqq`i8nKKaioBuABjExAwCAqKiooKsonp0sFNwwdAACjiUGVkAMAAA==`====`"

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x8000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#define FDT_HIGH "0x17FFFFFF"
#define INITRD_HIGH "0x17FF7FFF"
#define VERIFY_ADDR "0x10000000"

#include "niresetenv.h"

/* sbRIO-9651 uses usbgadgetethaddr for the USB Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(usbgadgetethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(usbgadgetethaddr)

/* sbRIO-9651 uses eth3addr for WiFi. */
#define WIFIETHADDR_SAVE ENV_SAVE(eth3addr)
#define WIFIETHADDR_RESTORE ENV_RESTORE(eth3addr)

/* sbRIO-9651 uses ethaddr and eth1addr for wired Ethernet. */
#define ETHADDR_SAVE ENV_SAVE(ethaddr) ENV_SAVE(eth1addr)
#define ETHADDR_RESTORE ENV_RESTORE(ethaddr) ENV_RESTORE(eth1addr)

/* sbRIO-9651 might have a wl12xx */
#define WL12XXNVS_SAVE ENV_SAVE(wl12xxnvs)
#define WL12XXNVS_RESTORE ENV_RESTORE(wl12xxnvs)

#include "nicommonenv.h"

#define USB_HOST_DEVICE_COMMANDS \
	"read_usb0_otgsc_id=setexpr.l usb0_otgsc_id *" \
		__stringify(XPSS_USB0_OTGCSR) \
		" \\\\& 0x100\0" \
	"set_usb0_host_device_mode=" \
		"if itest.l $usb0_otgsc_id == 0; then " \
			"fdt addr $loadaddr && " \
			"fdt get addr fdtaddr /images/fdt_" \
				CONFIG_DEVICE_CODE " data && " \
			"fdt addr $fdtaddr && " \
			"fdt set /amba@0/usb@e0002000 dr_mode host; " \
		"fi;\0" \

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	USB_HOST_DEVICE_COMMANDS \
	"ipaddr=192.168.1.180\0" \
	"netmask=255.255.255.0\0" \
	"gatewayip=192.168.1.185\0" \
	"serverip=192.168.1.185\0" \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"dcache off; " \
	"setenv silent;"

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	USB_HOST_DEVICE_COMMANDS \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#define CONFIG_PREBOOT \
	REAL_PREBOOT

#endif

#endif /* __CONFIG_NISOMZYNQ_H */
