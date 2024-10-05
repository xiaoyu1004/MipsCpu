OUTPUT_ARCH(mips)
SECTIONS
{
    . = 0x00;

    /* data segment */
    .data : { *(.data) }

    /* text: test code section */
    .text :
    {
        *(.text)
    }
}