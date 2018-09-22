object Form7: TForm7
  Left = 1539
  Top = 824
  Width = 324
  Height = 180
  Caption = 'Noise'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 34
    Height = 16
    BiDiMode = bdLeftToRight
    Caption = 'Mean'
    ParentBiDiMode = False
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 60
    Height = 16
    Caption = 'Deviation '
  end
  object Edit1: TEdit
    Left = 96
    Top = 8
    Width = 201
    Height = 24
    TabOrder = 0
    Text = 'Value - Default : 3'
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 96
    Top = 48
    Width = 201
    Height = 24
    TabOrder = 1
    Text = 'Value - Default : 10'
    OnChange = Edit2Change
  end
  object Button1: TButton
    Left = 120
    Top = 96
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = Button1Click
  end
end
