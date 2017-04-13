#ifndef __CONFIG_NICRIOZYNQ_H
#define __CONFIG_NICRIOZYNQ_H

/*
 * High Level Configuration Options
 */

#define CONFIG_NAND_ZYNQ
#define CONFIG_ENV_IS_IN_UBI

#include <configs/zynq-common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_OF_BOARD_SETUP


#if !defined (CONFIG_ELVISIII)
#define CONFIG_CMD_DATE		/* RTC? */
#endif

#undef CONFIG_BOOTDELAY
/* -1 to Disable autoboot, -2 to force boot */
#define CONFIG_BOOTDELAY	-2
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_U_BOOT_ONLY
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
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
#ifndef CONFIG_ELVISIII
#undef CONFIG_ZYNQ_SERIAL
#ifndef CONFIG_CRIO_ENETEXP
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_CLK 58824000
#define CONFIG_SYS_NS16550_REG_SIZE 1
#define CONFIG_SYS_NS16550_COM1 0x80000000
#if !defined (CONFIG_CRIO_GEN2) && !defined(CONFIG_SBRIO9607)
#define CONFIG_SYS_NS16550_COM2 0x80000010
#define CONFIG_SYS_NS16550_COM3 0x80000020
#endif
#define CONFIG_CONS_INDEX 1 /* not actually used */
#endif
#endif
#endif

/* Zynq GEM Ethernet support */
/* EMIO state for different speeds */
#define MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_ON	0
#define MACB_ZYNQ_GEM_LINKSPD_1000_GPIO_OFF	1
#define MACB_ZYNQ_GEM_LINKSPD_100_GPIO_ON	1
#define MACB_ZYNQ_GEM_LINKSPD_100_GPIO_OFF	0

#define CONFIG_NET_MULTI
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031
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
#define CONFIG_ETHADDR 00:80:2f:00:00:00
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
#if defined(CONFIG_CRIO_SBRIO96X7) && defined(CONFIG_CRIO_RMC)
#define CONFIG_OTG_USB_BASE_ADDR ZYNQ_USB_BASEADDR1
#endif

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_BASE 0x00000000
#if defined (CONFIG_CRIO_MEM256) || defined (CONFIG_CRIO_ENETEXP)
#define PHYS_SDRAM_1_SIZE (256 * 1024 * 1024)
#else
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

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

#if defined (CONFIG_CRIO_SBRIO96X7) && defined (CONFIG_CRIO_FULL_BOARD)
#define CONFIG_MMC

#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
#define CONFIG_ZYNQ_SDHCI
#define CONFIG_MMC_SDMA
#endif

#if defined (CONFIG_ELVISIII)
#define WLAN_ZYNQ_SDHCI_BASEADDR0	"e0100000"
#endif

#define CONFIG_DOS_PARTITION

#define NI_DEFAULT_NVS

#undef CONFIG_SYS_LOAD_ADDR
#if defined (CONFIG_CRIO_MEM256) || defined (CONFIG_CRIO_ENETEXP)
#define CONFIG_SYS_LOAD_ADDR 0x4000000
#else
#define CONFIG_SYS_LOAD_ADDR 0x8000000
#endif
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#if defined (CONFIG_CRIO_MEM256) || defined (CONFIG_CRIO_ENETEXP)
#define FDT_HIGH "0x7FFFFFF"
#define INITRD_HIGH "0x7FF7FFF"
#define VERIFY_ADDR "0x8000000"
#else
#define FDT_HIGH "0x17FFFFFF"
#define INITRD_HIGH "0x17FF7FFF"
#define VERIFY_ADDR "0x10000000"
#endif

/* This address is 32k before the load address of the initrd */
#define FDT_ADDR "0x7FF8000"

#include "niresetenv.h"

#if defined(CONFIG_CRIO_GEN2) || \
	(defined(CONFIG_CRIO_SBRIO96X7) && defined(CONFIG_CRIO_RMC)) || \
	defined(CONFIG_ELVISIII)

/* GEN2 cRIO-906x, sbRIO-96x7 with RMC use usbgadgetethaddr for the USB
 * Gadget Ethernet MAC. */
#define USBGADGETETHADDR_SAVE ENV_SAVE(usbgadgetethaddr)
#define USBGADGETETHADDR_RESTORE ENV_RESTORE(usbgadgetethaddr)

#else

#define USBGADGETETHADDR_SAVE
#define USBGADGETETHADDR_RESTORE

#endif

/* cRIO doesn't have WiFi. */
#define WIFIETHADDR_SAVE
#define WIFIETHADDR_RESTORE

#if defined(CONFIG_CRIO_MEM256) || \
	defined(CONFIG_CRIO_ENETEXP) || \
	(defined(CONFIG_CRIO_SBRIO96X7) && !defined(CONFIG_CRIO_RMC)) || \
	defined(CONFIG_ELVISIII)
/* cRIO-9063, cRIO-9066, NI 9147 and NI 9149 use ethaddr for wired Ethernet. */
#define ETHADDR_SAVE ENV_SAVE(ethaddr)
#define ETHADDR_RESTORE ENV_RESTORE(ethaddr)
#else
/* cRIO-9064, cRIO-9065, cRIO-9067 and cRIO-9068 use ethaddr and eth1addr for wired Ethernet. */
#define ETHADDR_SAVE ENV_SAVE(ethaddr) ENV_SAVE(eth1addr)
#define ETHADDR_RESTORE ENV_RESTORE(ethaddr) ENV_RESTORE(eth1addr)
#endif

/* cRIO does not have a wl12xx */
#define WL12XXNVS_SAVE
#define WL12XXNVS_RESTORE

#include "nicommonenv.h"

#define CONFIG_ENV_FLAGS_LIST_STATIC \
	READONLY_DEFAULT_ENV_FLAGS \
	READONLY_MFG_ENV_VARS \
	NET_TYPE_ENV_VARS

#if defined(CONFIG_ELVISIII)
#define CRIO_EXTRA_ENV_SETTINGS \
	"wirelessRegionFactory=00\0" \
	"wlan_sdio_base_addr=" WLAN_ZYNQ_SDHCI_BASEADDR0 "\0"
#else
#define CRIO_EXTRA_ENV_SETTINGS
#endif

#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"i2cbus=0\0" \
	"ipaddr=192.168.1.180\0" \
	"netmask=255.255.255.0\0" \
	"gatewayip=192.168.1.185\0" \
	"serverip=192.168.1.185\0" \
	"ethaddr=" __stringify(CONFIG_ETHADDR) "\0" \
	REAL_EXTRA_ENV_SETTINGS \
	CRIO_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"dcache off; " \
	"i2c dev $i2cbus; " \
	"usb start; " \
	"usb reset; " \
	"run nc; " \
	"setenv silent; "

#else /* !CONFIG_MFG */

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"i2cbus=0\0" \
	REAL_EXTRA_ENV_SETTINGS \
	CRIO_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	REAL_BOOTCOMMAND

#ifdef CONFIG_CRIO_ENETEXP
#define CONFIG_PREBOOT \
	REAL_PREBOOT_FEEBLE_RECOVERY
#else
#define CONFIG_PREBOOT \
	REAL_PREBOOT
#endif

#endif

#endif /* __CONFIG_NICRIOZYNQ_H */
