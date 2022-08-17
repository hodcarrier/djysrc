/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Timothy Manning <timothy@yaffs.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#if 0 // DJYOS������

#include "test_yaffs_freespace_EINVAL.h"



int test_yaffs_freespace_EINVAL(void)
{
	int output=0;
	int error=0;
	output = yaffs_freespace("non-existing-mount-point");
	if (output<0){
		error=yaffs_get_error();
		if (abs(error)==EINVAL){
			return 1;
		} else {
			print_message("different error than expected\n",2);
			return -1;
		}
	} else {
		print_message("got the free space of a non existing mountpoint (which is a bad thing)\n",2);
		return -1;
	}
}


int test_yaffs_freespace_EINVAL_clean(void)
{
	return 1;
}

#endif // DJYOS������
