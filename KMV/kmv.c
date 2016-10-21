#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/acpi.h>		/* For ACPI_COMPANION */
#include <linux/pm.h>
#include <acpi/acpi_bus.h>
#include <linux/pm_runtime.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>	/* For copy_from_user */

#include "kmv.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("EasyPM Kernel module");
MODULE_AUTHOR("EasyPM");

#define MAX_DEVICE 		32
#define PROCFS_MAX_SIZE		1024
#define PROCFS_NAME 		"easypm_km"
#define PREFIX 	  		"EPM_KM: "

#ifdef DEBUG
#define DEBUG_PRINT(...) printk(KERN_INFO PREFIX __VA_ARGS__)
#else
#define DEBUG_PRINT(...) do{} while (0)
#endif


extern struct proc_dir_entry *acpi_root_dir; /* /proc/acpi */



/* Generated */
enum hw_ids {
        audio_snd,
        ethernet,
        vga,
};

static struct hw_comp hw_audio_snd = {
        .name 	= "Audio snd",
        .id	= audio_snd,
        //.pdev	= pci_get_device(0x8086, 0x8c20, NULL)
};
static struct hw_comp hw_ethernet = {
        .name 	= "Ethernet",
        .id	= ethernet,
        //.pdev	= pci_get_device(0x14e4, 0x166a, NULL)
};
static struct hw_comp hw_vga = {
        .name 	= "VGA Controller",
        .id	= vga,
        //.pdev	= pci_get_device(0x1002, 0x515e, NULL)
};


static struct hw_state hw_audio_on = {
        .hw          	= &hw_audio_snd,
        .power_state 	= PCI_D0,
        .attr = {}
};
static struct hw_state hw_ethernet_on = {
        .hw     	= &hw_ethernet,
        .power_state    = PCI_D0,
        .attr = {}
};
static struct hw_state hw_vga_on = {
        .hw             = &hw_vga,
        .power_state    = PCI_D0,
        .attr = {}
};


static struct hw_state hw_audio_off = {
        .hw          	= &hw_audio_snd,
        .power_state 	= PCI_D3hot,
        .attr = {}
};
static struct hw_state hw_ethernet_off = {
        .hw     	= &hw_ethernet,
        .power_state    = PCI_D3hot,
        .attr = {}
};
static struct hw_state hw_vga_off = {
        .hw             = &hw_vga,
        .power_state    = PCI_D3hot,
        .attr = {}
};


static struct machine_state skypeOn_state = {
        .hw_states = {
                [audio_snd]     = &hw_audio_on,
                [ethernet]      = &hw_ethernet_on,
                [vga]           = &hw_vga_on,
        }
};
static struct machine_state skypeOff_state = {
        .hw_states = {
                [audio_snd]     = &hw_audio_off,
                [ethernet]      = &hw_ethernet_off,
                [vga]           = &hw_vga_off,
        }
};


static struct transition skypeOn_skypeOff = {
        .start  = &skypeOn_state,
        .end    = &skypeOff_state,
};
static struct transition skypeOff_skypeOn = {
        .start  = &skypeOff_state,
        .end    = &skypeOn_state,
};


static struct pm_policy main_policy = {
        .initial_state = &skypeOn_state,
        .transitions = {
                &skypeOn_skypeOff,
                &skypeOff_skypeOn,
        }
};
/* End generation */


static void change_machine_state(struct transition *tr)
{

        return;
}


static void resume(struct hw_comp *comp)
{
	int retval;
        struct pci_dev *pdev = comp->pdev;
        struct device *dev   = &pdev->dev;

	printk(KERN_INFO PREFIX "Resuming %s", comp->name);
	while (dev->power.usage_count.counter < comp->init_counter) {
		pm_runtime_get_noresume(dev); // usage_count++
	}

	retval = pm_runtime_resume(dev);
	DEBUG_PRINT("Resume return %d -- %s state:%d\n", retval, comp->name, pdev->current_state);
}

/**
 * suspend - Suspend a device
 * @hw_comp:	pointer to hardware component
 */
static void suspend(struct hw_comp *comp)
{
	int retval;
        struct pci_dev *pdev = comp->pdev;
        struct device *dev   = &pdev->dev;

	printk(KERN_INFO PREFIX "Suspending %s", comp->name);
	while (dev->power.usage_count.counter > 0) {
		pm_runtime_put_noidle(dev); // usage_count--
	}

	retval = pm_runtime_suspend(dev);
	DEBUG_PRINT("Suspend return %d -- %s state:%d\n", retval, comp->name, pdev->current_state);
}



static int kmv_show(struct seq_file *seqfp, void *p)
{
        struct transition *transitions = *main_policy.transitions;

        struct transition skOff = transitions[0];
        struct transition skOn  = transitions[1];

        struct machine_state skOff_st = *skOff.start;
        struct machine_state skOn_fi = *skOn.end;

        int i = 0;
        while (skOff_st.hw_states[i] != NULL) {
                struct hw_state *hw_st = skOff_st.hw_states[i];
                seq_printf(seqfp, "%s - %d %s\n", hw_st->hw->name, hw_st->hw->init_counter, hw_st->hw->pdev != NULL? "Not null" : "null");
                i++;
        }
	return 0;
}

static int kmv_open(struct inode *inode, struct file *file)
{
	return single_open(file, kmv_show, NULL);
}

/**
 * This function is called whenever something is write into /proc/acpi/kmv
 */
static ssize_t kmv_write (struct file *file, const char __user *in, size_t size, loff_t *off)
{
	static char buffer[PROCFS_MAX_SIZE];
	static size_t buffer_size;

	buffer_size = size;
	if (buffer_size > PROCFS_MAX_SIZE ) {
		buffer_size = PROCFS_MAX_SIZE;
	}

	if (copy_from_user(buffer, in, buffer_size)) {
       		return -EFAULT;
   	}
	DEBUG_PRINT("Write in /proc/acpi/%s : %.*s\n", PROCFS_NAME, (int)buffer_size - 1, buffer);

	return size;
}

static struct file_operations fops = {
	.open    = kmv_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
	.write	 = kmv_write,
};

static int __init kmv_driver_init(void)
{
	struct proc_dir_entry 	*acpi_entry;

	acpi_entry = proc_create(PROCFS_NAME, 0644, acpi_root_dir, &fops);
	if (acpi_entry == NULL) {
		printk(KERN_INFO PREFIX "Can't create /proc/acpi/%s entry\n", PROCFS_NAME);
		return -ENOMEM;
	}

        /* Generated */
	hw_audio_snd.pdev = pci_get_device(0x8086, 0x8c20, NULL);
        if (hw_audio_snd.pdev != NULL)
                hw_audio_snd.init_counter = hw_audio_snd.pdev->dev.power.usage_count.counter > 0?  hw_audio_snd.pdev->dev.power.usage_count.counter:1;

        hw_vga.pdev = pci_get_device(0x1002, 0x515e, NULL);
        if (hw_vga.pdev != NULL)
                hw_vga.init_counter = hw_vga.pdev->dev.power.usage_count.counter > 0?  hw_vga.pdev->dev.power.usage_count.counter:1;

        hw_ethernet.pdev = pci_get_device(0x14e4, 0x166a, NULL);
        if (hw_ethernet.pdev != NULL)
                hw_ethernet.init_counter = hw_ethernet.pdev->dev.power.usage_count.counter > 0?  hw_ethernet.pdev->dev.power.usage_count.counter:1;
        /* End of generation */

	printk(KERN_INFO PREFIX "/proc/acpi/%s created\n", PROCFS_NAME);
	return 0;
}

static void __exit kmv_driver_exit(void)
{
	remove_proc_entry(PROCFS_NAME, acpi_root_dir);
	DEBUG_PRINT("Exiting driver\n");
}

MODULE_LICENSE("GPL");
module_init(kmv_driver_init);
module_exit(kmv_driver_exit);
