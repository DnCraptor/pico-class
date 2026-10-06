{ Переполнение: 32767 + 1 в 16-битном Integer }
program Perepol;
var
  a: Integer;          { 16 бит: от -32768 до 32767 }
  b: LongInt;          { 32 бита: до 2147483647 }
begin
  a := 32767;
  WriteLn('a = ', a);
  a := a + 1;          { проверки нет: число "перекручивается" }
  WriteLn('a + 1 = ', a);
  b := 32767;
  b := b + 1;
  WriteLn('LongInt: ', b);
end.
