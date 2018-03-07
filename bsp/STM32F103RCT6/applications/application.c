/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */


#include <rtthread.h>
#include <lcd_port.h>
#include <lite_gui.h>

#ifdef RT_USING_DFS
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#endif


ALIGN(RT_ALIGN_SIZE)

void rt_init_thread_entry(void *parameter) {
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

    /* Filesystem Initialization */
#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
    /* mount sd card fat partition 1 as root directory */
    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0) {
      rt_kprintf("File System initialized!\n");
    } else
      rt_kprintf("File System initialzation failed!\n");
#endif /* RT_USING_DFS */

#ifdef USING_LITE_GUI
    rt_hw_lcd_init();
    lite_gui_hw_init(rt_device_find("lcd"));
#endif
}

int rt_application_init(void) {
    rt_thread_t init_thread;

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread =
            rt_thread_create("init", rt_init_thread_entry, RT_NULL, 2048, 8, 20);
#else
    init_thread =
            rt_thread_create("init", rt_init_thread_entry, RT_NULL, 2048, 80, 20);
#endif

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}
