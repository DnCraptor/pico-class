{ Память: читаем байты по адресу сегмент:смещение }
program Pamyat;

function Hex(w: Word): string;            { число -> шестнадцатеричная запись }
const Cifry: string[16] = '0123456789ABCDEF';
var s: string[4]; i: Integer;
begin
  s := '';
  for i := 1 to 4 do
  begin
    s := Cifry[w mod 16 + 1] + s;
    w := w div 16;
  end;
  Hex := s;
end;

var i: Integer;
begin
  Write('Дата BIOS (F000:FFF5): ');
  for i := 0 to 7 do Write(Chr(Mem[$F000:$FFF5 + i]));
  WriteLn;
  WriteLn('ОЗУ (0040:0013): ', MemW[$0040:$0013], ' КБ');
  WriteLn('Порт COM1 (0040:0000): ', Hex(MemW[$0040:$0000]), 'h');
  WriteLn('Порт LPT1 (0040:0008): ', Hex(MemW[$0040:$0008]), 'h');
  if Mem[$0040:$0017] and $40 <> 0 then
    WriteLn('Caps Lock включен')
  else
    WriteLn('Caps Lock выключен');
  Mem[$B800:0] := Ord('A');               { буква }
  Mem[$B800:1] := $1E;                    { цвет: желтый на синем }
  WriteLn('В левом верхнем углу - буква A, записанная прямо в видеопамять');
end.
