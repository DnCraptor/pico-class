{ Рисунок из процедур: квадрат с параметрами, дом и солнце }
program Risunok;
uses Graph;
var
  gd, gm, i: Integer;

{ квадрат: левый верхний угол (x, y), сторона a, цвет c }
procedure Kvadrat(x, y, a, c: Integer);
begin
  SetColor(c);
  Rectangle(x, y, x + a, y + a);
end;

{ дом: левый нижний угол (x, y), ширина w }
procedure Dom(x, y, w: Integer);
begin
  SetFillStyle(SolidFill, Brown);
  Bar(x, y - w, x + w, y);                       { стены }
  SetColor(Red);
  Line(x, y - w, x + w div 2, y - w - w div 2);  { крыша }
  Line(x + w div 2, y - w - w div 2, x + w, y - w);
  SetFillStyle(SolidFill, LightCyan);
  Bar(x + w div 3, y - 2 * w div 3, x + 2 * w div 3, y - w div 3);  { окно }
end;

procedure Solnce(x, y, r: Integer);
begin
  SetColor(Yellow);
  SetFillStyle(SolidFill, Yellow);
  FillEllipse(x, y, r, r);
end;

begin
  gd := EGA;                     { режим 640 x 350, 16 цветов: есть у EGA и VGA }
  gm := EGAHi;
  InitGraph(gd, gm, '\FREEDOS\EDU\TP55');
  if GraphResult <> grOk then
  begin
    WriteLn('Графика не включилась: нужен видеоадаптер EGA или VGA (в murm386 - Win+F11)');
    Halt;
  end;
  for i := 1 to 8 do
    Kvadrat(20 + i * 6, 20 + i * 6, 120 - i * 12, i);
  Dom(250, 300, 160);
  Dom(450, 300, 100);
  Solnce(560, 70, 40);
  OutTextXY(20, GetMaxY - 20, 'Press Enter');
  ReadLn;
  CloseGraph;
end.
