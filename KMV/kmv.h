#ifndef KMV_H
#define KMV_H

#include <linux/types.h>
#include <linux/pci.h>

#define MAX_ATTRIBUTES  32 // Random number, can be modified
#define MAX_HW_COMP     32
#define MAX_TRANSITION  32

struct attributes {
        char *name;
        void *value;
};

/**
 * struct hw_comp - hardware component
 */
struct hw_comp {
        char *name;
        u32 id;
        struct pci_dev *pdev;
        int init_counter;       /* Inital value of dependencies counter */
        // list of effectors
        // list of events it can generate
        // list of probe fucntions
        // dependencies
};

/**
 * struct hw_state - an hardware state
 */
struct hw_state {
        struct hw_comp *hw;
        pci_power_t power_state;        // PCI_D0, etc..
        struct attributes attr[MAX_ATTRIBUTES];
};

struct machine_state {
        struct hw_state *hw_states[MAX_HW_COMP];
};

struct transition {
        struct machine_state *start;
        struct machine_state *end;
        // How to trigger this?
};

/**
 * struct pm_policy - a power management policy
 * @initial_state:      Initial state of the pm_policy
 * @transitions:        Array containing all transitions possibles in this policy
 *
 * A policy have an initial state, and is kind of a graph. A transition
 * represents an arrow in the graph, and machine_state is a node.
 */
struct pm_policy {
        struct machine_state *initial_state;
        struct transition *transitions[MAX_TRANSITION];
};

#endif /* KMV_H */
