object Form2: TForm2
  Left = 1530
  Top = 658
  Width = 348
  Height = 147
  Caption = 'Filter'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 58
    Height = 16
    Caption = 'Filter Size'
  end
  object Threshold: TLabel
    Left = 8
    Top = 40
    Width = 61
    Height = 16
    Caption = 'Threshold'
  end
  object Size: TEdit
    Left = 112
    Top = 8
    Width = 209
    Height = 24
    TabOrder = 0
    Text = 'Odd Number Size - Default : 3'
    OnChange = SizeChange
  end
  object Value: TEdit
    Left = 112
    Top = 32
    Width = 209
    Height = 24
    TabOrder = 1
    Text = 'Value - Default : 4'
    OnChange = ValueChange
  end
  object Button1: TButton
    Left = 96
    Top = 72
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = Button1Click
  end
end
