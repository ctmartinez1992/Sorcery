#pragma once

#include "FrameBuffer.h"
#include "SimpleFonts.h"
#include "math.h"

class BasicRenderer {
   public:
    Point CursorPosition;
    FrameBuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;

    BasicRenderer(FrameBuffer* targetFramebuffer, PSF1_FONT* psf1_font);

    void Print(const char* str);
    void PutChar(char c, unsigned int xOff, unsigned int yOff);
};