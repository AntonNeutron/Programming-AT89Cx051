program Programator;

uses
  Forms,
  U_Main in 'U_Main.pas' {Form1},
  U_LookBit in 'U_LookBit.pas' {F_LookBit};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TF_LookBit, F_LookBit);
  Application.Run;
end.
