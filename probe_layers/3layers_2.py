from __future__ import print_function
from bcc import BPF
from time import sleep

# BPF program
bpf_text = """
#include <uapi/linux/ptrace.h>

BPF_HASH(start, u32);

int vfs_entry(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 ts = bpf_ktime_get_ns();
    start.update(&pid, &ts);
    return 0;
}

int vfs_return(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 *tsp = start.lookup(&pid);
    if (tsp != 0) {
        u64 ts = bpf_ktime_get_ns();
        u64 delta = ts - *tsp;
        bpf_trace_printk("[VFS] PID: %d took %llu ns \\n", pid, delta);
        start.delete(&pid);
    }
    return 0;
}


int ext4_read_entry(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 ts = bpf_ktime_get_ns();
    start.update(&pid, &ts);
    return 0;
}

int ext4_read_return(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 *tsp = start.lookup(&pid);
    if (tsp != 0) {
        u64 ts = bpf_ktime_get_ns();
        u64 delta = ts - *tsp;
        bpf_trace_printk("[File system] PID: %d took %llu ns \\n", pid, delta);
        start.delete(&pid);
    }
    return 0;
}


int submit_bio_entry(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 ts = bpf_ktime_get_ns();
    start.update(&pid, &ts);
    return 0;
}

int submit_bio_return(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid();
    u64 *tsp = start.lookup(&pid);
    if (tsp != 0) {
        u64 ts = bpf_ktime_get_ns();
        u64 delta = ts - *tsp;
        bpf_trace_printk("[Block layer] PID: %d took %llu ns \\n", pid, delta);
        start.delete(&pid);
    }
    return 0;
}
"""

# load BPF program
b = BPF(text=bpf_text)

b.attach_kprobe(event="vfs_readv", fn_name="vfs_entry")
b.attach_kprobe(event="ext4_file_read_iter", fn_name="ext4_read_entry")
b.attach_kprobe(event="submit_bio_noacct", fn_name="submit_bio_entry")

b.attach_kretprobe(event="vfs_readv", fn_name="vfs_return")
b.attach_kretprobe(event="ext4_file_read_iter", fn_name="ext4_read_return")
b.attach_kretprobe(event="submit_bio_noacct", fn_name="submit_bio_return")


try:
    sleep(99999999)
except KeyboardInterrupt:
    pass
