
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/gpio.h>

#include <video/jzpanel.h>
#include <video/panel-top035.h>


struct top035 {
	struct top035_platform_data *pdata;
};


static int nt39016_panel_init(
			      void **out_panel,
			      struct device *dev,
			      void *panel_pdata) {
  
	struct top035_platform_data *pdata = panel_pdata;
	struct top035 *panel;
	int ret;

	panel = devm_kzalloc(dev, sizeof(*panel), GFP_KERNEL);
	if (!panel) {
		dev_err(dev, "Failed to alloc panel data\n");
		return -ENOMEM;
	}

	panel->pdata = pdata;

	*out_panel = panel;

	/* Reserve GPIO pins. */

	ret = devm_gpio_request(dev, pdata->gpio_reset, "LCD panel reset");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel reset pin: %d\n", ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_clock, "LCD 3-wire clock");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire clock pin: %d\n",
			ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_enable, "LCD 3-wire enable");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire enable pin: %d\n",
			ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_data, "LCD 3-wire data");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire data pin: %d\n",
			ret);
		return ret;
	}

	/* Set initial GPIO pin directions and value. */

	gpio_direction_output(pdata->gpio_clock,  1);
	gpio_direction_output(pdata->gpio_enable, 1);
	gpio_direction_output(pdata->gpio_data,   0);

	return 0;
}

static void top035_panel_exit(void *panel) {}

static void top035_panel_enable(void *panel) {
	struct top035_platform_data *pdata = ((struct top035 *)panel)->pdata;
	int i;

	/* Reset LCD panel. */
	gpio_direction_output(pdata->gpio_reset, 0);
	udelay(50);
	gpio_direction_output(pdata->gpio_reset, 1);
}

static void top035_panel_disable(void *panel) {
	struct top035_platform_data *pdata = ((struct top035 *)panel)->pdata;
}

struct panel_ops top035_panel_ops = {
	.init		= top035_panel_init,
	.exit		= top035_panel_exit,
	.enable		= top035_panel_enable,
	.disable	= top035_panel_disable,
};
