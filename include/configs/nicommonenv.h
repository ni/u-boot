/*
 * Common environment definition for all NI devices
 */

#ifndef __CONFIG_NICOMMON_ENV_H
#define __CONFIG_NICOMMON_ENV_H

#define READONLY_DEFAULT_ENV_FLAGS \
	"consolecmd:so,ncoutport:do,ncinport:do,nc:so,sc:so," \
	"fdt_high:xo,initrd_high:xo,TargetClass:so,DeviceDesc:so," \
	"DeviceCode:xo,FPGADeviceCode:xo," \
	"USBVendorID:xo,USBProductID:xo,USBProduct:so,USBDevice:xo," \
	"loadaddr:xo,verifyaddr:xo," \
	"backuppage:xo,backupserialoffset:xo,backupethaddroffset:xo," \
	"backupeth1addroffset:xo," \
	"backupusbgadgetethaddroffset:xo," \
	"backupeth3addroffset:xo," \
	"boot_safemode:so,boot_runmode:so,boot_pxe:so,consoleoutcmd:so," \
	"recoverybootcmd:so,recoverycmd:so,fpgaloadcmd:so,ipresetcmd:so," \
	"ipconfigcmd:so,markhardbootcomplete:so,stopwatchdog:so," \
	"setlederrorstatus:so,readbootmode:so," \
	"readsoftdip:so,readcplddip:so,evaldip:so,safemode_err:so," \
	"fpga_err:so,recovery_err:so,updateenv:so,resetenv:so," \
	"writepartitions:so,writeboot:so,writefsbl:so,writeuboot:so," \
	"bootcmd:so,preboot:so,mtdids:so,mtdparts:so,"

#define READONLY_MFG_ENV_VARS \
	"serial#:xo,ethaddr:mc,eth1addr:mc,usbgadgetethaddr:mc," \
	"wirelessRegionFactory:so,wl12xxnvs:so,"

#define NET_TYPE_ENV_VARS \
	"ipaddr:i,sipaddr:i,netmask:i,snetmask:i,gatewayip:i,sgatewayip:i," \
	"ncip:i,mtu:d,"

/* Make sure that all resetenv save and restore macros are defined. */
#if !defined(USBGADGETETHADDR_SAVE) || !defined(USBGADGETETHADDR_RESTORE) || \
    !defined(WIFIETHADDR_SAVE) || !defined(WIFIETHADDR_RESTORE) || \
    !defined(ETHADDR_SAVE) || !defined(ETHADDR_RESTORE) || \
    !defined(WL12XXNVS_SAVE) || !defined(WL12XXNVS_RESTORE)
#error "Please define resetenv save and restore macros for your board."
#endif

#if defined(CONFIG_NI_USB_VID) && defined(CONFIG_NI_USB_PID)
#define CONFIG_GADGET_VARS \
	"USBVendorID=" CONFIG_NI_USB_VID "\0" \
	"USBProductID=" CONFIG_NI_USB_PID "\0" \
	"USBProduct=" CONFIG_PREFIXED_DEVICE_DESC "\0" \
	"USBDevice=0x" CONFIG_DEVICE_CODE "\0"
#else
#define CONFIG_GADGET_VARS
#endif

#define REAL_EXTRA_ENV_SETTINGS \
	"autoload=n\0" \
	"silent=1\0" \
	"consolecmd=setenv console " CONFIG_CONSOLE_LINUX_DEV ",$baudrate\0" \
	"ncoutport=7865\0" \
	"ncinport=8473\0" \
	"ncip=255.255.255.255\0" \
	"nc=setenv stdout nc;setenv stdin nc\0" \
	"sc=setenv stdout " CONFIG_CONSOLE_UBOOT_DEV ";" \
		"setenv stderr " CONFIG_CONSOLE_UBOOT_DEV ";" \
		"setenv stdin " CONFIG_CONSOLE_UBOOT_DEV ";\0" \
	"fdt_high=" FDT_HIGH "\0" \
	"initrd_high=" INITRD_HIGH "\0" \
	"TargetClass=" CONFIG_TARGET_CLASS "\0" \
	"DeviceDesc=" CONFIG_DEVICE_DESC "\0" \
	"DeviceCode=0x" CONFIG_DEVICE_CODE "\0" \
	"FPGADeviceCode=0x" CONFIG_FPGA_DEVICE_CODE "\0" \
	CONFIG_GADGET_VARS \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"verifyaddr=" VERIFY_ADDR "\0" \
	"backuppage=" __stringify(CONFIG_BACKUP_PAGE) "\0" \
	"backupserialoffset=" __stringify(CONFIG_BACKUP_SERIAL_OFFSET) "\0" \
	"backupethaddroffset=" __stringify(CONFIG_BACKUP_ETHADDR_OFFSET) "\0" \
	"backupeth1addroffset=" \
		__stringify(CONFIG_BACKUP_ETH1ADDR_OFFSET) "\0" \
	"backupusbgadgetethaddroffset=" \
		__stringify(CONFIG_BACKUP_USBGADGETETHADDR_OFFSET) "\0" \
	"backupeth3addroffset=" \
		__stringify(CONFIG_BACKUP_ETH3ADDR_OFFSET) "\0" \
	"wirelessRegionFactory=840\0" \
	"wl12xxnvs=" CONFIG_DEFAULT_NVS "\0" \
	"sdboot=echo Copying Safemode from SD to RAM...; " \
		"mmcinfo; " \
		"fatload mmc 0 $loadaddr linux_safemode.itb; " \
		"source $loadaddr:bootscript;\0" \
	"boot_safemode=" \
		"if ubifsmount ubi:bootfs && " \
		"ubifsload $loadaddr .safe/linux_safemode.itb && " \
		"imi $loadaddr; then " \
			"setenv verify n; " \
			"run set_usb0_host_device_mode; " \
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
			"run set_usb0_host_device_mode; " \
			"source $loadaddr:bootscript; " \
		"else " \
			"run boot_safemode; " \
		"fi;\0" \
	"boot_pxe=" \
		"dcache off; " \
		"while sleep 1; do " \
			"setenv pxefile_addr_r $loadaddr; " \
			"dhcp && " \
			"if pxe get; then " \
				"setenv kernel_addr_r $loadaddr; " \
				"setenv ramdisk_addr_r $verifyaddr; " \
				"setenv fdt_addr_r " FDT_ADDR "; " \
				"pxe boot; " \
			"fi; " \
		"done;\0" \
	"consoleoutcmd=" \
		"if test -n \\\\\"$isconsoleout\\\\\"; then " \
			"run consolecmd; " \
			"if test \\\\\"$bootdelay\\\\\" -gt 0; then " \
				"setenv silent; " \
				"setenv consoleparam console=$console; " \
			"else " \
				"setenv consoleparam console=$console quiet; " \
			"fi; " \
			"run sc; " \
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
			"usb reset && " \
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
	"readbootmode=i2c read 0x40 0x1F 1 $loadaddr; " \
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
			"setenv pxeboot.enabled ${pxeboot.enabled:-false}; " \
		"else " \
			"setenv safemode.enabled false; " \
			"setenv ipreset.enabled false; " \
			"setenv consoleout.enabled false; " \
			"setenv nofpgaapp.enabled false; " \
			"setenv pxeboot.enabled false; " \
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
		"if test \\\\\"${pxeboot.enabled}\\\\\" != false; then " \
			"ispxeboot=1; " \
		"fi; " \
		"setenv safemode.enabled; " \
		"setenv ipreset.enabled; " \
		"setenv consoleout.enabled; " \
		"setenv nofpgaapp.enabled; " \
		"setenv noapp.enabled; " \
		"setenv pxeboot.enabled; " \
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
		ENV_SAVE(wirelessRegionFactory) \
		USBGADGETETHADDR_SAVE \
		WIFIETHADDR_SAVE \
		ETHADDR_SAVE \
		WL12XXNVS_SAVE \
		"env default -a && " \
		"env set serial# $serial_save && " \
		ENV_RESTORE(wirelessRegionFactory) \
		USBGADGETETHADDR_RESTORE \
		WIFIETHADDR_RESTORE \
		ETHADDR_RESTORE \
		WL12XXNVS_RESTORE \
		"setenv .flags;\0" \
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
		"if ubi part root && " \
			"ubi read $verifyaddr rootfs 1; " \
		"then " \
			"ubi remove rootfs; " \
		"else " \
			"nand erase.part root && " \
			"ubi part root; " \
		"fi && " \
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

#define REAL_BOOTCOMMAND \
	"if test $bootmode = safemode; then " \
		"run boot_safemode; " \
	"elif test \\\\\"${ispxeboot}\\\\\" = 1; then " \
		"run boot_pxe; " \
	"else " \
		"run boot_runmode; "\
	"fi"

#define REAL_PREBOOT \
	"setenv silent 1; " \
	"nand lock tight; " \
	"ubi part boot-config; " \
	"run readsoftdip; " \
	"run readcplddip; " \
	"run readbootmode; " \
	"run read_usb0_otgsc_id;" \
	"run evaldip; " \
	"if test -n \\\\\"$isforcedrecoverymode\\\\\"; then " \
		"if test -n \\\\\"$isconsoleout\\\\\"; then " \
			"setenv silent; " \
		"fi; " \
		"forcedrecovery=1; " \
		"run recoverycmd; " \
	"else " \
		"run fpgaloadcmd; " \
		"run consoleoutcmd; " \
		"if test -n \\\\\"$isipreset\\\\\"; then " \
			"run ipresetcmd; " \
		"fi; " \
	"fi;"


#endif /* __CONFIG_NICOMMON_ENV_H */
