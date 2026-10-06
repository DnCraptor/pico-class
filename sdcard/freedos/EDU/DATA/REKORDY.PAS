{ Рекорды: таблица из пяти записей в файле REKORDY.TXT }
program Rekordy;
type
  Rekord = record
    imja:  string[15];      { имя - не длиннее 15 букв }
    ochki: Integer;
  end;
var
  t: array[1..6] of Rekord; { 5 рекордов + 1 место для нового }
  f: Text;
  i, k: Integer;
  r: Rekord;

procedure Pokazat;
var i: Integer;
begin
  WriteLn;
  WriteLn('   Имя             Очки');
  for i := 1 to 5 do
    WriteLn(i:2, ' ', t[i].imja, '':16 - Length(t[i].imja), t[i].ochki);
  WriteLn;
end;

begin
  for i := 1 to 5 do
  begin
    t[i].imja := '---';
    t[i].ochki := 0;
  end;
  Assign(f, 'REKORDY.TXT');
  {$I-} Reset(f); {$I+}          { $I- : не падать, если файла нет }
  if IOResult = 0 then
  begin
    for i := 1 to 5 do
    begin
      ReadLn(f, t[i].imja);
      ReadLn(f, t[i].ochki);
    end;
    Close(f);
  end;
  Pokazat;
  Write('Имя: ');   ReadLn(t[6].imja);
  Write('Очки: ');  ReadLn(t[6].ochki);
  { сортировка обменом: большие очки - наверх }
  for k := 1 to 5 do
    for i := 1 to 6 - k do
      if t[i].ochki < t[i + 1].ochki then
      begin
        r := t[i]; t[i] := t[i + 1]; t[i + 1] := r;   { запись меняем целиком }
      end;
  Rewrite(f);
  for i := 1 to 5 do
  begin
    WriteLn(f, t[i].imja);
    WriteLn(f, t[i].ochki);
  end;
  Close(f);
  Pokazat;
end.
