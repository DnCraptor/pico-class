{ Истина и ложь: в Паскале для этого есть свой тип Boolean }
program Istina;
var
  b: Boolean;
begin
  b := 5 > 3;
  WriteLn('5 > 3 : ', b);
  WriteLn('5 < 3 : ', 5 < 3);
  WriteLn('Ord(True) = ', Ord(True), ', Ord(False) = ', Ord(False));
  { if 7 then ... в Паскале не соберется: нужно именно True или False }
end.
