switchtsk:
    push ebx
    push ebp
    push esi
    push edi
    mov [eax], esp
    mov esp, edx
    pop edi
    pop esi
    pop ebp
    pop ebx
    ret
