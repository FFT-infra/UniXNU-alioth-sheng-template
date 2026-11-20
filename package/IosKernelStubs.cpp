// The two symbols the macOS profile patches define for device components that the iPadOS build links
// too. Each body keeps the behavior the caller documents for the symbol's absence.
#include <stdint.h>
#include <mach/vm_types.h>
#include <mach/kern_return.h>

extern "C" {
class IOMemoryDescriptor;

// memwired-kc-segment-release / payload-addon-release (macOS profile): a range the routine would not
// take is kept, so answering "not prepared" keeps the GPU addon's loader range instead of freeing it.
bool ml_static_mfree_prepare(vm_offset_t vaddr, vm_size_t size) {
    (void)vaddr;
    (void)size;
    return false;
}

// gpupool-device-pager-close (macOS profile): without the report the pool does not reuse the pages of
// a grant that had a view (NativeAdrenoPool.h), which is the documented behavior without the hook.
kern_return_t iokit_physical_descriptor_close_notify(IOMemoryDescriptor *, void (*)(void *), void *) {
    return KERN_FAILURE;
}

// The VideoToolbox host (drivers/QcomIris) and the video probe are macOS-only in this build
// (build-gop-legacy-display-27.py); the boot log and the legacy display still reference their
// counters and hook, so the symbols keep their zero and no-op meaning here.
volatile uint32_t vt_paravirt_stage, vt_paravirt_opens, vt_paravirt_messages, vt_paravirt_refused;
volatile uint32_t vt_host_sessions, vt_host_frames_in, vt_host_frames_out, vt_host_frames_dropped;
volatile uint32_t vt_host_errors, vt_host_error_stage;

void sheng_video_probe(void) {
}
}
