#include <mydebug.h>
#include <font.h>
#include <stdio.h>

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

static int FreetypeInit(void);
static int FreeTypeGetOneFontBitmap(unsigned int p_onefontencode , P_FontBitmap p_fontbitmap);

static T_FontOprType gt_freetype={
    .name="freetype",
    .FontTypeInit=FreetypeInit,
    .GetFontBitmap=FreeTypeGetOneFontBitmap,
};

static FT_Library      library;
static FT_Face         face;
static FT_GlyphSlot    slot;
static FT_Error        error;


static int FreetypeInit()
{
    error = FT_Init_FreeType( &library );              
    if(error){
        DebugPrint("init freetype error!\n");
        return -1;
    }

    error = FT_New_Face( library, "./simsun.ttc", 0, &face ); 
    if(error){
        DebugPrint("face error!\n");
        return -1;
    }
    FT_Set_Pixel_Sizes(face , 24 , 0);
    slot = face->glyph;
    return 0;
}

static int FreeTypeGetOneFontBitmap(unsigned int p_onefontencode , P_FontBitmap p_fontbitmap)
{
    int pen_x ,pen_y;
    pen_x = p_fontbitmap->cur_pos_x;
    pen_y = p_fontbitmap->cur_pos_y;

    error = FT_Load_Char( face,p_onefontencode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME ); 

    p_fontbitmap->left            = pen_x + slot->bitmap_left;
    p_fontbitmap->top             = pen_y - slot->bitmap_top;
    p_fontbitmap->xMax            = p_fontbitmap->left + slot->bitmap.width;
    p_fontbitmap->yMax            = p_fontbitmap->top  + slot->bitmap.rows;
    p_fontbitmap->pitch           = slot->bitmap.pitch;
    p_fontbitmap->next_pos_x      = pen_x + slot->advance.x / 64;
    p_fontbitmap->next_pos_y      = pen_y;
    p_fontbitmap->p_bitmap_buffer = slot->bitmap.buffer;

//    DebugPrint("pen_y = %d\nslot->bitmap_top=%d\n",pen_y,slot->bitmap_top);
    return 0;
}

int ExitFT()
{
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );
    return 0;
}
int FreetypeRegister()
{
    return FontOprRegister(&gt_freetype);
}
