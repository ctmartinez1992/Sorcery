#include <stdint.h>

#include "BasicRenderer.h"
#include "cstr.h"
#include "EFIMemory.h"

typedef struct
{
    FrameBuffer *framebuffer;
    PSF1_FONT *psf1_font;
    void *mMap; // EFI_MEMORY_DESCRIPTOR
    uint64_t mMapSize;
    uint64_t mMapDescriptorSize;
} BootInfo;

extern "C" void _start(BootInfo *bootInfo)
{
    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);

    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;
    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescriptorSize));
        newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
        newRenderer.Print(EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
        newRenderer.Color = 0xffff00ff;
        newRenderer.Print(" ");
        newRenderer.Print(to_string(descriptor->numPages * 4096 / 1024));
        newRenderer.Print(" KB");
        newRenderer.Color = 0xffffffff;
    }

    return;
}