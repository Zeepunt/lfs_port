/*
 * Copyright (c) 2024 by Zeepunt, All Rights Reserved.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <lfs_port/lfs_port_linux.h>

/* Ram Interface */
static FILE *_ram_fd = NULL;
static uint8_t *_ram_buf = NULL;
static uint32_t _ram_buf_len = 0;
static lfs_port_info_mgmt_t _ram_info;

static int _lfs_ram_read(const struct lfs_config *config,
                         lfs_block_t block,
                         lfs_off_t offset,
                         void *buffer,
                         lfs_size_t size)
{
    if (NULL == _ram_buf) {
        return -1;
    }

    memcpy(buffer, _ram_buf + (block * config->block_size) + offset, size);
    return 0;
}

static int _lfs_ram_write(const struct lfs_config *config,
                          lfs_block_t block,
                          lfs_off_t offset,
                          const void *buffer,
                          lfs_size_t size)
{
    if (NULL == _ram_buf) {
        return -1;
    }

    memcpy(_ram_buf + (block * config->block_size) + offset, buffer, size);
    return 0;
}

static int _lfs_ram_erase(const struct lfs_config *config,
                          lfs_block_t block)
{
    if (NULL == _ram_buf) {
        return -1;
    }

    memset(_ram_buf + (block * config->block_size), 0xff, config->block_size);
    return 0;
}

static int _lfs_ram_sync(const struct lfs_config *config)
{
    if (NULL != _ram_fd) {
        fseek(_ram_fd, 0, SEEK_SET);
        fwrite(_ram_buf, 1024 * 1024, 1, _ram_fd);
    }
    return 0;
}

int lfs_port_linux_ram_init(lfs_port_cfg_t *config, const uint8_t *path)
{
    int ret = 0; 

    memset(&_ram_info, 0, sizeof(lfs_port_info_mgmt_t));

    _ram_info.cfg.read  = _lfs_ram_read;
    _ram_info.cfg.prog  = _lfs_ram_write;
    _ram_info.cfg.erase = _lfs_ram_erase;
    _ram_info.cfg.sync  = _lfs_ram_sync;

    _ram_info.cfg.block_size     = config->block;
    _ram_info.cfg.block_count    = config->size / config->block;
    _ram_info.cfg.lookahead_size = _ram_info.cfg.block_count;

    _ram_info.cfg.block_cycles = 1000;

    /**
     * 对于 cache_size 的要求:
     * 1. block_size % cache_size == 0
     * 2. cache_size % read_size == 0
     * 3. cache_size % prog_size == 0
     */
    _ram_info.cfg.cache_size = 512;
    _ram_info.cfg.read_size  = 512;
    _ram_info.cfg.prog_size  = 512;

    _ram_buf_len = config->size;
    _ram_buf = (uint8_t *)malloc(_ram_buf_len * sizeof(uint8_t));
    if (NULL == _ram_buf) {
        printf("[lfs_ram] malloc %d fail.", _ram_buf_len);
        ret = -1;
        goto exit;
    }
    memset(_ram_buf, 0xff, _ram_buf_len);

    if (NULL != path) {
        _ram_fd = fopen(path, "wb+");
        if (NULL == _ram_fd) {
            printf("[lfs_ram] fopen %s fail.", path);
        }
    }

    ret = lfs_format(&_ram_info.handle, &_ram_info.cfg);
    if (LFS_ERR_OK != ret) {
        printf("[lfs_ram] lfs format fail: %d.", ret);
        ret = -1;
        goto exit;
    }

    ret = lfs_mount(&_ram_info.handle, &_ram_info.cfg);
    if (LFS_ERR_OK != ret) {
        printf("[lfs_ram] lfs mount fail: %d.", ret);
        ret = -1;
        goto exit;
    }

    return 0;

exit:
    if (NULL != _ram_buf) {
        free(_ram_buf);
        _ram_buf = NULL;
        _ram_buf_len = 0;
    }

    if (NULL != _ram_fd) {
        fclose(_ram_fd);
        _ram_fd = NULL;
    }
    return ret;
}

void lfs_port_linux_ram_deinit(void)
{
    int ret = 0;

    ret = lfs_unmount(&_ram_info.handle);
    if (LFS_ERR_OK != ret) {
        printf("[lfs_ram] lfs unmount fail: %d.", ret);
    }

    if (NULL != _ram_buf) {
        free(_ram_buf);
        _ram_buf = NULL;
        _ram_buf_len = 0;
    }

    if (NULL != _ram_fd) {
        fclose(_ram_fd);
        _ram_fd = NULL;
    }
}

lfs_t *lfs_port_linux_ram_handle_get(void)
{
    return &_ram_info.handle;
}

/* Fuse Interface */