/*
 * Command to create a new MAC address based on serial#
 */
#include <common.h>
#include <command.h>
#include <linux/ctype.h>

int do_defethaddr(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	char enetvar[32];
	char enetaddr[32];
	const char *serial;
	int index;

	if (argc != cmdtp->maxargs)
	{
		index = 0;
	}
	else
	{
		index = simple_strtoul(argv[1], NULL, 10);
	}

	serial = getenv("serial#");
	sprintf(enetaddr, "00:80:2f:%c%c:%c%c:%c%c", tolower(serial[1]) + index,
		tolower(serial[2]), tolower(serial[3]), tolower(serial[4]),
		tolower(serial[5]), tolower(serial[6]));

	sprintf(enetvar, index ? "eth%daddr" : "ethaddr", index);
	setenv(enetvar, enetaddr);

	return 0;
}

U_BOOT_CMD(
   defethaddr,   2,   0,   do_defethaddr,
   "create a mac address based on serial#.",
   " interface\n    - select the interface to generate the ethaddr for."
);

