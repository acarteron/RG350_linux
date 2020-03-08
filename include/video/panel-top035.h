#ifndef __PANEL_TOP035_H
#define __PANEL_TOP035_H

struct top035_platform_data {
	int gpio_reset;
	int gpio_clock;
	int gpio_enable;
	int gpio_data;
};

extern struct panel_ops top035_panel_ops;

#endif /* __PANEL_TOP035_H */
