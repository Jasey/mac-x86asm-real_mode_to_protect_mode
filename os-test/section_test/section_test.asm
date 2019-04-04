
;如果不加对齐字段，默认4字节对齐
section data1
    db 0x55
section data2 align=16
    db 0xaa
section data3 align=16
    db 0x99