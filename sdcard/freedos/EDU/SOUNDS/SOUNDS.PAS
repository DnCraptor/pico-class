{ SOUNDS.PAS - звуковые устройства PC 1981-1992 годов.
  Начало "Оды к радости" Бетховена - первые 8 тактов темы, мелодия и бас -
  звучит на:
    1  PC speaker (пищалка)  - одна прямоугольная волна от микросхемы таймера,
                               голос один - только мелодия, без баса
    2  Tandy / PCjr          - генератор звука SN76489 (PSG), 3 голоса;
                               после мелодии - три голоса по очереди и шум
    3  AdLib                 - FM-синтезатор Yamaha OPL2
    4  Covox Sound Master    - генератор звука AY-3-8930 (PSG), только порт 240h:
                               в настройках murm386 - Sound Master (240h), не 220h
    5  Roland MPU-401        - ноты General MIDI для синтезатора
    6  Covox Speech Thing    - 8-битный ЦАП на LPT2 (порт 278h)
    7  Disney Sound Source   - ЦАП с очередью на 16 чисел на LPT1 (378h)
    8  Sound Blaster         - ЦАП, числа подает контроллер DMA (220h, DMA 1)
  Для ЦАП (6-8) программа сама вычисляет звуковую волну, число за числом,
  а после мелодии произносит фразу из файла SPEECH.RAW (8000 чисел в
  секунду, 8 бит).
  A - первые два такта мелодии четырьмя инструментами AdLib.
  D - та же фраза с меньшим числом бит и меньшей частотой дискретизации.
  G - игра "Угадай устройство".

  Сокращения:
    PSG  - Programmable Sound Generator, программируемый генератор звука
    FM   - Frequency Modulation, частотная модуляция
    OPL  - FM Operator Type-L, семейство FM-микросхем Yamaha
    MIDI - Musical Instrument Digital Interface, цифровой интерфейс
           музыкальных инструментов
    ЦАП  - цифро-аналоговый преобразователь (DAC, Digital-to-Analog Converter)
    DMA  - Direct Memory Access, прямой доступ к памяти (ПДП)
    DSP  - Digital Signal Processor: так называется микросхема-контроллер
           Sound Blaster, которой программа посылает команды
    PIT  - Programmable Interval Timer, микросхема таймера 8253/8254
    LPT  - порт принтера

  Модуль Crt не используется: его задержка Delay ломается на быстрых
  машинах. Все паузы отсчитывает микросхема таймера.
  Часть комплекта pico-class, см. tools/sound/README.md. }

program Sounds;

{$F+}  { дальние вызовы везде: процедуры нот передаются как параметры }

uses Dos;

const
  PIT_HZ   = 1193182;           { частота, на которой считает таймер }
  RATE     = 8000;              { частота дискретизации для Covox и Sound Blaster }
  DSS_RATE = 7000;              { Disney Sound Source всегда играет 7000 чисел в секунду }
  QUARTER  = 220;               { длина четверти, миллисекунды: темп }
  NNOTES   = 30;
  { мелодия: номера нот MIDI (60 = до первой октавы) и длины в восьмых }
  MelNote: array[1..NNOTES] of Byte = (
    64,64,65,67, 67,65,64,62, 60,60,62,64, 64,62,62,
    64,64,65,67, 67,65,64,62, 60,60,62,64, 62,60,60);
  MelLen: array[1..NNOTES] of Byte = (
    2,2,2,2, 2,2,2,2, 2,2,2,2, 3,1,4,
    2,2,2,2, 2,2,2,2, 2,2,2,2, 3,1,4);
  { бас: одна нота на такт (8 восьмых) }
  NBARS = 8;
  BassNote: array[1..NBARS] of Byte = (48, 43, 48, 43, 48, 43, 48, 48);

type
  TBuf = array[0..65000] of Byte;
  PBuf = ^TBuf;

var
  Wave: PBuf;                   { вычисленная мелодия для ЦАП }
  WaveLen: Word;
  Speech: PBuf;                 { фраза из SPEECH.RAW }
  SpeechLen: Word;
  Work: PBuf;                   { фраза, испорченная для пункта D }
  Wave7, Speech7: PBuf;         { мелодия и фраза для Disney Sound Source: 7000 в секунду }
  Wave7Len, Speech7Len: Word;
  SinTab: array[0..255] of ShortInt;
  Stop: Boolean;                { нажали Esc - остановить звук }

{ ---------- таймер, клавиатура ---------- }

function PitCount: Word;        { счетчик канала 0, считает вниз 1,19 млн раз в секунду }
var lo, hi: Byte;
begin
  Port[$43] := $00;             { защелкнуть счетчик канала 0 }
  lo := Port[$40];
  hi := Port[$40];
  PitCount := lo or (Word(hi) shl 8);
end;

procedure PitMode2;
{ В режиме 2 канал 0 уменьшается на 1 за такт (BIOS ставит режим 3,
  в нем счет идет по 2); перезагрузка прежняя - 65536, 18,2 прерывания в секунду. }
begin
  Port[$43] := $34; Port[$40] := 0; Port[$40] := 0;
end;

procedure PitMode3;             { вернуть режим, как было у BIOS }
begin
  Port[$43] := $36; Port[$40] := 0; Port[$40] := 0;
end;

function KeyPressed: Boolean;
{ Есть ли нажатая клавиша в буфере клавиатуры. Буфер лежит в области данных
  BIOS (сегмент 40h): слово 1Ah - откуда читать, 1Ch - куда писать; если они
  равны, буфер пуст. Так проверку не испортит ни одна реализация INT 16h. }
begin
  KeyPressed := MemW[$40:$1A] <> MemW[$40:$1C];
end;

function ReadKey: Char;         { дождаться клавиши и прочитать ее }
var r: Registers;
begin
  r.ah := 0; Intr($16, r);
  ReadKey := Chr(r.al);
end;

procedure WaitTicks(ticks: LongInt);   { подождать столько тактов таймера }
var last, now: Word;
begin
  last := PitCount;
  while ticks > 0 do
  begin
    now := PitCount;
    ticks := ticks - Word(last - now);
    last := now;
  end;
end;

procedure WaitMs(ms: Word);     { подождать миллисекунды; Esc - остановить }
begin
  if KeyPressed then if ReadKey = #27 then Stop := True;
  if not Stop then WaitTicks(LongInt(ms) * 1193);
end;

function Freq(note: Byte): Real;       { нота MIDI -> частота в герцах, ля = 440 }
begin
  Freq := 440.0 * Exp((note - 69) * Ln(2.0) / 12.0);
end;

{ ---------- 1. PC speaker ---------- }

procedure SpeakerOn(f: Real);
var d: Word;
begin
  d := Round(PIT_HZ / f);       { делитель: во сколько раз медленнее таймера }
  Port[$43] := $B6;             { канал 2 таймера, прямоугольная волна }
  Port[$42] := Lo(d); Port[$42] := Hi(d);
  Port[$61] := Port[$61] or 3;  { подключить таймер к динамику }
end;

procedure SpeakerOff;
begin
  Port[$61] := Port[$61] and $FC;
end;

procedure PlaySpeaker;          { голос один - поэтому только мелодия, без баса }
var i: Integer;
begin
  i := 1;
  while (i <= NNOTES) and not Stop do
  begin
    SpeakerOn(Freq(MelNote[i]));
    WaitMs(MelLen[i] * QUARTER div 2 - 20);
    SpeakerOff;
    WaitMs(20);
    Inc(i);
  end;
  SpeakerOff;
end;

{ ---------- общий проигрыватель для микросхем: мелодия и бас на двух голосах ---------- }

type
  TNoteProc = procedure(voice: Byte; note: Byte);   { нота 0 - тишина }

procedure PlayChip(play: TNoteProc);
{ идет по мелодии восьмыми и в начале каждого такта берет ноту баса }
var i, k, eighth: Integer;
begin
  i := 1; eighth := 0;
  while (i <= NNOTES) and not Stop do
  begin
    if eighth mod 8 = 0 then play(1, BassNote[eighth div 8 + 1]);
    play(0, MelNote[i]);
    for k := 1 to MelLen[i] do
    begin
      if (k > 1) and (eighth mod 8 = 0) then play(1, BassNote[eighth div 8 + 1]);
      if k = MelLen[i] then
      begin
        WaitMs(QUARTER div 2 - 25); play(0, 0); WaitMs(25);
      end
      else WaitMs(QUARTER div 2);
      Inc(eighth);
    end;
    Inc(i);
  end;
  play(0, 0); play(1, 0);
end;

{ ---------- 2. Tandy / PCjr: SN76489, порт 0C0h ---------- }

procedure TandyNote(voice: Byte; note: Byte);
var n: Word;
begin
  if note = 0 then
    Port[$C0] := $90 or (voice shl 5) or $0F       { громкость - выключено }
  else
  begin
    n := Round(3579545.0 / (32.0 * Freq(note)));   { период, 10 бит }
    { ниже 109 Гц SN76489 не умеет (период больше 1023): берем октавой выше }
    while n > 1023 do n := n shr 1;
    Port[$C0] := $80 or (voice shl 5) or (n and $0F);
    Port[$C0] := n shr 4;
    Port[$C0] := $90 or (voice shl 5) or (voice * 3);  { бас немного тише }
  end;
end;

procedure TandyVolume(voice, att: Byte);   { ослабление: 0 - громче всего, 15 - тишина }
begin
  Port[$C0] := $90 or (voice shl 5) or att;
end;

procedure TandyVoices;
{ Три голоса по очереди: каждый плавно становится громче, потом все вместе
  затихают. Затем четвертый голос - шум, как взрыв в игре. }
const Chord: array[0..2] of Byte = (60, 64, 67);   { до, ми, соль }
var v, a: Integer; n: Word;
begin
  for v := 0 to 2 do
  begin
    n := Round(3579545.0 / (32.0 * Freq(Chord[v])));
    TandyVolume(v, 15);
    Port[$C0] := $80 or (v shl 5) or (n and $0F);
    Port[$C0] := n shr 4;
  end;
  v := 0;
  while (v <= 2) and not Stop do
  begin
    Writeln('  голос ', v + 1);
    a := 15;
    while (a >= 0) and not Stop do
    begin
      TandyVolume(v, a); WaitMs(70); Dec(a);
    end;
    WaitMs(400);
    Inc(v);
  end;
  if not Stop then Writeln('  все три голоса затихают');
  a := 0;
  while (a <= 15) and not Stop do
  begin
    for v := 0 to 2 do TandyVolume(v, a);
    WaitMs(120); Inc(a);
  end;
  for v := 0 to 2 do TandyVolume(v, 15);
  if not Stop then
  begin
    Writeln('  шум');
    Port[$C0] := $E4;           { шум: белый, самый частый }
    a := 0;
    while (a <= 15) and not Stop do
    begin
      TandyVolume(3, a); WaitMs(90); Inc(a);
    end;
  end;
  TandyVolume(3, 15);
end;

procedure PlayTandyTune;        { только мелодия и бас - для игры }
var v: Byte;
begin
  for v := 0 to 3 do TandyVolume(v, 15);   { все четыре голоса выключить }
  PlayChip(TandyNote);
end;

procedure PlayTandy;
begin
  Writeln('  мелодия и бас');
  PlayTandyTune;
  if not Stop then
  begin
    WaitMs(500);
    TandyVoices;
  end;
end;

{ ---------- 3. AdLib: OPL2, порты 388h и 389h ---------- }

procedure OplWrite(reg, val: Byte);   { записать число в регистр OPL2 }
var i: Integer; b: Byte;
begin
  Port[$388] := reg;
  for i := 1 to 6 do b := Port[$388];  { микросхеме нужно время: ждем, читая порт }
  Port[$389] := val;
  for i := 1 to 35 do b := Port[$388];
end;

const
  { номера операторов голосов 0 и 1: модулятор и несущая }
  OplMod: array[0..1] of Byte = ($00, $01);
  OplCar: array[0..1] of Byte = ($03, $04);

procedure OplSetup;
var r: Integer; ch: Byte;
begin
  for r := $01 to $F5 do OplWrite(r, 0);
  OplWrite($01, $20);           { разрешить выбор формы волны }
  for ch := 0 to 1 do
  begin
    { мягкий звук, похожий на электропианино: модулятор раскачивает несущую }
    OplWrite($20 + OplMod[ch], $01);  OplWrite($20 + OplCar[ch], $01);
    OplWrite($40 + OplMod[ch], $10);  OplWrite($40 + OplCar[ch], $00);
    OplWrite($60 + OplMod[ch], $F2);  OplWrite($60 + OplCar[ch], $F3);
    OplWrite($80 + OplMod[ch], $77);  OplWrite($80 + OplCar[ch], $75);
    OplWrite($E0 + OplMod[ch], $00);  OplWrite($E0 + OplCar[ch], $00);
    OplWrite($C0 + ch, $06);          { обратная связь 3, режим FM }
  end;
end;

type
  TPatch = record
    name: String[12];
    m20, m40, m60, m80, mE0, c20, c40, c60, c80, cE0, fbC0: Byte;
  end;
const
  NPATCH = 4;
  { Одни и те же два оператора, настроенные четырьмя способами. Регистры:
    20h - множитель частоты, 40h - громкость (0 - громче всего, 3Fh - тише),
    60h - атака и спад, 80h - уровень и время затухания, E0h - форма волны,
    C0h - обратная связь. Громкость модулятора (40h) и его множитель (20h)
    решают, сколько обертонов получит несущая, - в этом весь фокус FM-синтеза. }
  Patches: array[1..NPATCH] of TPatch = (
    (name: 'флейта';      m20: $21; m40: $3F; m60: $F0; m80: $F0; mE0: 0;
                          c20: $21; c40: $00; c60: $A1; c80: $15; cE0: 0; fbC0: $00),
    (name: 'пианино';     m20: $01; m40: $10; m60: $F2; m80: $77; mE0: 0;
                          c20: $01; c40: $00; c60: $F3; c80: $75; cE0: 0; fbC0: $06),
    (name: 'труба';       m20: $21; m40: $16; m60: $71; m80: $AE; mE0: 0;
                          c20: $21; c40: $00; c60: $81; c80: $9E; cE0: 0; fbC0: $0E),
    (name: 'колокольчик'; m20: $05; m40: $14; m60: $F2; m80: $13; mE0: 0;
                          c20: $01; c40: $00; c60: $F2; c80: $14; cE0: 0; fbC0: $00));

procedure OplPatch(ch: Byte; var pt: TPatch);
begin
  OplWrite($20 + OplMod[ch], pt.m20);  OplWrite($20 + OplCar[ch], pt.c20);
  OplWrite($40 + OplMod[ch], pt.m40);  OplWrite($40 + OplCar[ch], pt.c40);
  OplWrite($60 + OplMod[ch], pt.m60);  OplWrite($60 + OplCar[ch], pt.c60);
  OplWrite($80 + OplMod[ch], pt.m80);  OplWrite($80 + OplCar[ch], pt.c80);
  OplWrite($E0 + OplMod[ch], pt.mE0);  OplWrite($E0 + OplCar[ch], pt.cE0);
  OplWrite($C0 + ch, pt.fbC0);
end;

procedure AdlibNote(voice: Byte; note: Byte);
var block: Byte; f: Real; fnum: Word;
begin
  OplWrite($B0 + voice, 0);     { отпустить клавишу }
  if note = 0 then Exit;
  block := note div 12 - 1;     { октава }
  if block > 7 then block := 7;
  f := Freq(note);
  fnum := Round(f * 1048576.0 / 49716.0 / (1 shl block));   { число частоты, 10 бит }
  OplWrite($A0 + voice, Lo(fnum));
  OplWrite($B0 + voice, $20 or (block shl 2) or (Hi(fnum) and 3));   { нажать клавишу }
end;

procedure PlayAdlib;
begin
  OplSetup;
  PlayChip(AdlibNote);
  OplWrite($B0, 0); OplWrite($B1, 0);
end;

procedure PlayAdlibPatches;
{ A: первые два такта мелодии (все ноты - четверти) четырьмя FM-инструментами по очереди }
const Bars12: array[1..8] of Byte = (64, 64, 65, 67, 67, 65, 64, 62);
var p, i: Integer;
begin
  OplSetup;
  p := 1;
  while (p <= NPATCH) and not Stop do
  begin
    Writeln('  ', Patches[p].name);
    OplPatch(0, Patches[p]);
    i := 1;
    while (i <= 8) and not Stop do
    begin
      AdlibNote(0, Bars12[i]);
      WaitMs(QUARTER);
      Inc(i);
    end;
    AdlibNote(0, 0);
    WaitMs(400);
    Inc(p);
  end;
  OplWrite($B0, 0);
end;

{ ---------- 4. Covox Sound Master: AY-3-8930, порт 240h ---------- }

const SM_BASE = $240;           { карта бывала и на 220h - эта программа туда не пишет }

procedure AyWrite(reg, val: Byte);
begin
  Port[SM_BASE] := reg;         { выбрать регистр }
  Port[SM_BASE + 1] := val;     { записать в него }
end;

procedure SoundMasterNote(voice: Byte; note: Byte);
var n: Word;
begin
  if note = 0 then
    AyWrite(8 + voice, 0)       { громкость 0 }
  else
  begin
    n := Round(1773400.0 / (16.0 * Freq(note)));   { период, 12 бит }
    AyWrite(voice * 2, Lo(n));
    AyWrite(voice * 2 + 1, Hi(n) and $0F);
    AyWrite(8 + voice, 15 - voice * 3);
  end;
end;

procedure PlaySoundMaster;
begin
  AyWrite(7, $3C);              { тон на голосах A и B; голос C и шум выключены }
  PlayChip(SoundMasterNote);
  AyWrite(8, 0); AyWrite(9, 0); AyWrite(10, 0);
end;

{ ---------- 5. Roland MPU-401: MIDI, порты 330h и 331h ---------- }

procedure MidiOut(b: Byte);     { отправить один байт MIDI }
var t: Word;
begin
  t := 0;
  while ((Port[$331] and $40) <> 0) and (t < 60000) do Inc(t);   { ждем, пока примет }
  Port[$330] := b;
end;

var MidiLast: array[0..1] of Byte;

procedure MidiNote(voice: Byte; note: Byte);
begin
  if MidiLast[voice] <> 0 then
  begin
    MidiOut($80 + voice); MidiOut(MidiLast[voice]); MidiOut(0);   { отпустить ноту }
  end;
  MidiLast[voice] := note;
  if note <> 0 then
  begin
    MidiOut($90 + voice); MidiOut(note); MidiOut(100 - voice * 20);  { нажать ноту }
  end;
end;

procedure PlayMidi;
var t: Word;
begin
  Port[$331] := $FF;            { сброс }
  for t := 1 to 30000 do ;
  Port[$331] := $3F;            { режим UART: байты идут прямо в MIDI }
  for t := 1 to 30000 do ;
  MidiOut($C0); MidiOut(0);     { канал 1: инструмент General MIDI 1, пианино }
  MidiOut($C1); MidiOut(32);    { канал 2: инструмент 33, акустический бас }
  MidiLast[0] := 0; MidiLast[1] := 0;
  PlayChip(MidiNote);
end;

{ ---------- волна для ЦАП: вычисляем каждое число ---------- }

procedure MakeWave(rate: Word);
{ Два голоса: мелодия - синусоида, бас - тихая прямоугольная волна.
  Каждое число = 128 + сумма голосов. Нота мелодии звучит ровно, пока
  "клавишу держат": быстро нарастает (5 мс), в конце плавно затихает
  (30 мс) и короткая пауза (20 мс) перед следующей нотой. }
var i: Integer;
    n, len, bar: Word;
    ph1, inc1, ph2, inc2: LongInt;
    amp, v: Integer;
    eighthLen: Word;
begin
  eighthLen := LongInt(rate) * QUARTER div 2000;   { сколько чисел в восьмой }
  WaveLen := 0;
  ph1 := 0; ph2 := 0; inc2 := 0;
  for i := 1 to NNOTES do
  begin
    len := MelLen[i] * eighthLen;
    inc1 := Round(Freq(MelNote[i]) * 256.0 * 256.0 / rate);   { шаг по таблице синуса, 8.8 }
    for n := 0 to len - 1 do
    begin
      if (WaveLen mod (8 * eighthLen)) = 0 then
      begin
        bar := WaveLen div (8 * eighthLen) + 1;
        if bar > NBARS then bar := NBARS;
        inc2 := Round(Freq(BassNote[bar]) * 256.0 * 256.0 / rate);
      end;
      if n < 40 then amp := LongInt(90) * n div 40          { нарастание }
      else if n >= len - 160 then amp := 0                  { пауза }
      else if n >= len - 400 then amp := LongInt(90) * (len - 160 - n) div 240   { затухание }
      else amp := 90;                                       { звучит ровно }
      v := SinTab[(ph1 shr 8) and 255] * amp div 128;
      if (ph2 shr 15) and 1 = 0 then v := v + 24 else v := v - 24;
      Wave^[WaveLen] := 128 + v;
      Inc(WaveLen);
      ph1 := ph1 + inc1;
      ph2 := ph2 + inc2;
    end;
  end;
end;

{ ---------- 6. Covox Speech Thing: ЦАП на LPT2 (278h) ---------- }

procedure CovoxPlay(buf: PBuf; len: Word);
{ процессор сам отдает каждое число и ждет по таймеру следующего - 8000 раз в секунду }
var i: Word; last, now: Word;
const STEP = PIT_HZ div RATE;
begin
  last := PitCount;
  i := 0;
  while (i < len) and not Stop do
  begin
    Port[$278] := buf^[i];
    repeat
      now := PitCount;
    until Word(last - now) >= STEP;
    last := last - STEP;
    if (i and 1023) = 0 then
      if KeyPressed then if ReadKey = #27 then Stop := True;
    Inc(i);
  end;
  Port[$278] := 128;
end;

procedure PlayCovox;
begin
  Writeln('  играю: компьютер сам отдает ЦАП 8000 чисел в секунду');
  CovoxPlay(Wave, WaveLen);
  if not Stop then
  begin
    WaitMs(400);
    CovoxPlay(Speech, SpeechLen);
  end;
end;

{ ---------- 7. Disney Sound Source: ЦАП с очередью на LPT1 (378h) ---------- }

procedure Resample7(src: PBuf; srcLen: Word; dst: PBuf; var dstLen: Word);
{ 8000 -> 7000 чисел в секунду: из каждых 8 чисел остаются 7. Делается один
  раз при запуске: пока звук играет, процессору на это не хватило бы времени. }
var i: Word;
begin
  dstLen := LongInt(srcLen) * DSS_RATE div RATE;
  for i := 0 to dstLen - 1 do
    dst^[i] := src^[LongInt(i) * RATE div DSS_RATE];
end;

procedure DssPlay(buf: PBuf; len: Word);
var i, t: Word;
begin
  i := 0;
  Port[$37A] := $04;
  while (i < len) and not Stop do
  begin
    t := 0;
    while ((Port[$379] and $40) <> 0) and (t < 50000) do Inc(t);   { очередь полна - ждем }
    Port[$378] := buf^[i];
    Port[$37A] := $0C;          { строб (линия SELECT): число уходит в очередь }
    Port[$37A] := $04;
    if (i and 1023) = 0 then
      if KeyPressed then if ReadKey = #27 then Stop := True;
    Inc(i);
  end;
end;

procedure PlayDisney;
begin
  Writeln('  играю: Sound Source сам берет 7000 чисел в секунду из очереди');
  DssPlay(Wave7, Wave7Len);
  if not Stop then
  begin
    WaitMs(400);
    DssPlay(Speech7, Speech7Len);
  end;
end;

{ ---------- 8. Sound Blaster: ЦАП и DMA, порт 220h ---------- }

const SB = $220;
var DmaBuf: PBuf;               { 64000 байт: внутри - окно на 32000, удобное для DMA }
    DmaOfs: Word;

procedure DspWrite(b: Byte);    { послать байт в DSP }
var t: Word;
begin
  t := 0;
  while ((Port[SB + $C] and $80) <> 0) and (t < 60000) do Inc(t);
  Port[SB + $C] := b;
end;

function DspReset: Boolean;     { сброс DSP; ответ AAh - Sound Blaster на месте }
var t: Word;
begin
  Port[SB + 6] := 1;
  for t := 1 to 1000 do ;
  Port[SB + 6] := 0;
  t := 0;
  while ((Port[SB + $E] and $80) = 0) and (t < 60000) do Inc(t);
  DspReset := Port[SB + $A] = $AA;
end;

procedure FindDmaWindow;
{ Передача DMA не может пересечь границу 64 КБ физической памяти. }
var phys, edge: LongInt;
begin
  phys := LongInt(Seg(DmaBuf^)) * 16 + Ofs(DmaBuf^);
  edge := (phys or $FFFF) + 1;  { следующая граница 64 КБ }
  if edge - phys >= 32000 then DmaOfs := 0
  else DmaOfs := edge - phys;
end;

procedure SbPlayChunk(src: PBuf; from, len: Word);
var phys: LongInt; i: Word;
begin
  for i := 0 to len - 1 do DmaBuf^[DmaOfs + i] := src^[from + i];
  phys := LongInt(Seg(DmaBuf^)) * 16 + Ofs(DmaBuf^) + DmaOfs;
  { контроллер DMA, канал 1: из памяти в Sound Blaster, один проход }
  Port[$0A] := $05;                       { замаскировать канал 1 }
  Port[$0C] := 0;                         { сбросить триггер байтов }
  Port[$0B] := $49;                       { один проход, чтение памяти, канал 1 }
  Port[$02] := Lo(Word(phys)); Port[$02] := Hi(Word(phys));
  Port[$83] := phys shr 16;               { страница }
  Port[$03] := Lo(len - 1); Port[$03] := Hi(len - 1);
  Port[$0A] := $01;                       { снять маску канала 1 }
  { Sound Blaster: вывод 8 бит через DMA, один проход }
  DspWrite($14); DspWrite(Lo(len - 1)); DspWrite(Hi(len - 1));
  { процессор свободен; ждем, пока кусок отыграет, и подтверждаем прерывание }
  for i := 1 to len div 80 do
    if not Stop then WaitTicks(PIT_HZ div 100);
  i := Port[SB + $E];
end;

procedure SbPlay(src: PBuf; len: Word);
var from, n: Word;
begin
  from := 0;
  while (from < len) and not Stop do
  begin
    n := len - from;
    if n > 28160 then n := 28160;         { половина мелодии: 4 такта }
    SbPlayChunk(src, from, n);
    from := from + n;
  end;
end;

function SbStart: Boolean;      { найти Sound Blaster и настроить на 8000 чисел в секунду }
begin
  SbStart := False;
  if not DspReset then Exit;
  DspWrite($D1);                          { включить выход ЦАП }
  DspWrite($40); DspWrite(256 - 1000000 div RATE);   { постоянная времени: 8000 Гц }
  SbStart := True;
end;

procedure PlaySoundBlaster;
begin
  if not SbStart then
  begin
    Writeln('  Sound Blaster на 220h не найден');
    Exit;
  end;
  Writeln('  играю: числа подает контроллер DMA, процессор свободен');
  SbPlay(Wave, WaveLen);
  if not Stop then
  begin
    WaitMs(400);
    SbPlay(Speech, SpeechLen);
  end;
  DspWrite($D3);                          { выключить выход ЦАП }
end;

procedure DacPlay(buf: PBuf; len: Word);
{ сыграть через Sound Blaster, а если его нет - через Covox Speech Thing }
begin
  if SbStart then
  begin
    SbPlay(buf, len);
    DspWrite($D3);
  end
  else CovoxPlay(buf, len);
end;

{ ---------- D. Качество цифрового звука: биты и частота дискретизации ---------- }

procedure PlayQuality;
const
  NSTEP = 7;
  Bits: array[1..NSTEP] of Byte = (8, 4, 2, 1, 8, 8, 8);
  Hold: array[1..NSTEP] of Byte = (1, 1, 1, 1, 2, 4, 8);
  Info: array[1..NSTEP] of String[60] = (
    '8 бит, 8000 чисел в секунду: 256 ступенек',
    '4 бита: 16 ступенек',
    '2 бита: 4 ступеньки',
    '1 бит: 2 ступеньки - только "вкл" и "выкл", как у пищалки',
    '8 бит, 4000 чисел в секунду',
    '8 бит, 2000 чисел в секунду',
    '8 бит, 1000 чисел в секунду');
var s, mask, half, q: Byte; i, src: Word;
begin
  if SpeechLen = 0 then
  begin
    Writeln('  нет файла SPEECH.RAW: запускайте SOUNDS из его каталога');
    Exit;
  end;
  s := 1;
  while (s <= NSTEP) and not Stop do
  begin
    Writeln('  ', Info[s]);
    { оставить в каждом числе только старшие биты: ступенек меньше }
    mask := Lo($FF shl (8 - Bits[s]));
    half := 128 shr Bits[s];    { середина ступеньки, чтобы звук не сдвигался }
    for i := 0 to SpeechLen - 1 do
    begin
      src := i - i mod Hold[s]; { повторять число Hold раз: чисел в секунду меньше }
      q := Speech^[src];
      if Bits[s] < 8 then q := (q and mask) + half;
      Work^[i] := q;
    end;
    DacPlay(Work, SpeechLen);
    WaitMs(600);
    Inc(s);
  end;
end;

{ ---------- G. Игра "Угадай устройство" ---------- }

procedure Game;
const
  Names: array[1..5] of String[50] = (
    'пищалка - PC speaker',
    'PSG - генератор звука SN76489 (Tandy)',
    'FM-синтез - микросхема OPL2 (AdLib)',
    'MIDI - ноты для синтезатора (MPU-401)',
    'ЦАП - волна, записанная числами');
var dev, last, score, total: Integer; c: Char;
begin
  Randomize;
  last := 0; score := 0; total := 0;
  repeat
    repeat dev := Random(5) + 1 until dev <> last;
    last := dev;
    Writeln;
    Writeln('Слушай мелодию...');
    Stop := False;
    case dev of
      1: PlaySpeaker;
      2: PlayTandyTune;
      3: PlayAdlib;
      4: PlayMidi;
      5: DacPlay(Wave, WaveLen);
    end;
    if Stop then Exit;          { Esc во время мелодии - в меню }
    Writeln('Что это было?  1 - пищалка  2 - PSG  3 - FM  4 - MIDI  5 - ЦАП');
    Write('Твой ответ: ');
    repeat c := ReadKey until (c in ['1'..'5']) or (c = #27);
    if c = #27 then Exit;
    Writeln(c);
    Inc(total);
    if Ord(c) - Ord('0') = dev then
    begin
      Writeln('Угадал!');
      Inc(score);
    end
    else Writeln('Нет, это ', Names[dev], '.');
    Writeln('Угадано ', score, ' из ', total, '.   Enter - еще раз, Esc - в меню');
    repeat c := ReadKey until (c = #13) or (c = #27);
  until c = #27;
end;

{ ---------- меню ---------- }

procedure LoadSpeech;
var f: file; n: Word;
begin
  SpeechLen := 0;
  Assign(f, 'SPEECH.RAW');
  {$I-} Reset(f, 1); {$I+}
  if IOResult <> 0 then Exit;
  BlockRead(f, Speech^, 32000, n);
  Close(f);
  SpeechLen := n;
end;

procedure Menu;
begin
  Writeln;
  Writeln('SOUNDS - звуковые устройства PC: начало "Оды к радости" Бетховена');
  Writeln;
  Writeln('  1  PC speaker (1981)          пищалка: одна прямоугольная волна');
  Writeln('  2  Tandy / PCjr (1984)        PSG SN76489: 3 прямоугольных голоса и шум');
  Writeln('  3  AdLib (1987)               FM-синтез, микросхема OPL2: 9 голосов');
  Writeln('  4  Covox Sound Master (1989)  PSG AY-3-8930, только порт 240h!');
  Writeln('  5  Roland MPU-401 (1984)      ноты MIDI для синтезатора');
  Writeln('  6  Covox Speech Thing (1986)  ЦАП на LPT2: любая волна');
  Writeln('  7  Disney Sound Source (1990) ЦАП с очередью на LPT1');
  Writeln('  8  Sound Blaster (1989)       ЦАП, числа подает DMA; и OPL2');
  Writeln('  A  AdLib: четыре инструмента  флейта, пианино, труба, колокольчик');
  Writeln('  D  Качество цифрового звука   меньше бит, меньше чисел в секунду');
  Writeln('  G  Игра "Угадай устройство"');
  Writeln;
  Writeln('  Esc - остановить звук / выйти.');
  Writeln('  Устройство сначала включается в настройках эмулятора (Win+F11).');
  Write('Выбор: ');
end;

procedure PlayDevice(c: Char);
begin
  Stop := False;
  case c of
    '1': PlaySpeaker;
    '2': PlayTandy;
    '3': PlayAdlib;
    '4': PlaySoundMaster;
    '5': PlayMidi;
    '6': PlayCovox;
    '7': PlayDisney;
    '8': PlaySoundBlaster;
    'a', 'A': PlayAdlibPatches;
    'd', 'D': PlayQuality;
    'g', 'G': Game;
  end;
end;

var c: Char; i: Integer; Arg: String;
begin
  for i := 0 to 255 do SinTab[i] := Round(127 * Sin(i * 2 * Pi / 256));
  GetMem(Wave, 65000);
  GetMem(Speech, 32000);
  GetMem(Work, 32000);
  GetMem(DmaBuf, 64000);
  GetMem(Wave7, 50000);
  GetMem(Speech7, 28000);
  FindDmaWindow;
  LoadSpeech;
  Writeln('SOUNDS: подготовка - вычисляю волну для ЦАП...');
  MakeWave(RATE);
  Resample7(Wave, WaveLen, Wave7, Wave7Len);
  Speech7Len := 0;
  if SpeechLen > 0 then Resample7(Speech, SpeechLen, Speech7, Speech7Len);
  PitMode2;
  if ParamCount > 0 then
  begin
    Arg := ParamStr(1);
    PlayDevice(Arg[1]);         { SOUNDS 3 - сыграть одно устройство и выйти }
  end
  else
    repeat
      Menu;
      c := ReadKey;
      Writeln(c);
      PlayDevice(c);
    until c = #27;
  SpeakerOff;
  PitMode3;
end.
