; --- ZAPLATKA 4: obrabotchik preryvaniya ---
STARYJ  dw      0,0                ; zdes zapomnim staryj vektor
OBRABOTCHIK:
        push    ax
        push    dx
        push    ds
        push    cs
        pop     ds                 ; DS - nash segment: tam peremennye Bejsika
        inc     word [N_SCHET]     ; SCHET = SCHET + 1
        mov     dx,22Eh
        db      236                ; IN AL,DX: "preryvanie prinyato" (BASM ne znaet IN)
        mov     al,20h
        db      230,20h            ; OUT 20h,AL: "obrabotka okonchena" (BASM ne znaet OUT)
        pop     ds
        pop     dx
        pop     ax
        db      207                ; IRET (BASM ne znaet IRET)
; --- konec zaplatki 4 ---
