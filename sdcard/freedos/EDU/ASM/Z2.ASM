; --- ZAPLATKA 2: postavit svoj obrabotchik IRQ 5 (vektor 0Dh) ---
        push    es
        mov     ax,350Dh           ; DOS: kakoj sejchas vektor 0Dh? -> ES:BX
        int     21h
        mov     [STARYJ],bx
        mov     ax,es
        mov     [STARYJ+2],ax
        pop     es
        mov     dx,OFFSET OBRABOTCHIK  ; DS:DX - nash obrabotchik
        mov     ax,250Dh           ; DOS: postavit vektor 0Dh
        int     21h
        mov     word [N_OB],1      ; Bejsik uvidit, chto zaplatka est
; --- konec zaplatki 2 ---
