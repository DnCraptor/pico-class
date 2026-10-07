{ Синусоида через DMA: Sound Blaster играет блок памяти сам }
program Sinus;
uses Dos, Crt;

const
  RATE  = 8000;                 { отсчетов в секунду }
  DLINA = 8000;                 { блок - ровно 1 секунда }
  STRANICA: array[0..3] of Byte = ($87, $83, $81, $82);  { страничные регистры DMA }

var
  buf: array[0..2 * DLINA - 1] of Byte;   { вдвое больше: внутри ищем окно для DMA }
  baza: Word;                   { порт Sound Blaster, обычно 220h }
  irq, dma: Byte;               { номер IRQ и канал DMA }
  schet: Word;                  { сколько блоков отыграно - считает обработчик }
  staryj: Pointer;              { прежний обработчик прерывания }
  maska: Byte;                  { прежняя маска контроллера прерываний }
  sdvig: Word;                  { сдвиг окна внутри buf }
  phys: LongInt;
  f: Real;
  i: Integer;
  s: string;
  ostalos: Word;
  c: Char;

procedure Razobrat(s: string);  { BLASTER=A220 I5 D1 ... }
var i: Integer;
begin
  baza := $220; irq := 5; dma := 1;
  for i := 1 to Length(s) - 1 do
    if (i = 1) or (s[i - 1] = ' ') then
      case UpCase(s[i]) of
        'A': baza := (Ord(s[i+1]) - 48) * 256 + (Ord(s[i+2]) - 48) * 16 + (Ord(s[i+3]) - 48);
        'I': irq := Ord(s[i+1]) - 48;
        'D': dma := Ord(s[i+1]) - 48;
      end;
end;

procedure DspPishi(b: Byte);    { подождать, пока DSP готов, и записать }
begin
  while Port[baza + $C] and $80 <> 0 do ;
  Port[baza + $C] := b;
end;

function DspSbros: Boolean;
var i: Integer;
begin
  Port[baza + 6] := 1;
  for i := 1 to 100 do ;        { небольшая пауза }
  Port[baza + 6] := 0;
  i := 0;
  repeat Inc(i) until (Port[baza + $E] and $80 <> 0) or (i > 30000);
  DspSbros := Port[baza + $A] = $AA;
end;

procedure Obrabotchik; interrupt;   { вызывается по IRQ: блок отыгран }
var b: Byte;
begin
  Inc(schet);
  b := Port[baza + $E];         { сказать карте: прерывание принято }
  Port[$20] := $20;             { сказать контроллеру прерываний: обработка окончена }
end;

begin
  s := GetEnv('BLASTER');
  Razobrat(s);
  WriteLn('BLASTER=', s, '  ->  IRQ ', irq, ', DMA ', dma);
  if not DspSbros then begin WriteLn('Sound Blaster не отвечает'); Halt; end;
  Write('Частота, Гц (например, 440): ');
  ReadLn(f);

  { окно в buf, которое не пересекает границу 64 КБ физической памяти }
  phys := LongInt(Seg(buf)) * 16 + Ofs(buf);
  if (phys and $FFFF) + DLINA > $10000 then sdvig := $10000 - (phys and $FFFF) else sdvig := 0;
  phys := phys + sdvig;
  for i := 0 to DLINA - 1 do
    buf[sdvig + i] := Round(128 + 120 * Sin(2 * Pi * f * i / RATE));

  { свой обработчик IRQ }
  schet := 0;
  GetIntVec(8 + irq, staryj);
  SetIntVec(8 + irq, @Obrabotchik);
  maska := Port[$21];
  Port[$21] := maska and not (1 shl irq);       { разрешить этот IRQ }

  { контроллер DMA: из памяти в устройство, с автоповтором }
  Port[$0A] := 4 + dma;                         { закрыть канал }
  Port[$0C] := 0;                               { сброс: дальше сначала младший байт }
  Port[$0B] := $58 + dma;                       { режим: чтение памяти, автоповтор }
  Port[dma * 2] := phys and $FF;                { адрес внутри страницы 64 КБ }
  Port[dma * 2] := (phys shr 8) and $FF;
  Port[STRANICA[dma]] := phys shr 16;           { номер страницы 64 КБ }
  Port[dma * 2 + 1] := (DLINA - 1) and $FF;     { длина - 1 }
  Port[dma * 2 + 1] := (DLINA - 1) shr 8;
  Port[$0A] := dma;                             { открыть канал }

  { Sound Blaster: частота, размер блока, пуск с автоповтором }
  DspPishi($D1);                                { включить выход }
  DspPishi($40); DspPishi(256 - 1000000 div RATE);
  DspPishi($48); DspPishi((DLINA - 1) and $FF); DspPishi((DLINA - 1) shr 8);
  DspPishi($1C);

  WriteLn('Играет. Любая клавиша - стоп.');
  repeat
    Port[$0C] := 0;                             { сколько осталось в блоке - спросим DMA }
    ostalos := Port[dma * 2 + 1];
    ostalos := ostalos + Port[dma * 2 + 1] * 256;
    Write(#13'Блоков отыграно: ', schet, '   DMA: осталось ', ostalos:5, ' байт ');
  until KeyPressed;
  c := ReadKey;
  if c = #0 then c := ReadKey;               { у стрелок и F1-F12 - два кода }
  WriteLn;

  DspPishi($D0);                                { остановить DMA карты }
  DspPishi($D3);                                { выключить выход }
  Port[$0A] := 4 + dma;                         { закрыть канал DMA }
  Port[$21] := maska;                           { вернуть маску IRQ }
  SetIntVec(8 + irq, staryj);                   { вернуть прежний обработчик! }
  if DspSbros then ;                          { сброс карты в исходное состояние }
end.
