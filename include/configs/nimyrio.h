/*
 * See zynq-common.h for Zynq common configs
 */

#ifndef __CONFIG_NIMYRIO_H
#define __CONFIG_NIMYRIO_H

/*
 * High Level Configuration Options
 */
#define CONFIG_NIMYRIO /* Board */
#ifdef CONFIG_TARGET_NIMYRIO_BO
#define CONFIG_BOARD_ONLY
#define CONFIG_DEVICE_CODE "76D3"
#else
#define CONFIG_DEVICE_CODE "762F"
#endif

#define CONFIG_NAND_ZYNQ
#include <configs/zynq-common.h>

#undef CONFIG_ZYNQ_XIL_LQSPI

#define CONFIG_SYS_NO_FLASH

#define CONFIG_OF_BOARD_SETUP
#define CONFIG_REGINFO		/* Again, debugging */
/* check for input to stop even if delay is 0 */
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SYS_DEVICE_NULLDEV

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "hush> "

#define CONFIG_CMD_UNZIP
#define CONFIG_CRC32_VERIFY
#define CONFIG_CMD_MD5SUM
#define CONFIG_MD5SUM_VERIFY
#define CONFIG_CMD_SHA1SUM
#define CONFIG_SHA1SUM_VERIFY
#define CONFIG_CMD_INI
#define CONFIG_INI_CASE_INSENSITIVE

#define CONFIG_TIMESTAMP	/* print image timestamp on bootm, etc */

#define CONFIG_IDENT_STRING	"\nNational Instruments myRIO"

#define CONFIG_DEFAULT_HOSTNAME_BASE	"myRIO-"

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

#ifdef CONFIG_ENV_IS_IN_FLASH
#undef CONFIG_ENV_IS_IN_FLASH /* zynq-common.h could have defined this */
#endif

#define CONFIG_ENV_IS_IN_NAND
#ifdef CONFIG_ENV_OFFSET
#undef CONFIG_ENV_OFFSET
#endif
#define CONFIG_ENV_OFFSET		0xA0000
#define CONFIG_ENV_OFFSET_REDUND	0xE0000
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_SIZE			0x20000
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE
#define CONFIG_ENV_RANGE		0x40000

#define CONFIG_ENV_OVERWRITE

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

/*
 * Physical Memory map
 */
#define PHYS_SDRAM_1_SIZE (512 * 1024 * 1024)
#define CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH 0xFFFFF000
#define CONFIG_CMD_CACHE
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

/*
 * NAND Flash settings
 */
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=xilinx_nand"
#define MTDPARTS_DEFAULT \
	"mtdparts=xilinx_nand:" \
		"128k(fsbl)ro," \
		"512k(u-boot)ro," \
		"256k(u-boot-env1)," \
		"256k(u-boot-env2)," \
		"60M(boot-config)," \
		"-(root)"

/* UBI */
#define CONFIG_RBTREE
#define CONFIG_CMD_UBI

/* UBIFS */
#define CONFIG_LZO
#define CONFIG_CMD_UBIFS

#define CONFIG_MTD_UBOOT_OFFSET		0x20000
#define CONFIG_BOARD_SIZE_LIMIT		0x80000
#define CONFIG_BOOTFS_VOLUME_SIZE	0x3600000

#undef CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_LOAD_ADDR 0x8000000
#define CONFIG_LOADADDR CONFIG_SYS_LOAD_ADDR

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
	"autoload=n\0" \
	"silent=1\0" \
	"consolecmd=setenv console ttyPS0,$baudrate\0" \
	"ncoutport=7865\0" \
	"ncinport=8473\0" \
	"ncip=255.255.255.255\0" \
	"nc=setenv stdout nc;setenv stdin nc\0" \
	"sc=setenv stdout serial;setenv stdin serial\0" \
	"fdt_high=0x17ffffff\0" \
	"initrd_high=0x17ffffff\0" \
	"TargetClass=cRIO\0" \
	"DeviceDesc=cRIO-Zynq\0" \
	"DeviceCode=0x" CONFIG_DEVICE_CODE "\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=0x10000000\0" \
	"sdboot=echo Copying Safemode from SD to RAM...; " \
		"mmcinfo; " \
		"fatload mmc 0 $loadaddr linux_safemode.itb; " \
		"source $loadaddr:bootscript;\0" \
	"boot_safemode=" \
		"if ubifsload $loadaddr linux_safemode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"echo $safemode_err; " \
			"run recoverycmd; " \
			"run recoverybootcmd;" \
		"fi;\0" \
	"boot_runmode=" \
		"if ubifsload $loadaddr linux_runmode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"run boot_safemode; " \
		"fi;\0" \
	"recoverybootcmd=setenv bootcmd $savebootcmd; " \
		"setenv bootdelay $savebootdelay;\0" \
	"recoverycmd=echo Entering recovery mode!; " \
		"run markhardbootcomplete; " \
		"if test -n \\\"$forcedrecovery\\\"; " \
		"then " \
			"run ipresetcmd; " \
		"fi;" \
		"run ipconfigcmd; " \
		"savebootcmd=$bootcmd; " \
		"setenv bootcmd \\\"$recoverybootcmd\\\"; " \
		"savebootdelay=$bootdelay; " \
		"setenv bootdelay 0; " \
		"setenv silent; " \
		"run nc; " \
		"while sleep 1; do " \
			"echo \\\"${DeviceCode:-<not-set>}, " \
				"${serial#:-<not-set>}, " \
				"${ethaddr:-<not-set>}, " \
				"${ipaddr:-<not-set>}, " \
				"${hostname:-<not-set>}, " \
				"${comment:-<not-set>}\\\"; " \
		"done;\0" \
	"fpgaloadcmd=ubifsmount ubi:bootfs; " \
		"if test -n \\\"$isnofpgaapp\\\" -o $bootmode = safemode; then " \
			"loaddefaultbit=1; " \
		"else " \
			"if ubifsload $verifyaddr user.bit.crc && " \
				"ubifsload $loadaddr user.bit.bin && " \
				"md5sum -vp $loadaddr $filesize $verifyaddr; " \
			"then " \
				"configfpga=1; " \
			"else " \
				"loaddefaultbit=1; " \
			"fi; " \
		"fi; " \
		"if test -n \\\"$loaddefaultbit\\\"; then " \
			"if ubifsload $verifyaddr default.bit.crc && " \
				"ubifsload $loadaddr default.bit.bin && " \
				"md5sum -vp $loadaddr $filesize $verifyaddr; " \
			"then " \
				"configfpga=1; " \
			"fi; " \
		"fi; " \
		"if test -n \\\"$configfpga\\\"; then " \
			"fpga load 0 $loadaddr $filesize; " \
		"fi; " \
		"run markhardbootcomplete;\0" \
	"ipresetcmd=echo Resetting primary Ethernet configuration; " \
		"setenv dhcpenabled 1; " \
		"setenv linklocalenabled 1; " \
		"setenv sdnsip; " \
		"setenv sgatewayip; " \
		"setenv sipaddr; " \
		"setenv snetmask; " \
		"setenv hostname " CONFIG_DEFAULT_HOSTNAME_BASE "${serial#}; " \
		"setenv mtu; " \
		"setenv ncip 255.255.255.255; " \
		"savesilent=$silent;setenv silent 1; " \
		"saveenv; " \
		"setenv silent $savesilent;\0" \
	"ipconfigcmd=if test \\\"$dhcpenabled\\\" -ne 0 -a " \
			"-z \\\"$forcedrecovery\\\";" \
		"then " \
			"dhcpfail=0; " \
			"dhcp || dhcpfail=1;" \
		"else " \
			"dhcpfail=-1;" \
		"fi;" \
		"if test \\\"$linklocalenabled\\\" -ne 0 -a " \
			"\\\"$dhcpfail\\\" -ne 0 -o " \
			"-n \\\"$forcedrecovery\\\";" \
		"then " \
			"llfail=0;" \
			"linklocal || llfail=1;" \
		"else " \
			"llfail=-1;" \
		"fi;" \
		"if test \\\"$llfail\\\" -ne 0 -a " \
			"\\\"$dhcpfail\\\" -ne 0; " \
		"then " \
			"setenv ipaddr $sipaddr; " \
			"setenv netmask $snetmask; " \
			"setenv gatewayip $sgatewayip; " \
		"fi;\0" \
	"markhardbootcomplete=" \
		"i2c mw 0x40 1 0x20;\0" \
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
			"\\\"${safemode.enabled}\\\" != false; " \
		"then " \
			"issafemode=1; " \
		"fi; " \
		"if test ${cpld.ipreset} -ne 0 -o " \
			"\\\"${ipreset.enabled}\\\" != false; " \
		"then " \
			"isipreset=1; " \
		"fi; " \
		"if test ${cpld.consoleout} -ne 0 -o " \
			"\\\"${consoleout.enabled}\\\" != false; " \
		"then " \
			"isconsoleout=1; " \
		"fi; " \
		"if test \\\"${nofpgaapp.enabled}\\\" != false; then " \
			"isnofpgaapp=1; " \
		"fi; " \
		"if test ${cpld.softboot} -eq 0 -a " \
			"${cpld.resetbybutton} -ne 0; " \
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
		"setenv cpld.safemode; " \
		"setenv cpld.ipreset; " \
		"setenv cpld.consoleout; " \
		"setenv cpld.softboot; " \
		"setenv cpld.resetbybutton;\0" \
	"safemode_err=Failed to find a valid safemode image.\0" \
	"updateenv=env export -b $loadaddr; " \
		"env default -f; " \
		"env import -b $loadaddr;\0" \
	"writepartitions=" \
		"nand erase.part boot-config && " \
		"ubi part boot-config && " \
		"ubi create u-boot-env1 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create u-boot-env2 " __stringify(CONFIG_ENV_SIZE) " dynamic && " \
		"ubi create bootfs " __stringify(CONFIG_BOOTFS_VOLUME_SIZE) " dynamic && " \
		"ubi create config - dynamic && " \
		"nand erase.part root && " \
		"ubi part root && " \
		"ubi create rootfs - dynamic;\0" \
	"writeboot=nand erase 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand write $loadaddr 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"nand read $verifyaddr 0 " __stringify(CONFIG_ENV_OFFSET) "; " \
		"cmp.b $loadaddr $verifyaddr " __stringify(CONFIG_ENV_OFFSET) ";\0" \
	"writefsbl=nand erase.part fsbl; " \
		"nand write $loadaddr fsbl; " \
		"nand read $verifyaddr fsbl; " \
		"cmp.b $loadaddr $verifyaddr 0x20000;\0" \
	"writeuboot=nand erase.part u-boot; " \
		"nand write $loadaddr u-boot; " \
		"nand read $verifyaddr u-boot; " \
		"cmp.b $loadaddr $verifyaddr " \
			__stringify(CONFIG_BOARD_SIZE_LIMIT) ";\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND	\
	"if test $bootmode = safemode -o $bootmode = install; then " \
		"run boot_safemode; " \
	"else " \
		"run boot_runmode; "\
	"fi"

#define CONFIG_PREBOOT \
	"dcache on; " \
	"ubi part boot-config; " \
	"run readsoftdip; " \
	"run readcplddip; " \
	"run evaldip; " \
	"if test -n \\\"$isforcedrecoverymode\\\"; then " \
		"if test -n \\\"$isconsoleout\\\"; then " \
			"setenv silent; " \
		"fi; " \
		"forcedrecovery=1; " \
		"run recoverycmd; " \
	"else " \
		"run fpgaloadcmd; " \
		"if test -n \\\"$isipreset\\\"; then " \
			"run ipresetcmd; " \
		"fi; " \
		"if test -n \\\"$isconsoleout\\\"; then " \
			"setenv silent; " \
			"run consolecmd; " \
			"setenv consoleparam console=$console earlyprintk; " \
		"else " \
			"setenv consoleparam console= quiet; " \
			"setenv stderr nulldev; " \
			"setenv stdin nulldev; " \
			"setenv stdout nulldev; " \
			"setenv bootdelay -2; " \
		"fi; " \
		"run readbootmode; " \
		"if test -n \\\"$issafemode\\\"; then " \
			"bootmode=safemode; " \
		"fi; " \
	"fi;"

#endif /* __CONFIG_NIMYRIO_H */
