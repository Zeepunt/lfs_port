/*
 * Copyright (c) 2024 by Zeepunt, All Rights Reserved.
 */
#ifndef __LFS_PORT_LINUX_H__
#define __LFS_PORT_LINUX_H__

#include <lfs_port/lfs_port.h>

int lfs_port_linux_ram_init(lfs_port_cfg_t *config, const uint8_t *path);

void lfs_port_linux_ram_deinit(void);

lfs_t *lfs_port_linux_ram_handle_get(void);

#endif /* __LFS_PORT_LINUX_H__ */
