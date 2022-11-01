unit U_Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, U_ComWake, CPort, StdCtrls, CPortCtl, ExtCtrls, rxPlacemnt,
  Buttons, ActnList, Grids, ComCtrls;

type
	TWorkMode=(wmWait, wmChipRead, wmChipVerif, wmChipWrite);
  TForm1 = class(TForm)
    Panel1: TPanel;
    ComPort: TComPort;
    ComPKG: TComWake;
    btn_Connected: TBitBtn;
    btn2: TBitBtn;
    frmstrg1: TFormStorage;
    mmo_log: TMemo;
    tmr: TTimer;
    actlst1: TActionList;
    btn1: TBitBtn;
    act_Signatura: TAction;
    act_Erase: TAction;
    btnSignatura: TBitBtn;
    btnErase: TBitBtn;
    btnErase1: TBitBtn;
    dlgOpen: TOpenDialog;
    dlgSave1: TSaveDialog;
    sGrid_MemFileGrid_ChipMem: TStringGrid;
    btnChipVerif: TBitBtn;
    btn5: TSpeedButton;
    act_ChipRead: TAction;
    stat1: TStatusBar;
    pb1: TProgressBar;
    act_ChipVerif: TAction;
    act_ChipWrite: TAction;
    btn3: TBitBtn;
    btn4: TBitBtn;
    act_LockBit: TAction;
    procedure btn2Click(Sender: TObject);
    procedure btn_ConnectedClick(Sender: TObject);
    procedure ComPortAfterOpen(Sender: TObject);
    procedure ComPortAfterClose(Sender: TObject);
    procedure ComPKGCommand(Sender: TObject; cmd: Byte;
      const Data: array of Byte; Count: Integer);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure act_SignaturaUpdate(Sender: TObject);
    procedure act_SignaturaExecute(Sender: TObject);
    procedure act_EraseExecute(Sender: TObject);
    procedure act_EraseUpdate(Sender: TObject);
    procedure btn3Click(Sender: TObject);
    procedure tmrTimer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure act_ChipReadExecute(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure act_ChipVerifExecute(Sender: TObject);
    procedure act_ChipWriteExecute(Sender: TObject);
    procedure act_LockBitExecute(Sender: TObject);
    procedure btn4Click(Sender: TObject);
    procedure ComPortBeforeClose(Sender: TObject);
  private
    WorkMode : TWorkMode;
    FileSizeProgam, address : integer;
    ChipMem : array[0..4096]of byte;
    ChipMaxMemSize : integer;
    NameChip : string;
    procedure AddLog(StrLog:string);
    procedure GetProgrammatorInfo(const Data: array of Byte;
       Count: Integer);
    procedure GetChipSignature(const Data: array of Byte;
      Count: Integer);
    procedure loadFileHex(FileName: string);
    procedure DisplayMem(Length: integer);
    procedure ClearArrayChipMem;
    function ObrabotkaChipRead(const Data: array of Byte;
      Count: Integer): boolean;
    function ObrabotkaChipWrite(const Data: array of Byte):boolean;
    function GetProgramSize(usedMem: integer): integer;
    { Private declarations }
  public
    { Public declarations }
  end;

const
  FirstCaption = '������������ Chip: ';
  cmd_info = 3;
  cmd_ChipSignature = 5;
  cmd_ChipErase = 6;
  CMD_CHIPON   = 7;
  CMD_CHIPOFF  = 8;
  CMD_CHIPREAD = 9;
  CMD_CHIPWRITE = 10;
  CMD_CHIPSETBIT = 11;
  CMD_OFF_LED    = 12;
var
  Form1: TForm1;

implementation

uses U_LookBit;

{$R *.dfm}

procedure TForm1.AddLog(StrLog:string);
begin
  mmo_log.Lines.Add(FormatDateTime('tt',Now) +' - ' + StrLog);
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
 ComPort.Close;
end;

procedure TForm1.btn2Click(Sender: TObject);
begin
  ComPort.ShowSetupDialog;
end;

procedure TForm1.btn_ConnectedClick(Sender: TObject);
begin
if ComPort.Connected then
    ComPort.Close
  else begin
    ComPKG.Reset;
    ComPort.Open;
  end;
end;

procedure TForm1.ComPortAfterOpen(Sender: TObject);
begin
  AddLog('���������������� ���� ������, ����������� � ����������...');
  btn_Connected.Caption := '���������';
  ComPKG.SendCommand(cmd_info,[]);
  tmr.Enabled := true;
end;

procedure TForm1.ComPortAfterClose(Sender: TObject);
begin
   ComPort.Tag := 0;
   AddLog('���������������� ���� ������');
   btn_Connected.Caption := '����������';
end;

procedure TForm1.act_SignaturaUpdate(Sender: TObject);
begin
 (Sender as TAction).Enabled := ComPort.Connected and  (ComPort.Tag in [1,2]);
end;

procedure TForm1.act_SignaturaExecute(Sender: TObject);
begin
  ComPKG.SendCommand(cmd_ChipSignature,[]);
end;

procedure TForm1.act_EraseExecute(Sender: TObject);
begin
  ComPKG.SendCommand(cmd_ChipErase,[]);
end;

procedure TForm1.act_EraseUpdate(Sender: TObject);
begin
 (Sender as TAction).Enabled := ComPort.Connected and  (ComPort.Tag = 2);
end;

procedure TForm1.btn3Click(Sender: TObject);
begin
 if dlgOpen.Execute then
  loadFileHex(dlgOpen.FileName);
end;


function Hex2ToByte(Hex: string):byte;
const
  Convert: array['0'..'f'] of SmallInt =
  (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15);
begin
    if not (Hex[1] in ['0'..'f']) or not (Hex[2] in ['0'..'f']) then raise Exception.Create('Not HEX string');
    Result := Byte((Convert[Hex[1]] shl 4) + Convert[Hex[2]]);
end;

function Hex4ToInt(Hex: string):integer;
var
  res, Code : integer;
begin
  Val('$'+Hex,res,Code);
  if Code <> 0 then raise Exception.Create('Not HEX string');
  Result := res;
end;

function Get_uint16_t(l,h:byte):Word;
begin
 Result := ((h shl 8) or l);
end;

procedure TForm1.DisplayMem(Length: integer);
var
  i, adr, aCol, aRow : integer;
begin
  adr := Length div 16;
  if (Length mod 16 <> 0) then inc(adr);
  sGrid_MemFileGrid_ChipMem.RowCount := adr +1;
  if sGrid_MemFileGrid_ChipMem.RowCount = 1 then sGrid_MemFileGrid_ChipMem.RowCount := 2;
  sGrid_MemFileGrid_ChipMem.FixedRows := 1;
  sGrid_MemFileGrid_ChipMem.Cells[0, 0] := 'Adress';
  for i := 0 to 15 do
   sGrid_MemFileGrid_ChipMem.Cells[i+1, 0] := IntToHex(i,1);
  i := 0;
  aRow := 0;
  aCol := 0;
  while (i < 16 *adr) do begin
    if (i mod 16) = 0 then begin
      aRow := i div 16+1;
      aCol := 0;
      sGrid_MemFileGrid_ChipMem.Cells[0, aRow] := IntToHex(i,4);
    end;
    inc(aCol);
    sGrid_MemFileGrid_ChipMem.Cells[aCol, aRow] := IntToHex(ChipMem[i],2);
    inc(i);
  end;
end;

procedure TForm1.loadFileHex(FileName: string);
var
 F : TextFile;
 recordType,byteCount : Byte;
 i, csum, startAddress, aSizeProgam : integer;
 Str: string;
begin
 aSizeProgam := 0;
 AssignFile(F,FileName);
 FileMode := 0;  {Set file access to read only }
 try
 Reset(F);
   if FileSize(F) = 0 then begin
     MessageDlg('������ ����',mtError,[mbOk],0);
     Exit;
   end;
   AddLog('�������� �����: '+ FileName);
 // test validity of hex file
   while not Eof(F) do begin
     Readln(F,Str);
     if Length(Str) < 11 then begin
       MessageDlg('� ����� ������������ ������ ����� 11 ��������, ���� hex ���������',mtError,[mbOk],0);
       AddLog('� ����� ������������ ������ ����� 11 ��������, ���� hex ���������');
       Exit;
     end;
     if Str[1] <> ':' then begin
       MessageDlg('���� �� ����� �� ���������� � ":", ���� hex ���������',mtError,[mbOk],0);
       AddLog('���� �� ����� �� ���������� � ":", ���� hex ���������');
       Exit;
     end;
     for i := 2 to Length(Str) do begin
       if (Str[i] >= '0') and (Str[i] <= '9') then Continue;
       if (Str[i] >= 'A') and (Str[i] <= 'F') then Continue;
       MessageDlg('� ����� ������������ ������� �������� �� HEX, ���� ���������',mtError,[mbOk],0);
       AddLog('� ����� ������������ ������� �������� �� HEX, ���� ���������');
       Exit;
     end;

     // ��� �����
     i := 2; csum := 0;
     while (i < Length(Str)) do begin
       csum := csum + Hex2ToByte(Copy(Str,i,2));
       inc(i,2);
     end;
     csum := csum and $0F;
     if csum <> 0 then begin
       MessageDlg('�� ��������� ����������� �����, ���� hex ���������',mtError,[mbOk],0);
       AddLog('�� ��������� ����������� �����, ���� hex ���������');
       Exit;
     end;

     //���������� ���� � ������
     byteCount := Hex2ToByte(Copy(Str,2,2));
     if ((byteCount*2 + 11)  <> Length(Str)) then begin
       MessageDlg('� ������ ��������� ���������� ���� �� ��������� � ����������� ������������, ���� hex ���������',mtError,[mbOk],0);
       AddLog('� ������ ��������� ���������� ���� �� ��������� � ����������� ������������, ���� hex ���������');
       Exit;
     end;
     // ������� ���� �������
      recordType := Hex2ToByte(Copy(Str,8,2));
     if (recordType>1) then begin
       MessageDlg('���� hex ����� ����� �������� ��������� ������, ����� ���� � ��������� �� ����� ���� ��������',mtError,[mbOk],0);
       AddLog('���� hex ����� ����� �������� ��������� ������, ����� ���� � ��������� �� ����� ���� ��������');
       Exit;
     end;
     if (recordType = 0) then begin
       byteCount    := Hex2ToByte(Copy(Str,2,2)); // ���������� ����
       startAddress := Hex4ToInt(Copy(Str,4,4)); //�����
       if (startAddress + byteCount - 1 > aSizeProgam) then begin
         aSizeProgam := startAddress + byteCount - 1;
       end;
     end;
   end;
   if aSizeProgam > 4096 then begin
    AddLog('������: ������ ����������� ��������: '+ IntToStr(aSizeProgam)+ ' ���� ��� ������ ����������� ��������� 4096 ����');
    MessageDlg('������: ������ ����������� ��������: '+ IntToStr(aSizeProgam)+ ' ���� ��� ������ ����������� ��������� 4096 ����',mtError,[mbOk],0);
    Exit;
   end;
   AddLog('������ ��������: '+ IntToStr(aSizeProgam)+ ' ����');
   Caption := FirstCaption + NameChip+' - ['+FileName+']';
   FileSizeProgam := aSizeProgam;
   // � ������ ��� ������ ��������� � fileMem ������
   ClearArrayChipMem; // ������� ����� ����� ������
   Reset(F); // ����� � ������ �����
   while not Eof(F) do begin
     Readln(F,Str);
     recordType := Hex2ToByte(Copy(Str,8,2));
     if recordType = 0 then begin  // ���� ���� �������� ������
       byteCount    := Hex2ToByte(Copy(Str,2,2));
       startAddress := Hex4ToInt(Copy(Str,4,4));
       for i := 0 to byteCount-1 do
          ChipMem[startAddress + i] := Hex2ToByte(Copy(Str,10+i*2,2)); // �������� � ����� ������ �� �����
     end;
   end;
   DisplayMem(FileSizeProgam);
 finally
   CloseFile(F);
 end;
end;


procedure TForm1.tmrTimer(Sender: TObject);
begin
 tmr.Enabled := false;
 AddLog('���������� �� ��������, �������� �������� �����, ������� ������� �� ����������');
 ComPort.Connected := false;

end;

procedure TForm1.FormCreate(Sender: TObject);
begin
 Caption := FirstCaption+'??? - []';
 FileSizeProgam := 0;
 ChipMaxMemSize := 0;
 pb1.Parent := stat1;
  pb1.Top := 2;
 DisplayMem(0); 
end;

procedure TForm1.FormResize(Sender: TObject);
begin
 stat1.Panels[0].Width := Width - 325;
 pb1.Left := stat1.Panels[0].Width+2;
end;

procedure TForm1.ClearArrayChipMem;
var
i: integer;
begin
  for i := 0 to 4096 do ChipMem[i] := $FF;
end;

procedure TForm1.GetProgrammatorInfo(const Data: array of Byte; Count: Integer);
var
  i : integer;
  str : string;
begin
  tmr.Enabled := false;
  str := '';
  for i := 1 to Count do begin
    str := str + char(Data[i-1]);
  end;
  AddLog('��������� � �������������: '+str);
  if str = 'Programming AT89Cx051' then
    ComPort.Tag := 1;
end;

procedure TForm1.GetChipSignature(const Data: array of Byte; Count: Integer);
var
  str : string;
begin
  NameChip := 'unknown';
  str := '��������� ����: ';
  if (data[0] = 30) then
    str := str + '������������� Atmel  '
  else
    str := str + IntToHex(Data[0],2)+ '  ';
  case data[1] of
    33 :  begin
       NameChip := 'AT89C2051';
       str := str + '���: '+NameChip;
       ChipMaxMemSize := 2048;
       ComPort.Tag := 2;
    end;
    65 :  begin
       NameChip := 'AT89C4051';
       str := str + '���: '+NameChip;
       ChipMaxMemSize := 4096;
       ComPort.Tag := 2;
    end;
   else
    str := str + IntToHex(Data[1],2)+ ' ';
  end;
  str := str +' Dop:' + IntToHex(Data[2],2)+ ' ';
  Caption := FirstCaption + NameChip;
  AddLog(str);
end;

procedure TForm1.ComPKGCommand(Sender: TObject; cmd: Byte;
  const Data: array of Byte; Count: Integer);
begin
  case cmd of
    cmd_info: GetProgrammatorInfo(Data, Count); // ���������� � �������������
    cmd_ChipSignature: GetChipSignature(Data, Count); // ���������� � ���������� ����
    CMD_CHIPON: begin // ���������� ������� ���� ����� �������� ������
      address := 0;
      case WorkMode of
        wmChipRead, wmChipVerif : begin // ������ ���
          AddLog('�������� ��������� �������� �� ����');
          if WorkMode = wmChipRead then ClearArrayChipMem;
          pb1.Max := ChipMaxMemSize;
          ComPKG.SendCommand(CMD_CHIPREAD,[]); // ������� �� ���������� ������
        end;
        wmChipWrite : begin
          AddLog('�������� ���������� �������� � ���');
          pb1.Max := FileSizeProgam;
          ObrabotkaChipWrite([0,0]);
        end;
      end;
    end;
    CMD_CHIPOFF: AddLog('������� ���� ���������');
    CMD_CHIPREAD:begin // �������� ������ �� ������ ���� (16 ����) � ������ (ChipAddress,1,2 .. 16 ����)
       if not ObrabotkaChipRead(Data,Count) then begin
         WorkMode := wmWait;
         ComPKG.SendCommand(CMD_CHIPOFF,[]);
         pb1.Position := 0;
         Exit;
       end;
       pb1.Position := address;
       if address >= ChipMaxMemSize then begin // ���� ��� ��������
         ComPKG.SendCommand(CMD_CHIPOFF,[]); // ��������� �������
         pb1.Position := 0;
         if WorkMode = wmChipRead then begin
           AddLog('��� ��������');
           FileSizeProgam := GetProgramSize(ChipMaxMemSize);
           DisplayMem(FileSizeProgam);
         end else begin
           AddLog('��� ������, ����������� �� ���������');
           MessageDlg('��� ������, ����������� �� ���������',mtInformation,[mbOk],0);
         end;
         WorkMode := wmWait;
       end else // ���� ��� ������
         ComPKG.SendCommand(CMD_CHIPREAD,[]); // ������� �� ���������� ������
    end;
    CMD_CHIPWRITE:begin
       pb1.Position := address;
       if address > FileSizeProgam then begin // ���� ��� �������
         WorkMode := wmWait;
         ComPKG.SendCommand(CMD_CHIPOFF,[]);
         pb1.Position := 0;
         AddLog('������ � ��� ��������');
         MessageDlg('������ � ��� ��������',mtInformation,[mbOk],0);
       end else begin
         if not ObrabotkaChipWrite(Data) then begin // ��������� ������
           WorkMode := wmWait;
           ComPKG.SendCommand(CMD_CHIPOFF,[]);
           pb1.Position := 0;
           Exit;
         end;
       end;
    end;
    CMD_CHIPERASE: begin
       AddLog('��� ������');
       if WorkMode = wmChipWrite then
          ComPKG.SendCommand(CMD_CHIPON,[]);
    end;
    CMD_CHIPSETBIT: AddLog('��� ������������');
  end;


end;

function TForm1.ObrabotkaChipRead(const Data: array of Byte; Count: Integer):boolean;
var
 i, putAdress : integer;
begin

 putAdress := Get_uint16_t(data[0],data[1]);
 if putAdress <> address then begin
    MessageDlg('���������������� �������',mtError,[mbOk],0);
    AddLog('���������������� �������');
    Result := false;
    Exit;
 end;
 for i := 0 to 15 do begin
   case WorkMode of
     wmChipRead : ChipMem[putAdress+i] := Data[2+i];
     wmChipVerif: begin
       if ChipMem[putAdress+i] <> Data[2+i] then begin
         MessageDlg('������ �� ���������',mtError,[mbOk],0);
         AddLog('������ �� ���������');
         Result := False;
         Exit;
       end;

     end;
   end;
 end;
 address := address + 16;
 Result := true;
end;

function TForm1.ObrabotkaChipWrite(const Data: array of Byte):boolean;
var
 i, putAdress : integer;
 Target : array[0..15]of byte;
begin
 putAdress := Get_uint16_t(data[0],data[1]);
 if putAdress <> address then begin
    MessageDlg('���������������� �������',mtError,[mbOk],0);
    AddLog('���������������� �������');
    Result := false;
    Exit;
 end;
 for i := 0 to 15 do
   Target[i] := ChipMem[putAdress+i];

 ComPKG.SendCommand(CMD_CHIPWRITE,Target);
 address := address + 16;
 Result := true;
end;


procedure TForm1.act_ChipReadExecute(Sender: TObject);
begin
  WorkMode := wmChipRead;
  AddLog('������ ������ �� ����');
  ComPKG.SendCommand(CMD_CHIPON,[]); // �������� �������
end;



procedure TForm1.act_ChipVerifExecute(Sender: TObject);
begin
  if FileSizeProgam = 0 then raise  Exception.Create('�� ������ ���� �������� ��� �����������');
  WorkMode := wmChipVerif;
  AddLog('������ ������ �� ����');
  ComPKG.SendCommand(CMD_CHIPON,[]); // �������� �������
end;

procedure TForm1.act_ChipWriteExecute(Sender: TObject);
begin
 if FileSizeProgam = 0 then raise  Exception.Create('�� ������ ���� �������� ��� ������');
 if FileSizeProgam > ChipMaxMemSize then raise  Exception.Create('������ �������� ������ ��� ����� ��������� � '+NameChip);
 WorkMode := wmChipWrite;
 ComPKG.SendCommand(cmd_ChipErase,[]);
end;

procedure TForm1.act_LockBitExecute(Sender: TObject);
var
 bit : byte;
begin
 if (F_LookBit.ShowModal = mrOk) and (F_LookBit.rg_LookBit.ItemIndex > 0)  then begin
   bit := F_LookBit.rg_LookBit.ItemIndex;
   if bit = 2 then bit := 3;
   AddLog('��������� ������� ���������� ����');
   // ���������� 1 ��� 3  (� �������� ���� 00000001 ��� 00000011)
   ComPKG.SendCommand(CMD_CHIPSETBIT,[bit]);
 end;
end;


function addChecksum(s:string):string;
var
i,checksum : Word	;
begin
checksum := 0;
  i := 2;
  while (i < Length(s)) do begin
    checksum := checksum  + Hex2ToByte(Copy(S,i,2));
    i := i +2;
  end;
  checksum := checksum and $0F;

  checksum := not byte(checksum);
  checksum := checksum +1;
  checksum := checksum and $0F;
  Result := s + IntToHex(checksum,2);
end;


function TForm1.GetProgramSize(usedMem:integer):integer;
begin
  while (usedMem > 0) and (ChipMem[usedMem - 1] = $FF) do
     dec(usedMem);
  Result := usedMem;
end;

procedure TForm1.btn4Click(Sender: TObject);
var
   F : TextFile;
   i, len, adr, usedMem : integer;
   str : string;
begin
  if not  dlgSave1.Execute then Exit;
  AssignFile(F,dlgSave1.FileName);
  try
    Rewrite(F);
    // ������� �������
    usedMem := ChipMaxMemSize;
    if usedMem = 0 then usedMem := FileSizeProgam;

    usedMem := GetProgramSize(usedMem);
    if usedMem = 0 then begin
      MessageDlg('�������� ������',mtInformation,[mbOk],0);
      Exit;
    end;

    adr:=0;
    while (adr < usedMem) do begin
      len := 16;
      if (adr +15) > usedMem then len := usedMem - adr+1;
      str  := ':'+ IntToHex(len,2) + IntToHex(adr,4) + '00';
      for i := adr to adr+len -1 do
        str := str + IntToHex(ChipMem[i],2);
      Writeln(F,addChecksum(Str));
      inc(adr,16);
    end;
    Writeln(F,':00000001FF');
    AddLog('���� ��������: '+ dlgSave1.FileName);
    AddLog('������ ��������: '+ IntToStr(usedMem)+ ' ����');
    Caption := FirstCaption + NameChip+' - ['+dlgSave1.FileName+']';
  finally
    CloseFile(F);
  end;

end;

procedure TForm1.ComPortBeforeClose(Sender: TObject);
begin
 ComPKG.SendCommand(CMD_OFF_LED,[]);
end;

end.
