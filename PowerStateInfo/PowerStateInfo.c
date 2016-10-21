#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/acpi.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <acpi/acpi_bus.h>
#include <linux/seq_file.h>	/* For seq_file */
#include <linux/proc_fs.h>	/* For access to /proc/acpi */
#include <linux/cpufreq.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Information about ACPI and PCI devices");
MODULE_AUTHOR("EasyPM");

#define DEVNAME "power_state_info"
#define PREFIX 	"Power State Info- "
#define MAX_DEVICE	32
#define NO_VALUE	-1  // For a register, if it has not a value set yet

  //  ┐┌ ┘└  ┤├ ┬ ┴  ┼  │ ─  	▲▼ ▶
#define BEGIN_TAB "┌──────────────┬────────┬────────┬────┬─────┬────────────────┬─────┬────┐\n" \
		  "│ PCI Adress   │ Vendor │ Device │ St │ Dep │     Driver     │ pSt │ PM │\n" \
 		  "├──────────────┼────────┼────────┼────┼─────┼────────────────┼─────┼────┤\n"
#define ROW_TAB	  "│ %12.12s │ %#06x │ %#06x │ D%.1d │ %.1d %s │ %-15.15s│%4.1d │%3.1d │ "
#define END_TAB	  "└──────────────┴────────┴────────┴────┴─────┴────────────────┴─────┴────┘\n"

#define BOLD 	"\033[0;1m"
#define RED 	"\033[0;31m"
#define GREEN 	"\033[0;32m"
#define NF 	"\033[0m" // No Formating

#define UP 	"\033[0;32m▲\033[0m"
#define DOWN	"\033[0;31m▼\033[0m"
#define EQUAL	"▶"

/* Allow user to print more information on demand */
#define PRINT_ACPI_HANDLE	(1 << 0)
#define	PRINT_CLASS		(1 << 1)
#define	PRINT_DRIVER_NAME	(1 << 2)

static int flags;


extern struct proc_dir_entry *acpi_root_dir;

/* Register to record previous power counter value */
static int count_reg[MAX_DEVICE];
static int index;


// static struct notifier_block psi_notifier_block = {
// 	.notifier_call = NULL
// };


/**
 * Compare two int value, and return a string containing either "▲", "▼" or "▶"
 * to indicate if the new value has, respectivly, increased, decreased or stayed
 * the same.
 */
static char* compare(int old, int new)
{
	if (old == NO_VALUE || old == new) {
		return EQUAL;
	}
	return old < new? UP : DOWN;
}


static void* psi_start(struct seq_file *seqfp, loff_t *pos)
{
	index = 0;
	/* beginning a new sequence ? */
	if ((*pos)++ == 0) {
		//(*pos)++;
		seq_printf(seqfp, BEGIN_TAB);
		return pci_get_device(PCI_ANY_ID, PCI_ANY_ID, NULL);
	} else {
		/* no => it's the end of the sequence, return end to stop reading */
		*pos = 0;
		return NULL;
	}
}

static void* psi_next(struct seq_file *s, void *v, loff_t *pos)
{
	index++;
	return pci_get_device(PCI_ANY_ID, PCI_ANY_ID, (struct pci_dev *)v);
}

/**
 * Print only one line with device information.
 */
static int psi_show(struct seq_file *seqfp, void *p)
{
	struct pci_dev 	   *pdev = (struct pci_dev *)p;
	struct device 	   *dev  = &pdev->dev;
	struct acpi_device *adev = ACPI_COMPANION(dev);
	acpi_handle 	  handle = ACPI_HANDLE(dev);
	//struct pci_driver *driver = NULL;
 	struct acpi_buffer buf 	 = { ACPI_ALLOCATE_BUFFER, NULL };

	seq_printf(seqfp, ROW_TAB,
		dev_name(dev),
		pdev->vendor,
		pdev->device,
		pdev->current_state,
		dev->power.usage_count.counter,
		compare(count_reg[index], dev->power.usage_count.counter),
		pdev->driver ? pdev->driver->name : "",
		adev ? adev->power.state : 0,
		adev ? acpi_device_power_manageable(adev) : 0
	);
	count_reg[index] = dev->power.usage_count.counter;


	if (flags & PRINT_DRIVER_NAME) {
		seq_printf(seqfp, "%.s ", dev->pm_domain? "pmdomain  ":"nopmdomain");
		if (dev->type)	seq_printf(seqfp, "%s ", dev->type->pm? "type  ":"notype");
		if (dev->class)	seq_printf(seqfp, "%s ", dev->class->pm? "class  ":"noclass");
		if (dev->bus)	seq_printf(seqfp, "%s ", dev->bus->pm? 	"bus  ":"nobus");
		// seq_printf(seqfp, "%p ", dev->bus->pm); // = Adresse du bus.pm != adresse de driver.pm
		//seq_printf(seqfp, "%s ", dev->bus->dev_name); // null

		//seq_printf(seqfp, " disable_depth :%.3d ", pdev->dev.power.disable_depth);
		//seq_printf(seqfp, " no_callbacks :%.3d ", pdev->dev.power.no_callbacks);

		seq_printf(seqfp, " %13s ", dev_driver_string(dev));
	}

	if (flags & PRINT_CLASS) {
		seq_printf(seqfp, "%6x ", pdev->class);
	}

	if ((flags & PRINT_ACPI_HANDLE) && handle) {
		acpi_get_name(handle, ACPI_FULL_PATHNAME, &buf);
		seq_printf(seqfp, "%s", (char *)buf.pointer);
	}


	seq_printf(seqfp, "\n");
	return 0;
}


static void psi_stop(struct seq_file *seqfp, void *v)
{
	seq_printf(seqfp, END_TAB);
}


static struct seq_operations psi_ops = {
	.start = psi_start,
	.next  = psi_next,
	.stop  = psi_stop,
	.show  = psi_show
};


/**
 * FILE OPEERATION
 */
static int psi_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &psi_ops);
}

static ssize_t psi_write (struct file *file, const char __user *in, size_t size, loff_t *off)
{
	printk(KERN_INFO PREFIX "Write called with %s\n", in);
	// flags |= PRINT_DRIVER_NAME; // Set the mask
	flags ^= PRINT_DRIVER_NAME; // Flip the mask
	return size;
}

static struct file_operations fops = {
	.open    = psi_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
	.write	 = psi_write,
};


/**
 * CPUFREQ NOTIFIER
 */
#ifdef CONFIG_CPU_FREQ
static int psi_cpufreq_transition(struct notifier_block *nb,
                                  unsigned long val, void *data)
{
	// struct device *dev;
 	//dev = container_of(nb, struct davinci_i2c_dev, freq_transition); /* see linux/kernel.h for container_of */
 	if (val == CPUFREQ_PRECHANGE) {
		printk(KERN_INFO PREFIX "CPUFREQ Prechange\n");
        	// wait_for_completion(&dev->xfr_complete);
                // davinci_i2c_reset_ctrl(dev, 0);
        } else if (val == CPUFREQ_POSTCHANGE) {
		printk(KERN_INFO PREFIX "CPUFREQ Postchange\n");
                // i2c_davinci_calc_clk_dividers(dev);
                // davinci_i2c_reset_ctrl(dev, 1);
        }

        return 0;
}

static struct notifier_block psi_notifier_block = {
	.notifier_call = psi_cpufreq_transition
};

static int psi_cpufreq_register(void)
{
        return cpufreq_register_notifier(&psi_notifier_block, CPUFREQ_TRANSITION_NOTIFIER);
}

static void psi_cpufreq_deregister(void)
{
	cpufreq_unregister_notifier(&psi_notifier_block, CPUFREQ_TRANSITION_NOTIFIER);
}
#else
static int psi_cpufreq_register(void)
{
        return 0;
}

static void psi_cpufreq_deregister(void)
{
}
#endif


/**
 * DRIVER INIT AND EXIT
 */
static int __init psi_driver_init(void)
{
	int i;
	struct proc_dir_entry *acpi_entry;

	acpi_entry = proc_create(DEVNAME, 0644, acpi_root_dir, &fops);
	if (acpi_entry == NULL) {
		printk(KERN_INFO PREFIX "Can't create /proc/acpi entry\n");
		return -ENOMEM;
	}

	//Initialize pm_count register
	for (i=0; i<MAX_DEVICE; i++) {count_reg[i] = NO_VALUE;}
	flags = (PRINT_ACPI_HANDLE | PRINT_CLASS);

	printk(KERN_INFO PREFIX "Cpu freq driver : %s\n", cpufreq_get_current_driver());
	psi_cpufreq_register();

	printk(KERN_INFO PREFIX "Module loaded\n");
	return 0;
}

static void __exit psi_driver_exit(void)
{
	psi_cpufreq_deregister();
	remove_proc_entry(DEVNAME, acpi_root_dir);
	printk(KERN_INFO PREFIX "Exiting module\n");
}

module_init(psi_driver_init);
module_exit(psi_driver_exit);
