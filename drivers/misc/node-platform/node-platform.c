#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>

static int num=10;

int get_mynum(void){
	return num;
}
EXPORT_SYMBOL_GPL(get_mynum);


static irqreturn_t irq_int_handler(void)
{
    /* do something */
    return IRQ_HANDLED;
}

int pdr_probe(struct platform_device * pdev)
{
    bool prop_bool = false;
    int prop_int = 0;
    char *prop_string = NULL;

    unsigned int irq_number = 0; // sw interrupt number
    int ret = 0;

	printk("suws_kernel cmdline num:%d %s,%s,%d\n",num,__FILE__,__func__,__LINE__);
	printk("suws_kernel bdd +++ %s,%s,%d\n",__FILE__,__func__,__LINE__);
    if (pdev->dev.of_node){

        prop_bool = of_property_read_bool(pdev->dev.of_node,"prop_bool");
        printk("suws_kernel bdd prop_bool:%d,%s,%s,%d\n",prop_bool,__FILE__,__func__,__LINE__);

        of_property_read_u32_index(pdev->dev.of_node,"prop_int",0,&prop_int);
        printk("suws_kernel bdd prop_int:%d,%s,%s,%d\n",prop_int,__FILE__,__func__,__LINE__);

        of_property_read_string(pdev->dev.of_node,"prop_string",(const char **)&prop_string);
        printk("suws_kernel bdd prop_string:%s,%s,%s,%d\n",prop_string,__FILE__,__func__,__LINE__);

        irq_number = irq_of_parse_and_map(pdev->dev.of_node,0);
        printk("suws_kernel bdd irq_number:%d,%s,%s,%d\n",irq_number,__FILE__,__func__,__LINE__);
        ret = request_irq(irq_number, (irq_handler_t) irq_int_handler, IRQ_TYPE_LEVEL_HIGH, "node_platform", NULL);
        if (ret > 0) {
            printk("request_irq failed\n");
            return -1;
        }
    }
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

module_param(num,int,S_IRUGO);
/********模块三要素**********/
module_init(_driver_init);
module_exit(_driver_exit);
MODULE_LICENSE("GPL");
