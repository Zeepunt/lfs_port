/*
 * Copyright (c) 2024 by Zeepunt, All Rights Reserved.
 */
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <lfs/lfs.h>
#include <lfs_port/lfs_port_linux.h>

static void _print_usage(char *cmd)
{
    printf(" Usage : %s <options>\r\n", cmd);
    printf("Option : -c source directory\r\n");
    printf("         -d output directory\r\n");
    printf("         -s lfs image size, unit:KByte\r\n");
    printf("         -b lfs image block size, unit:KByte\r\n");
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int option = 0;

    uint8_t *src_dir = NULL;
    uint8_t *dst_dir = NULL;

    lfs_t *lfs_handle = NULL;
    lfs_file_t lfs_file;
    lfs_port_cfg_t lfs_port_cfg;

    memset(&lfs_port_cfg, 0, sizeof(lfs_port_cfg_t));
    lfs_port_cfg.block = 4 * 1024; /* default */

    while (-1 != (option = getopt(argc, argv, "c:d:s:b:"))) {
        switch (option) {
        case 'c': {
            src_dir = optarg;
            break;
        }

        case 'd': {
            dst_dir = optarg;
            break;
        }

        case 's': {
            lfs_port_cfg.size = atoi(optarg) * 1024;
            break;
        }

        case 'b': {
            lfs_port_cfg.block = atoi(optarg) * 1024;
            break;
        }

        default:
            break;
        }
    }

    if ((NULL == dst_dir) || (0 == lfs_port_cfg.size)) {
        _print_usage(argv[0]);
        ret = -1;
        goto exit;
    }

    if (0 != (lfs_port_cfg.size % 4)) {
        printf("size %d must be 4 aligned.\r\n", lfs_port_cfg.size);
        ret = -1;
        goto exit;
    }

    if (NULL != src_dir) {

    }

    printf("cur_dir: %s\r\n", src_dir);
    printf("dst_dir: %s\r\n", dst_dir);
    printf("size   : %d\r\n", lfs_port_cfg.size);
    printf("block  : %d\r\n", lfs_port_cfg.block);

    ret = lfs_port_linux_ram_init(&lfs_port_cfg, dst_dir);

    lfs_handle = lfs_port_linux_ram_handle_get();
    ret = lfs_file_open(lfs_handle, &lfs_file, "test.txt", LFS_O_RDWR | LFS_O_CREAT);
    if (LFS_ERR_OK != ret) {
        printf("lfs_file_open fail: %d.", ret);
    }

    char *tmp_buf = "hahahahah";
    ret = lfs_file_write(lfs_handle, &lfs_file, tmp_buf, strlen(tmp_buf));
    if (0 != ret) {
        printf("lfs_file_write fail: %d.", ret);
    }

    ret = lfs_file_close(lfs_handle, &lfs_file);

    lfs_port_linux_ram_deinit();
exit:
    return ret;
}
