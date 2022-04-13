global crc32_sse

section .text
crc32_sse:
        mov eax, 0x04c11db7
        lea rsi, [rdi + 32]
.hash:
        crc32 eax, dword [rdi]
        add rdi, 0x4
        cmp rdi, rsi
        jne .hash
        ret
        
        
