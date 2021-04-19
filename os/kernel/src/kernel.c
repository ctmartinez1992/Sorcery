typedef unsigned long long size_t;

typedef struct {
    void* BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanLine;
} FrameBuffer;

typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
} PSF1_HEADER;

typedef struct {
    PSF1_HEADER* psf1_Header;
    void* glyphBuffer;
} PSF1_FONT;

typedef struct {
    unsigned int x;
    unsigned int y;
} Point;

Point CursorPosition;

void putChar(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int color, char c, unsigned int xOff, unsigned int yOff) {
    unsigned int* pixPtr = (unsigned int*)framebuffer->BaseAddress;
    char* fontPtr = psf1_font->glyphBuffer + (c * psf1_font->psf1_Header->charsize);

    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * framebuffer->PixelsPerScanLine)) = color;
            }
        }
        fontPtr++;
    }
}

void Print(FrameBuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int color, char* str) {
    unsigned int x = 0;
    char* c = str;
    while (*c != 0) {
        putChar(framebuffer, psf1_font, color, *c, CursorPosition.x, CursorPosition.y);
        CursorPosition.x += 8;
        if (CursorPosition.x + 8 > framebuffer->Width) {
            CursorPosition.x = 0;
            CursorPosition.y += 16;
        }
        c++;
    }
}

void _start(FrameBuffer* framebuffer, PSF1_FONT* psf1_font) {
    CursorPosition.x = 50;
    CursorPosition.y = 50;

    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");
    Print(framebuffer, psf1_font, 0xffffffff, "Hello World");

    return;
}