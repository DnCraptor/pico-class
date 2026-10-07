; --- ZAPLATKA 3: vernut staryj obrabotchik ---
        push    ds
        mov     dx,[STARYJ]
        mov     ax,[STARYJ+2]
        mov     ds,ax              ; DS:DX - staryj obrabotchik
        mov     ax,250Dh
        int     21h
        pop     ds
; --- konec zaplatki 3 ---
