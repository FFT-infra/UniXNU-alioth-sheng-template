// The two symbols the macOS profile patches define for device components that the iPadOS build links
// too. Each body keeps the behavior the caller documents for the symbol's absence.
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
}
