#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>

static irqreturn_t irq_int_handler(void)
{
    /* do something */
    return IRQ_HANDLED;
}

//---------------------------- 节点序列中的 节点1

static char node_one_buf[32] = {0};

static ssize_t sysfs_demo_show_node_one(struct kobject *kobj, struct kobj_attribute *attr, char * buf)
{
    pr_info("enter show node: %s\n", attr->attr.name);
    return sprintf(buf, "%s: %s\n", attr->attr.name, node_one_buf);
}

static ssize_t sysfs_demo_store_node_one(struct kobject *kobj, struct kobj_attribute *attr, const char * buf, size_t n)
{
    pr_info("enter write node: %s\n", attr->attr.name);
    snprintf(node_one_buf,(n <= sizeof(node_one_buf)) ? n : sizeof(node_one_buf), "%s", buf);
    return n;
}

static struct kobj_attribute node_one_attribute =
    __ATTR(node_one, S_IWUSR|S_IRUGO, sysfs_demo_show_node_one, sysfs_demo_store_node_one); //读写属性

//---------------------------- 节点序列中的 节点2
static ssize_t sysfs_demo_show_node_two(struct kobject *kobj, struct kobj_attribute *attr, char * buf)
{
    return sprintf(buf, "%s\n", attr->attr.name);
}

static struct kobj_attribute node_two_attribute =
    __ATTR(node_two, S_IRUGO, sysfs_demo_show_node_two, NULL); // 读属性

//---------------------- 文件序列包括 两个节点
static struct attribute *sysfs_demo_attributes[] = {
    &node_one_attribute.attr,
    &node_two_attribute.attr,
    NULL
};

static const struct attribute_group sysfs_demo_attr_group = {
    .attrs = sysfs_demo_attributes,
};

int pdr_probe(struct platform_device * pdev)
{
    bool prop_bool = false;
    int prop_int = 0;
    char *prop_string = NULL;

    unsigned int irq_number = 0; // sw interrupt number
    int ret = 0;

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

	//------------------ 在 目录下 创建 一个 文件序列

	// platform_driver_register 的 驱动 会在 /sys/devices/platform/xxx 下面 创建文件 , xxx 为 dts 中的 节点名
	if(sysfs_create_group(&pdev->dev.kobj, &sysfs_demo_attr_group) ) { //在 /sys/devices/platform/node-platform 下创建文件序列
		pr_err("sysfs_create_group failed\n");
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

/********模块三要素**********/
module_init(_driver_init);
module_exit(_driver_exit);
MODULE_LICENSE("GPL");
