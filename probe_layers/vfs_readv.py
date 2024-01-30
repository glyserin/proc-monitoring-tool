from __future__ import print_function
from bcc import BPF
from time import sleep

# BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>

BPF_HASH(start, u32);

int do_entry(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 ts = bpf_ktime_get_ns();
    start.update(&pid, &ts);
    return 0;
}

int do_return(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 *tsp = start.lookup(&pid);
    if (tsp != 0) {
        u64 ts = bpf_ktime_get_ns();
        u64 delta = ts - *tsp;
        bpf_trace_printk("PID: %d took %llu ns \\n", pid, delta);
        start.delete(&pid);
    }
    return 0;
}
"""

# load BPF program
b = BPF(text=bpf_text)
b.attach_kprobe(event="vfs_readv", fn_name="do_entry")
b.attach_kretprobe(event="vfs_readv", fn_name="do_return")


# sleep until Ctrl-C
try:
    sleep(99999999)
except KeyboardInterrupt:
    pass
