{ VSTAVKA: вставить заплатку в .ASM после метки строки Бейсика
  vstavka sinb.asm L00200 z1.asm }
program Vstavka;
var
  f, g, z: Text;
  s, metka: string;
  najdeno: Boolean;
begin
  if ParamCount < 3 then
  begin
    WriteLn('vstavka файл.asm метка заплатка.asm');
    Halt(1);
  end;
  metka := ParamStr(2) + ':';
  najdeno := False;
  Assign(f, ParamStr(1)); Reset(f);
  Assign(g, 'VSTAVKA.TMP'); Rewrite(g);
  while not Eof(f) do
  begin
    ReadLn(f, s);
    WriteLn(g, s);
    if (not najdeno) and (s = metka) then
    begin
      najdeno := True;
      Assign(z, ParamStr(3)); Reset(z);
      while not Eof(z) do begin ReadLn(z, s); WriteLn(g, s); end;
      Close(z);
    end;
  end;
  Close(f); Close(g);
  if najdeno then
  begin
    Erase(f);
    Rename(g, ParamStr(1));
    WriteLn('Вставлено после ', metka);
  end
  else
  begin
    Erase(g);
    WriteLn('Метка ', metka, ' не найдена');
  end;
end.
