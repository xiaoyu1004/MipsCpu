OUTPUT_ARCH(mips)
SECTIONS
{
    . = 0x00;
    /* text: test code section */
    .text :
    {
        *(.text)
    }

    /* data segment */
    .data : { *(.data) }
}