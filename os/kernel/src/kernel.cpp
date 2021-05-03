#include <stdint.h>

#include "BasicRenderer.h"
#include "Bitmap.h"
#include "EFIMemory.h"
#include "PageFrameAllocator.h"
#include "cstr.h"
#include "memory.h"

typedef struct {
    FrameBuffer *framebuffer;
    PSF1_FONT *psf1_font;
    EFI_MEMORY_DESCRIPTOR *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescriptorSize;
} BootInfo;

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

uint8_t testBuffer[20];

extern "C" void _start(BootInfo *bootInfo) {
    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);

    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

    // Bitmap testBitmap;
    // testBitmap.Buffer = &testBuffer[0];
    // testBitmap.Set(0, false);
    // testBitmap.Set(1, true);
    // testBitmap.Set(2, false);
    // testBitmap.Set(3, false);
    // testBitmap.Set(4, true);
    // testBitmap.Set(5, true);
    // testBitmap.Set(6, false);

    // for (size_t i = 0; i < 20; i++) {
    //     newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    //     newRenderer.Print(testBitmap[i] ? "true" : "false");
    // }

    // newRenderer.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize)));

    // for (int i = 0; i < mMapEntries; i++)
    // {
    //     EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescriptorSize));
    //     newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    //     newRenderer.Print(EFI_MEMORY_TYPE_STRINGS[descriptor->type]);
    //     newRenderer.Color = 0xffff00ff;
    //     newRenderer.Print(" ");
    //     newRenderer.Print(to_string(descriptor->numPages * 4096 / 1024));
    //     newRenderer.Print(" KB");
    //     newRenderer.Color = 0xffffffff;
    // }

    PageFrameAllocator newAllocator;
    newAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    newAllocator.LockPages(&_KernelStart, kernelPages);

    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    newRenderer.Print("Free RAM: ");
    newRenderer.Print(to_string(newAllocator.GetFreeRAM() / 1024));
    newRenderer.Print(" KB ");
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    newRenderer.Print("Used RAM: ");
    newRenderer.Print(to_string(newAllocator.GetUsedRAM() / 1024));
    newRenderer.Print(" KB ");
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    newRenderer.Print("Reserved RAM: ");
    newRenderer.Print(to_string(newAllocator.GetReservedRAM() / 1024));
    newRenderer.Print(" KB ");
    newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};

    for (size_t i = 0; i < 20; i++) {
        void *address = newAllocator.RequestPage();
        newRenderer.Print(to_hstring((uint64_t)address));
        newRenderer.CursorPosition = {0, newRenderer.CursorPosition.y + 16};
    }

    return;
}