/* Copyright © 2022  China Mobile IOT. 
 * All rights reserved.
 *
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(cm_opencpu_entry)

#define ALIGNMENT   ALIGN(0x8)
#define IMG_INFO    LONG(0x5A5AA5A5) \
                    LONG(__flash_end - __flash_start) \
                    LONG(__flash_start) \
                    LONG(__ram_start)
#define SECTION_INFO(type, section) LONG(type) \
                                    LONG(__##section##_lma_start - __flash_start) \
                                    LONG(__##section##_lma_start) \
                                    LONG(__##section##_vma_start) \
                                    LONG(__##section##_vma_end - __##section##_vma_start)

#define SECTION_TEXT    .text .text.* .gnu.linkonce.t.* .glue_7 .glue_7t .vfp11_veneer .v4_bx
#define SECTION_SRAMTEXT .sramtext
#define SECTION_RODATA  .rdata .rodata .rodata.* .gnu.linkonce.r.*
#define SECTION_DATA    .data .data.* .gnu.linkonce.d.*
#define SECTION_BSS     .bss .bss.* COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.* .gnu.linkonce.b.*

MEMORY {
    ram (rwx): ORIGIN = 0x7E380000, LENGTH = 512K
    flash (rwx): ORIGIN = 0x805F8000, LENGTH = 1024K
}


SECTIONS {
    . = ORIGIN(flash);
    __flash_start = .;

    /*头部信息，lma=vma*/
    .image_header : {
        IMG_INFO
        SECTION_INFO(0, text)
        SECTION_INFO(1, rodata)
        SECTION_INFO(2, data)
        SECTION_INFO(3, bss)
        SECTION_INFO(4, sramtext)
    } > flash

    /*.text段，lma=vma*/
    .text ALIGNMENT : {
        __text_vma_start = .;
        *(.text.cm_opencpu_entry)
        EXCLUDE_FILE(*cm_stub_DSLN.o *.sram.o *lib_tts.a:)*(SECTION_TEXT)
        __text_vma_end = .;
        . = ALIGNMENT;
        __text_lma_start = LOADADDR(.text);
    } > flash

    /*.rodata段，lma=vma*/
    .rodata ALIGNMENT : {
        __rodata_vma_start = .;
        *(SECTION_RODATA)
        __rodata_vma_end = .;
        . = ALIGNMENT;
        __rodata_lma_start = LOADADDR(.rodata);
    } > flash

    /*vma切换至ram*/
    . = ORIGIN(ram);
    __ram_start = .;

    /*.sramtext段，lma=flash，vma=ram*/
    .sramtext ALIGNMENT : {
        __sramtext_vma_start = .;
        *cm_stub_DSLN.o(SECTION_TEXT)
        *.sram.o(SECTION_TEXT)
        *lib_tts.a:(SECTION_TEXT)
        *(SECTION_SRAMTEXT)
        __sramtext_vma_end = .;
        . = ALIGNMENT;
        __sramtext_lma_start = LOADADDR(.sramtext);
    } AT>flash

    /*.data段，lma=flash，vma=ram*/
    .data ALIGNMENT : {
        __data_vma_start = .;
        *(SECTION_DATA)
        __data_vma_end = .;
        . = ALIGNMENT;
        __data_lma_start = LOADADDR(.data);
    } AT>flash

    /*.bss段，lma=vma*/
    .bss ALIGNMENT : {
        __bss_vma_start = .;
        *(SECTION_BSS)
        __bss_vma_end = .;
        . = ALIGNMENT;
        __bss_lma_start = LOADADDR(.bss);
    }

    __flash_end = LOADADDR(.data) + SIZEOF(.data);
    __ram_end = ADDR(.bss) + SIZEOF(.bss);
    end = __ram_end;
    __ram_reserve = ORIGIN(ram) + LENGTH(ram) - __ram_end;
    __flash_reserve = ORIGIN(flash) + LENGTH(flash) - __flash_end;
    ASSERT(__flash_end <= ORIGIN(flash) + LENGTH(flash), "FLASH overflow")
    ASSERT(__ram_end <= ORIGIN(ram) + LENGTH(ram), "RAM overflow")
}
