/*
 * Copyright (c) 2010-2010 Chelsio, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * /drivers/net/cxgb4/sge.c+ * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * dump of firmware files generated using:
 *	hexdump -v -e '"\t" 16/1 "0x%02x, " "\n"'
 */

#include <linux/string.h>

#define FW_CFNAME "cxgb4/t4-config.txt"

static u8 t4fw_data[] = {
#  include "./t4fw.txt"
};

static u8 t4cfg_data[] = {
#  include "./t4cfg.txt"
};

static inline void t4_embedded_fw_cfg_free(const struct firmware *fw_cfg)
{
	kfree(fw_cfg);
}

static inline int t4_embedded_fw_cfg_load(const struct firmware **fw_cfg,
					  const char *fw_cfg_name, int *embed)
{
	struct firmware *fwcfg;

	*fw_cfg = fwcfg = kzalloc(sizeof(*fwcfg), GFP_KERNEL);
	if (!fwcfg) {
		printk(KERN_ERR "%s: kmalloc(struct firmware) failed\n",
				__FUNCTION__);
		return -ENOMEM;
	}

	if (strcmp(fw_cfg_name, FW_CFNAME) == 0) {
		fwcfg->data = t4cfg_data;
		fwcfg->size = sizeof(t4cfg_data);
	} else {
		fwcfg->data = t4fw_data;
		fwcfg->size = sizeof(t4fw_data);
	}

	*embed = 1;
	return 0;
}
