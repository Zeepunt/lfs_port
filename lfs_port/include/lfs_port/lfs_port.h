/*
 * Copyright (c) 2024 by Zeepunt, All Rights Reserved.
 */
#ifndef __LFS_PORT_H__
#define __LFS_PORT_H__

#include <lfs/lfs.h>

/**
 * 对于 Nor Flash 来说:
 * 1. Page  : 最小可编程单位, 单位 Byte
 * 2. Sector: 由多个 Page 组成
 * 3. Block : 由多个 Sector 组成
 *
 * 以华邦的 W25Q128JV 为例:
 *   Page: 256 Bytes, Sector: 4KB, Block: 64KB
 *   256 * Block = 256 * (16 * Sector) = 256 * (16 * ())
 */
typedef struct lfs_port_config {
    unsigned int size;
    unsigned int block;
    unsigned int sector;
    unsigned int page;
} lfs_port_cfg_t;

typedef struct lfs_port_information_management {
    lfs_t handle;
    struct lfs_config cfg;
} lfs_port_info_mgmt_t;

#endif /* __LFS_PORT_H__ */
