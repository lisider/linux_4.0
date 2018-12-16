#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

int pdr_probe(struct platform_device * pdev)
{
	printk("suws_kernel bdd +++ %s,%s,%d\n",__FILE__,__func__,__LINE__);
	printk("suws_kernel bdd --- %s,%s,%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

int pdr_remove(struct platform_device * pdev)
{
	printk("suws_kernel bdd +++ %s,%s,%d\n",__FILE__,__func__,__LINE__);
	printk("suws_kernel bdd --- %s,%s,%d\n",__FILE__,__func__,__LINE__);
	return 0;
}

static const struct of_device_id ids[] = {
    { .compatible = "chip-name"},
    {  },
};

struct platform_driver pdr =
{
	.driver=
	{
		.name = "noneed to fill in",
		.of_match_table = ids,

	},
	.probe = pdr_probe,
	.remove = pdr_remove,

};

int __init _driver_init(void)
{
	platform_driver_register(&pdr);
	return 0;

}

void __exit _driver_exit(void)
{
	platform_driver_unregister(&pdr);
	return ;

}

/********模块三要素**********/
module_init(_driver_init);
module_exit(_driver_exit);
MODULE_LICENSE("GPL");
