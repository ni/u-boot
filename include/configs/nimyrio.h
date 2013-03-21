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
#endif

#if defined(CONFIG_BOARD_ONLY)
#define CONFIG_DEVICE_CODE "76D3"
#define CONFIG_FPGA_DEVICE_CODE "76D3"
#define CONFIG_DEVICE_DESC "myRIO-1950"
#elif defined(CONFIG_ROBORIO)
#define CONFIG_DEVICE_CODE "76F2"
#define CONFIG_FPGA_DEVICE_CODE "77A9"
#define CONFIG_DEVICE_DESC "roboRIO"
#else
#define CONFIG_DEVICE_CODE "762F"
#define CONFIG_FPGA_DEVICE_CODE "762F"
#define CONFIG_DEVICE_DESC "myRIO-1900"
#endif

/* For all myRIOs, pretend to be the packaged myRIO over USB */
#define CONFIG_NI_USB_PID "0x762F"
#define CONFIG_NI_USB_VID "0x3923"

#define CONFIG_NAND_ZYNQ
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

#ifdef CONFIG_MFG
#define CONFIG_IDENT_STRING	"\nNational Instruments myRIO Manufacturing"
#else
#define CONFIG_IDENT_STRING	"\nNational Instruments myRIO"
#endif

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

#define CONFIG_BOOTP_MAY_FAIL
#define CONFIG_BOOTP_SERVERIP
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SEND_HOSTNAME

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_SPEED 400
#define CONFIG_HARD_I2C
#define CONFIG_ZYNQ_I2C

/* USB Host support */
#define CONFIG_USB_STORAGE
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
#define CONFIG_NAND_ZYNQ_USE_FSBL_TIMINGS
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
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

#define CONFIG_MTD_UBOOT_OFFSET		0x20000
#define CONFIG_BOARD_SIZE_LIMIT		0x100000
#define CONFIG_BOOT_BIN_SIZE_LIMIT	0x120000
#define CONFIG_BOOTFS_VOLUME_SIZE	0x3600000

#define CONFIG_BACKUP_PAGE		0xB1F800
#define CONFIG_BACKUP_SERIAL_OFFSET	0x7f0
#define CONFIG_BACKUP_ETHADDR_OFFSET	0x7f4
#define CONFIG_BACKUP_ETH1ADDR_OFFSET	0x7fa

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x4000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#define READONLY_DEFAULT_ENV_FLAGS \
	"consolecmd:so,ncoutport:do,ncinport:do,nc:so,sc:so," \
	"fdt_high:xo,initrd_high:xo,TargetClass:so,DeviceDesc:so," \
	"DeviceCode:xo,FPGADeviceCode:xo," \
	"USBVendorID:xo,USBProductID:xo,USBProduct:so," \
	"loadaddr:xo,verifyaddr:xo," \
	"boot_safemode:so,boot_runmode:so,consoleoutcmd:so," \
	"recoverybootcmd:so,recoverycmd:so,fpgaloadcmd:so,ipresetcmd:so," \
	"ipconfigcmd:so,markhardbootcomplete:so,stopwatchdog:so," \
	"setlederrorstatus:so,readbootmode:so," \
	"readsoftdip:so,readcplddip:so,evaldip:so,safemode_err:so," \
	"fpga_err:so,recovery_err:so,updateenv:so,resetenv:so," \
	"writepartitions:so,writeboot:so,writefsbl:so,writeuboot:so," \
	"bootcmd:so,preboot:so,"

#define READONLY_MFG_ENV_VARS \
	"serial#:xo,ethaddr:mc,eth1addr:mc,"

#define NET_TYPE_ENV_VARS \
	"ipaddr:i,sipaddr:i,netmask:i,snetmask:i,gatewayip:i,sgatewayip:i," \
	"ncip:i,mtu:d,"

#define CONFIG_ENV_FLAGS_LIST_STATIC \
	READONLY_DEFAULT_ENV_FLAGS \
	READONLY_MFG_ENV_VARS \
	NET_TYPE_ENV_VARS

#define REAL_EXTRA_ENV_SETTINGS \
	"autoload=n\0" \
	"silent=1\0" \
	"consolecmd=setenv console ttyPS0,$baudrate\0" \
	"ncoutport=7865\0" \
	"ncinport=8473\0" \
	"ncip=255.255.255.255\0" \
	"nc=setenv stdout nc;setenv stdin nc\0" \
	"sc=setenv stdout serial;setenv stdin serial\0" \
	"fdt_high=0x7ffffff\0" \
	"initrd_high=0x7ff7fff\0" \
	"TargetClass=cRIO\0" \
	"DeviceDesc=" CONFIG_DEVICE_DESC "\0" \
	"DeviceCode=0x" CONFIG_DEVICE_CODE "\0" \
	"FPGADeviceCode=0x" CONFIG_FPGA_DEVICE_CODE "\0" \
	"USBVendorID=" CONFIG_NI_USB_VID "\0" \
	"USBProductID=" CONFIG_NI_USB_PID "\0" \
	"USBProduct=NI myRIO\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=0x8000000\0" \
	"sdboot=echo Copying Safemode from SD to RAM...; " \
		"mmcinfo; " \
		"fatload mmc 0 $loadaddr linux_safemode.itb; " \
		"source $loadaddr:bootscript;\0" \
	"boot_safemode=" \
		"if ubifsmount ubi:bootfs && " \
		"ubifsload $loadaddr .safe/linux_safemode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"echo $safemode_err; " \
			"run recoverycmd; " \
			"run recoverybootcmd;" \
		"fi;\0" \
	"boot_runmode=" \
		"if ubifsmount ubi:bootfs && " \
		"ubifsload $loadaddr linux_runmode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"run boot_safemode; " \
		"fi;\0" \
	"consoleoutcmd=" \
		"if test -n \\\\\"$isconsoleout\\\\\"; then " \
			"run consolecmd; " \
			"if test \\\\\"$bootdelay\\\\\" -gt 0; then " \
				"setenv silent; " \
				"setenv consoleparam console=$console; " \
			"else " \
				"setenv consoleparam console=$console quiet; " \
			"fi; " \
		"else " \
			"setenv consoleparam console= quiet; " \
			"setenv stderr nulldev; " \
			"setenv stdin nulldev; " \
			"setenv stdout nulldev; " \
			"setenv bootdelay -2; " \
		"fi;\0" \
	"recoverybootcmd=" \
		"if test -n \\\\\"$savebootcmd\\\\\"; " \
		"then " \
			"flags=${.flags}; " \
			"setenv .flags bootcmd; " \
			"setenv bootcmd \\\\\"$savebootcmd\\\\\"; " \
			"setenv .flags $flags; " \
		"fi; " \
		"if test -n \\\\\"$savebootdelay\\\\\"; " \
		"then " \
			"setenv bootdelay $savebootdelay; " \
		"fi;\0" \
	"recoverycmd=echo Entering recovery mode!; " \
		"run markhardbootcomplete; " \
		"run stopwatchdog; " \
		"dcache off; " \
		"if test -n \\\\\"$forcedrecovery\\\\\"; " \
		"then " \
			"run resetenv && " \
			"run ipresetcmd; " \
		"fi;" \
		"if usb start && " \
			"fatload usb 0:auto $loadaddr recovery.cfg && " \
			"iminfo && " \
			"fdt addr $loadaddr && " \
			"fdt get value recoverType /images/recover type && " \
			"test $recoverType = script && " \
			"fdt get value recoveryDeviceCode / DeviceCode && " \
			"test $recoveryDeviceCode = $DeviceCode;" \
		"then " \
			"source $loadaddr:recover; " \
		"elif test -n \\\\\"$forcedrecovery\\\\\"; " \
		"then " \
			"flags=${.flags} && " \
			"setenv .flags bootcmd && " \
			"savebootcmd=$bootcmd && " \
			"setenv bootcmd \\\\\"$recoverybootcmd\\\\\" && " \
			"setenv .flags $flags && " \
			"savebootdelay=$bootdelay && " \
			"setenv bootdelay -2 && " \
			"run setlederrorstatus && " \
			"run ipconfigcmd; " \
			"run nc; " \
			"setenv silent; " \
			"while sleep 1; do " \
				"echo \\\"${DeviceCode:-<not-set>}, " \
					"${serial#:-<not-set>}, " \
					"${ethaddr:-<not-set>}, " \
					"${ipaddr:-<not-set>}, " \
					"${hostname:-<not-set>}, " \
					"${comment:-<not-set>}\\\"; " \
			"done; " \
		"else " \
			"run setlederrorstatus; " \
			"echo $recovery_err; " \
		"fi;\0" \
	"fpgaloadcmd=ubifsmount ubi:bootfs; " \
		"if test -n \\\\\"$isnofpgaapp\\\\\" -o $bootmode = safemode; then " \
			"loaddefaultbit=1; " \
		"else " \
			"if ubifsload $verifyaddr user.bit.crc && " \
				"ubifsload $loadaddr user.bit.bin && " \
				"md5sum -v $loadaddr $filesize *$verifyaddr; " \
			"then " \
				"if fpga load 0 $loadaddr $filesize; then " \
					"fpgasuccess=1; " \
				"else " \
					"echo $fpga_err..(user) ; " \
					"loaddefaultbit=1; " \
				"fi; " \
			"else " \
				"loaddefaultbit=1; " \
			"fi; " \
		"fi; " \
		"if test -n \\\\\"$loaddefaultbit\\\\\"; then " \
			"if ubifsload $verifyaddr .defbit/default.bit.crc && " \
				"ubifsload $loadaddr .defbit/default.bit.bin && " \
				"md5sum -v $loadaddr $filesize *$verifyaddr; " \
			"then " \
				"configfpga=1; " \
			"fi; " \
		"fi; " \
		"if test -n \\\\\"$configfpga\\\\\"; then " \
			"if fpga load 0 $loadaddr $filesize; then " \
				"fpgasuccess=1; " \
			"fi; " \
		"fi; " \
		"if test -z \\\\\"$fpgasuccess\\\\\"; then " \
			"echo $fpga_err; " \
			"run recoverycmd; " \
			"run recoverybootcmd;" \
		"fi; " \
		"run markhardbootcomplete;\0" \
	"ipresetcmd=echo Resetting primary Ethernet configuration; " \
		"setenv dhcpenabled 1; " \
		"setenv linklocalenabled 1; " \
		"setenv sdnsip; " \
		"setenv sgatewayip; " \
		"setenv sipaddr; " \
		"setenv snetmask; " \
		"setenv mtu; " \
		"setenv ncip 255.255.255.255; " \
		"savesilent=$silent;setenv silent 1; " \
		"saveenv; " \
		"setenv silent $savesilent;\0" \
	"ipconfigcmd=if test \\\\\"$dhcpenabled\\\\\" -ne 0 -a " \
			"-z \\\\\"$forcedrecovery\\\\\";" \
		"then " \
			"dhcpfail=0; " \
			"dhcp || dhcpfail=1;" \
		"else " \
			"dhcpfail=-1;" \
		"fi;" \
		"if test \\\\\"$linklocalenabled\\\\\" -ne 0 -a " \
			"$dhcpfail -ne 0 -o " \
			"-n \\\\\"$forcedrecovery\\\\\";" \
		"then " \
			"llfail=0;" \
			"linklocal || llfail=1;" \
		"else " \
			"llfail=-1;" \
		"fi;" \
		"if test $llfail -ne 0 -a " \
			"$dhcpfail -ne 0; " \
		"then " \
			"setenv ipaddr $sipaddr; " \
			"setenv netmask $snetmask; " \
			"setenv gatewayip $sgatewayip; " \
		"fi;\0" \
	"markhardbootcomplete=" \
		"i2c mw 0x40 1 0x20;\0" \
	"stopwatchdog=" \
		"i2c mw 0x40 0x13 0x80;\0" \
	"setlederrorstatus=" \
		"i2c mw 0x40 0x5 0xAA; " \
		"i2c mw 0x40 0x6 0xAA;\0" \
	"readbootmode=i2c read 0x40 0xFF 1 $loadaddr; " \
		"setexpr.b bootmodeval *$loadaddr \\\\& 0x3; " \
		"if test $bootmodeval -eq 1; then " \
			"bootmode=safemode; " \
		"else " \
			"bootmode=runmode; " \
		"fi; " \
		"setenv bootmodeval;\0" \
	"readsoftdip=if ubifsmount ubi:config && " \
		"ubifsload $loadaddr ni-rt.ini && " \
		"ini systemsettings; then " \
			"setenv safemode.enabled ${safemode.enabled:-false}; " \
			"setenv ipreset.enabled ${ipreset.enabled:-false}; " \
			"setenv consoleout.enabled ${consoleout.enabled:-false}; " \
			"setenv nofpgaapp.enabled ${nofpgaapp.enabled:-false}; " \
		"else " \
			"setenv safemode.enabled false; " \
			"setenv ipreset.enabled false; " \
			"setenv consoleout.enabled false; " \
			"setenv nofpgaapp.enabled false; " \
		"fi;\0" \
	"readcplddip=i2c read 0x40 1 1 $loadaddr; " \
		"setexpr.b cpld.safemode *$loadaddr \\\\& 0x01; " \
		"setexpr.b cpld.ipreset *$loadaddr \\\\& 0x02; " \
		"setexpr.b cpld.consoleout *$loadaddr \\\\& 0x04; " \
		"setexpr.b cpld.softboot *$loadaddr \\\\& 0x20; " \
		"i2c read 0x40 4 1 $loadaddr; " \
		"setexpr.b cpld.resetbybutton *$loadaddr \\\\& 0x01;\0" \
	"evaldip=" \
		"if test ${cpld.safemode} -ne 0 -o " \
			"\\\\\"${safemode.enabled}\\\\\" != false -o " \
			"\\\\\"${bootmode}\\\\\" = safemode; " \
		"then " \
			"issafemode=1; " \
			"bootmode=safemode; " \
		"fi; " \
		"if test ${cpld.ipreset} -ne 0 -o " \
			"\\\\\"${ipreset.enabled}\\\\\" != false; " \
		"then " \
			"isipreset=1; " \
		"fi; " \
		"if test ${cpld.consoleout} -ne 0 -o " \
			"\\\\\"${consoleout.enabled}\\\\\" != false; " \
		"then " \
			"isconsoleout=1; " \
		"fi; " \
		"if test \\\\\"${nofpgaapp.enabled}\\\\\" != false; then " \
			"isnofpgaapp=1; " \
		"fi; " \
		"if test ${cpld.softboot} -eq 0 -a " \
			"${cpld.resetbybutton} -ne 0 -a " \
			"${cpld.safemode} -ne 0; " \
		"then " \
			"isforcedrecoverymode=1; " \
		"fi; " \
		"setenv safemode.enabled; " \
		"setenv ipreset.enabled; " \
		"setenv consoleout.enabled; " \
		"setenv nofpgaapp.enabled; " \
		"setenv noapp.enabled; " \
		"setenv host_name; " \
		"setenv primarymac; " \
		"setenv language; " \
		"setenv sshd.enabled; " \
		"setenv ubifs_writeback_centisecs; " \
		"setenv ubifs_expire_centisecs; " \
		"setenv cpld.safemode; " \
		"setenv cpld.ipreset; " \
		"setenv cpld.consoleout; " \
		"setenv cpld.softboot; " \
		"setenv cpld.resetbybutton;\0" \
	"safemode_err=Failed to find a valid safemode image.\0" \
	"fpga_err=Failed to load an FPGA image.\0" \
	"recovery_err=Safemode or FPGA is corrupt. " \
		"Insert recovery USB and reboot.\0" \
	"updateenv=env export -b $loadaddr; " \
		"env default -a; " \
		"env import -b $loadaddr;\0" \
	"resetenv=" \
		"serial_save=${serial#} && " \
		"ethaddr_save=$ethaddr && " \
		"eth1addr_save=$eth1addr && " \
		"env default -a && " \
		"env set serial# $serial_save && " \
		"env set ethaddr $ethaddr_save && " \
		"env set eth1addr $eth1addr_save;\0" \
	"writepartitions=" \
		"if ubi part boot-config && " \
			"ubi read $verifyaddr u-boot-env1 1 && " \
			"ubi read $verifyaddr u-boot-env2 1; " \
		"then " \
			"ubi remove bootfs && " \
			"ubi remove config; " \
		"else " \
			"nand erase.part boot-config && " \
			"ubi part boot-config && " \
			"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
			"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic; " \
		"fi && " \
		"ubi create bootfs " __stringify(CONFIG_BOOTFS_VOLUME_SIZE) " dynamic && " \
		"ubi create config - dynamic && " \
		"nand erase.part root && " \
		"ubi part root && " \
		"ubi create rootfs - dynamic;\0" \
	"writeboot=nand erase 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"nand write $loadaddr 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"nand read $verifyaddr 0 " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) "; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_BOOT_BIN_SIZE_LIMIT) ";\0" \
	"writefsbl=nand erase.part fsbl; " \
		"nand write $loadaddr fsbl; " \
		"nand read $verifyaddr fsbl; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_MTD_UBOOT_OFFSET) ";\0" \
	"writeuboot=nand erase.part u-boot; " \
		"nand write $loadaddr u-boot; " \
		"nand read $verifyaddr u-boot; " \
		"cmp.b $loadaddr $verifyaddr " \
			__stringify(CONFIG_BOARD_SIZE_LIMIT) ";\0"

#define REAL_BOOTCOMMAND	\
	"if test $bootmode = safemode -o $bootmode = install; then " \
		"run boot_safemode; " \
	"else " \
		"run boot_runmode; "\
	"fi"

#define REAL_PREBOOT \
	"setenv silent 1; " \
	"dcache on; " \
	"nand lock tight; " \
	"ubi part boot-config; " \
	"run readsoftdip; " \
	"run readcplddip; " \
	"run readbootmode; " \
	"run evaldip; " \
	"run consoleoutcmd; " \
	"if test -n \\\\\"$isforcedrecoverymode\\\\\"; then " \
		"if test -n \\\\\"$isconsoleout\\\\\"; then " \
			"setenv silent; " \
		"fi; " \
		"forcedrecovery=1; " \
		"run recoverycmd; " \
	"else " \
		"run fpgaloadcmd; " \
		"if test -n \\\\\"$isipreset\\\\\"; then " \
			"run ipresetcmd; " \
		"fi; " \
	"fi;"


#if defined(CONFIG_MFG)

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"ipaddr=192.168.1.180\0" \
	"netmask=255.255.255.0\0" \
	"gatewayip=192.168.1.185\0" \
	"serverip=192.168.1.185\0" \
	REAL_EXTRA_ENV_SETTINGS

#undef CONFIG_BOOTCOMMAND

#define CONFIG_PREBOOT \
	"usb start; " \
	"usb reset; " \
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
