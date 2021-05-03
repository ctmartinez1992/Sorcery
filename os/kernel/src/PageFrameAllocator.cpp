#include "PageFrameAllocator.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize) {
    if (Initialized) return;

    Initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;
    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    for (size_t i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (descriptor->type == 7) {  // type = EfiConventionalMemory
            if (descriptor->numPages * 4096 > largestFreeMemSegSize) {
                largestFreeMemSeg = descriptor->physAddr;
                largestFreeMemSegSize = descriptor->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    InitBitmap(bitmapSize, largestFreeMemSeg);

    LockPages(&PageBitmap, PageBitmap.Size / 4096 + 1);

    for (size_t i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (descriptor->type != 7) {  // type is not efiConventionalMemory
            ReservePages(descriptor->physAddr, descriptor->numPages);
        }
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddress;
    for (size_t i = 0; i < bitmapSize; i++) {
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
    }
}

void PageFrameAllocator::FreePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void PageFrameAllocator::LockPage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount) {
    for (size_t i = 0; i < pageCount; i++) {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount) {
    for (size_t i = 0; i < pageCount; i++) {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true) return;
    PageBitmap.Set(index, true);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount) {
    for (size_t i = 0; i < pageCount; i++) {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReleasePage(void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false) return;
    PageBitmap.Set(index, false);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void PageFrameAllocator::ReleasePages(void* address, uint64_t pageCount) {
    for (size_t i = 0; i < pageCount; i++) {
        ReleasePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void* PageFrameAllocator::RequestPage() {
    for (uint64_t i = 0; i < PageBitmap.Size; i++) {
        if (PageBitmap[i] == true) {
            continue;
        }

        LockPage((void*)(i * 4096));
        return (void*)(i * 4096);
    }

    return NULL;  // Page Frame swap to file.
}

uint64_t PageFrameAllocator::GetFreeRAM() { return freeMemory; }
uint64_t PageFrameAllocator::GetUsedRAM() { return usedMemory; }
uint64_t PageFrameAllocator::GetReservedRAM() { return reservedMemory; }