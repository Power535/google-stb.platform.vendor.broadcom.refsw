/*
 * Linux user mode ttcp client to the rsock library
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2017,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 * $Id$
 */

#include <unistd.h>
#include "os.h"
#include "setup.h"
#include "ttcp.h"

int
main(int argc, char **argv)
{
	int rv;

	/* Prepare for remote socket calls */

	setup_rsock();

	rv = ttcp(argc, argv, 0);

	cleanup_rsock();

	exit(rv ? 1 : 0);

	/*NOTREACHED*/
	return 0;
}
