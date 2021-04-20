#include <stdint.h>

#include "BasicRenderer.h"
#include "cstr.h"

extern "C" void _start(FrameBuffer* framebuffer, PSF1_FONT* psf1_font) {
    BasicRenderer newRenderer = BasicRenderer(framebuffer, psf1_font);
    newRenderer.Print(to_string((uint64_t)1234976));
    newRenderer.CursorPosition = {0, 16};
    newRenderer.Print(to_string((int64_t)-1234976));
    newRenderer.CursorPosition = {0, 32};
    newRenderer.Print(to_string((double)-3.141351));
    newRenderer.CursorPosition = {0, 48};
    newRenderer.Print(to_hstring((uint64_t)0xF0));
    newRenderer.CursorPosition = {0, 64};
    newRenderer.Print(to_hstring((uint32_t)0xF0));
    newRenderer.CursorPosition = {0, 80};
    newRenderer.Print(to_hstring((uint16_t)0xF0));
    newRenderer.CursorPosition = {0, 96};
    newRenderer.Print(to_hstring((uint8_t)0xF0));

    return;
}