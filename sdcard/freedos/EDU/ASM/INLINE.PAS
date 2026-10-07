{ Машинный код прямо в программе на Паскале: inline }
program InlineP;
begin
  Write('inline: ');
  Flush(Output);           { дописать то, что Паскаль держит в буфере }
  inline($B4/$02/          { mov ah,2  - служба DOS 02h: вывести букву }
         $B2/$41/          { mov dl,41h - буква "A" }
         $CD/$21);         { int 21h   - вызвать DOS }
  WriteLn;
  WriteLn('Те же байты, что набирали в DEBUG: B4 02 B2 41 CD 21');
end.
