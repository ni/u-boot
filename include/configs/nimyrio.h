/*
 * See zynq-common.h for Zynq common configs
 */

#ifndef __CONFIG_NIMYRIO_H
#define __CONFIG_NIMYRIO_H

/*
 * High Level Configuration Options
 */

#ifdef CONFIG_ROBORIO2
#define CONFIG_ZYNQ_QSPI
#define CONFIG_MMC
#define CONFIG_ENV_IS_IN_EXT4
#define CONFIG_PHY_SMSC
#else
#define CONFIG_NAND_ZYNQ
#define CONFIG_ENV_IS_IN_UBI
#endif

#include <configs/zynq-common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_OF_BOARD_SETUP

#undef CONFIG_BOOTDELAY
/* -1 to Disable autoboot, -2 to force boot */
#define CONFIG_BOOTDELAY	-2
#define CONFIG_SYS_DEVICE_NULLDEV

#define CONFIG_SYS_LONGHELP
#define CONFIG_HUSH_PARSER
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

#ifdef CONFIG_ENV_IS_IN_SPI_FLASH /* zynq-common.h could have defined this */
#undef CONFIG_ENV_IS_IN_SPI_FLASH
#endif

#ifdef CONFIG_ROBORIO2
#define EXT4_ENV_INTERFACE "mmc"
#define EXT4_ENV_DEVICE_AND_PART "1:1"
#define EXT4_ENV_FILE "/uboot/uboot.env"
#else
#define CONFIG_ENV_UBI_PART "boot-config"
#define CONFIG_ENV_UBI_VOLUME "u-boot-env1"
#define CONFIG_ENV_UBI_VOLUME_REDUND "u-boot-env2"
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x20000
#endif

#define CONFIG_CMD_ENV_FLAGS

#ifdef CONFIG_ROBORIO2
#define LOAD_BOOTFS "ext4load mmc 1:1"
#define LOAD_CONFIG "ext4load mmc 1:2"
#define UBIFS_WB_CS
#define UBIFS_EXP_CS
#else
#define LOAD_BOOTFS "ubifsmount ubi:bootfs && ubifsload"
#define LOAD_CONFIG "ubifsmount ubi:config && ubifsload"
#define UBIFS_WB_CS "setenv ubifs_writeback_centisecs; "
#define UBIFS_EXP_CS "setenv ubifs_expire_centisecs; "
#endif

#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_ZYNQPL


#ifdef CONFIG_ROBORIO2
#ifdef CONFIG_SF_DEFAULT_SPEED /* spi_flash.h could have defined this */
#undef CONFIG_SF_DEFAULT_SPEED
#endif
#define CONFIG_SF_DEFAULT_SPEED 60000000
#endif

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

#if defined(CONFIG_ROBORIO) || defined(CONFIG_ROBORIO2)
/* Zynq GEM Ethernet support */
/* EMIO state for different speeds */
#define MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_ON	0
#define MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_OFF	1
#define MACB_ZYNQ_GEM_LINKSPD_100_GPIO_ON	1
#define MACB_ZYNQ_GEM_LINKSPD_100_GPIO_OFF	0

#define CONFIG_NET_MULTI
#define CONFIG_CMD_MII
#define CONFIG_MII
#endif /* CONFIG_ROBORIO || CONFIG_ROBORIO2 */

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

#ifdef CONFIG_MFG
#define CONFIG_NET_RANDOM_ETHADDR
#endif

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
#define PHYS_SDRAM_1_BASE 0x00000000
#if defined (CONFIG_NI_MEM256)
#define PHYS_SDRAM_1_SIZE (256 * 1024 * 1024)
#elif defined(CONFIG_NI_MEM512)
#define PHYS_SDRAM_1_SIZE (512 * 1024 * 1024)
#endif
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE

/*
 * NAND Flash settings
 */
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"11M(u-boot)ro," \
		"70M(boot-config)," \
		"-(root)"

/* UBI */
#define CONFIG_RBTREE
#define CONFIG_CMD_UBI

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

/* MMC */
#ifdef CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_DM_MMC
#define CONFIG_MMC_SDHCI
#define CONFIG_MMC_SDHCI_SDMA
#define CONFIG_MMC_SDHCI_ZYNQ
#define CONFIG_ZYNQ_SDHCI_MAX_FREQ 125000000
#define CONFIG_CMD_MMC
#endif

/* QSPI Flash */
#ifdef CONFIG_ZYNQ_QSPI
#define CONFIG_DM_SPI
#define CONFIG_DM_SPI_FLASH
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF
#endif

#define NI_DEFAULT_NVS "begin-base64 444 /lib/firmware/ti-connectivity/wl1271-nvs.bin.gz`H4sICK4CAAAAA3dsMTI3MS1udnMuYmluAGPMDWEAAsZCMIUAjDMZv0OZzP+/`/2PAA57/YWVn5+Tm5RcWkZBWUmXQZzBksGBwYvBkCGCIYohhiGdIYIhg8Mdn`xIgGf//9+w+kGCGACA1MwthEVzEDzWD6DzSKiZUBbB4DJ4uEAAfD9/fP74ME`+O1R+QwM6HwgePONgUfixx8GDoEPPxi4RCj0GgiUMzYwyMorqalrSskrqaqq`i8nKKaioBuABjExAwCAqKiooKsonp0sFNwwdAACjiUGVkAMAAA==`====`"

#undef CONFIG_SYS_LOAD_ADDR
#if defined (CONFIG_NI_MEM256)
#define CONFIG_SYS_LOAD_ADDR 0x4000000
#elif defined(CONFIG_NI_MEM512)
#define CONFIG_SYS_LOAD_ADDR 0x8000000
#endif
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#if defined (CONFIG_NI_MEM256)
#define FDT_HIGH "0x7FFFFFF"
#define INITRD_HIGH "0x7FF7FFF"
#define VERIFY_ADDR "0x8000000"
#elif defined(CONFIG_NI_MEM512)
#define FDT_HIGH "0x17FFFFFF"
#define INITRD_HIGH "0x17FF7FFF"
#define VERIFY_ADDR "0x10000000"
#endif

/* This address is 32k before the load address of the initrd */
#define FDT_ADDR "0x7FF8000"

#include "niresetenv.h"

#if defined(CONFIG_NIMYRIO)

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

#elif defined(CONFIG_ROBORIO) || defined(CONFIG_ROBORIO2)

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

#if defined(CONFIG_NIMYRIO)
#define MYRIO_EXTRA_ENV_SETTINGS \
	"wirelessRegionFactory=840\0"
#else
#define MYRIO_EXTRA_ENV_SETTINGS
#endif

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"i2cbus=0\0" \
	"serverip=192.168.1.185\0" \
	REAL_EXTRA_ENV_SETTINGS \
	MYRIO_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"dcache off; " \
	"i2c dev $i2cbus; " \
	"usb start; " \
	"usb reset; " \
	"dhcp; " \
	"run nc; " \
	"setenv silent;"

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"i2cbus=0\0" \
	REAL_EXTRA_ENV_SETTINGS \
	MYRIO_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#ifdef CONFIG_ROBORIO2
#define CONFIG_PREBOOT \
	REAL_PREBOOT_NO_NAND
#else
#define CONFIG_PREBOOT \
	REAL_PREBOOT
#endif

#endif

#endif /* __CONFIG_NIMYRIO_H */
