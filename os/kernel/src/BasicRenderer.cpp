#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(FrameBuffer* targetFramebuffer, PSF1_FONT* psf1_font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_font;
    Color = 0xffffffff;
    CursorPosition = {0, 0};
}

void BasicRenderer::Print(const char* str) {
    unsigned int x = 0;
    char* c = (char*)str;
    while (*c != 0) {
        PutChar(*c, CursorPosition.x, CursorPosition.y);
        CursorPosition.x += 8;
        if (CursorPosition.x + 8 > TargetFramebuffer->Width) {
            CursorPosition.x = 0;
            CursorPosition.y += 16;
        }
        c++;
    }
}

void BasicRenderer::PutChar(char c, unsigned int xOff, unsigned int yOff) {
    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (c * PSF1_Font->psf1_Header->charsize);

    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = Color;
            }
        }
        fontPtr++;
    }
}