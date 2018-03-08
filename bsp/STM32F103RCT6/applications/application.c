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
#include "../../../components/lite_gui/widget/widget_dc.h"

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
    lite_gui_widget_dc_t dock_dc = malloc(sizeof(struct lite_gui_widget_dc));
    dock_dc = lite_gui_widget_dc_init(dock_dc, hardware_dc, 0, (rt_int16_t) (hardware_dc->height - 60),
                                      hardware_dc->width, 60);
    dock_dc->base.brush_color = rgb(0, 155, 255);
    dock_dc->base.clear((struct lite_gui_dc *) dock_dc);
    dock_dc->base.brush_color = rgb(173, 255, 255);
    for (rt_uint16_t i = 0; i < 4; ++i) {
        dock_dc->base.fill_rounded_rectangle((struct lite_gui_dc *) dock_dc, (rt_uint16_t) (4 + (55 + 5) * i), 5,
                                             (rt_uint16_t) (55 + (55 + 5) * i), 55,
                                             8);
    }
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
