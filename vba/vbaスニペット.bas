190813
ExcelVBAでよく使う便利なコード・スニペットまとめ

■■外部ファイル・フォルダ操作
■ ファイル操作
	Mkdir "C:\temp" ' フォルダ作成
	RmDir "C:\temp" ' フォルダ削除
	Name  "C:\temp" As "C:\temp\works"   'フォルダ移動
	Name  "C:\temp" As "C:\works"        'フォルダ名変更
	FileCopy コピー前のファイルのフルパス, コピー後のファイルのフルパス 'ファイルコピー
	Kill     対象ファイルのフルパス 'ファイル削除

■特殊フォルダのパス
	str = ThisWorkbook.Path     '現在操作しているブックのパス
	str = ThisWorkbook.FullName '現在操作しているブックのフルパス
	str = CreateObject("WScript.Shell").SpecialFolders("MyDocuments") 'マイドキュメントのパス
	str = CreateObject("WScript.Shell").SpecialFolders("Desktop")     'デスクトップのパス

■フルパス→パス名・ファイル名を取得
	'文字列切り取りを利用
	file = "C:\folder1\folder2\sample.xls" 'フルパス取得
	i = InStrRev(file, "\") '右から数えて最初の\の位置
	file_name = Right(file, Len(file) - i) 'ファイル名
	path_name = Left(file, i - 1) 'パス名

■ディレクトリ・ファイルが存在確認
	Dim ExistsDirPath As String
	Dim NotExistsDirPath As String
	ExistsDirPath = "D:\tmp\excel"
	NotExistsDirPath = "D:\xxxx"
	Debug.Print "> " & Dir(ExistsDirPath, vbDirectory)
	Debug.Print "> " & Dir(NotExistsDirPath, vbDirectory)
	Debug.Print "> " & (Dir(NotExistsDirPath, vbDirectory) = "")
	> excel
	> 
	> True

    Dir() 関数で存在チェックが行える。
    ディレクトリをチェックする場合は、第二引数に vbDirectory を渡す。
    存在する場合は、どのディレクトリの名前が、存在しない場合は空文字が返される。
    ファイルの場合は、 vbDirectory 無しで Dir() 関数を使えばいい。

■ディレクトリ複数階層をまとめて生成
	Private Declare Function SHCreateDirectoryEx Lib "shell32" Alias "SHCreateDirectoryExA" ( _
	                                                                  ByVal hwnd As Long, _
	                                                                  ByVal pszPath As String, _
	                                                                  ByVal psa As Long) As Long

	' Win API を呼び出しディレクトリを作成する。
	' 作成に成功したら、0が返される。
	' 既にフォルダが存在する場合、自分の環境では 183 が返された。
	Private Function MkDirs(path As String) As Long
	    MkDirs = SHCreateDirectoryEx(0&, path, 0&)
	End Function

	Public Sub ディレクトリ複数階層をまとめて生成()
	    Debug.Print MkDirs("D:\tmp\hoge\fuga")
	End Sub


■■ブック操作
■特殊なブック
	ThisWorkBook    ' VBAコードが記述されているブック
	ActiveWorkBook  ' 現在表示されているブック
		'VBAコードを保存しているブックがどれかを知りたい場合のみThisWorkBookを利用し、
		'それ以外は全てActiveWorkBookを利用すると覚えておいても問題ない。

■ブックを開く
	Dim wb As Workbook
	Set wb = Workbooks.Open("（フルパス）ブック名.xlsm") '戻り値を取得しなくてもいい
	Set wb = Workbooks.Open(Filename:="（フルパス）ブック名.xlsm", ReadOnly:=True) ' 読み取り専用

■ブックを閉じる
	wb.Close
	wb.Close saveChanges:=True '保存して閉じる
	wb.Close saveChanges:=False '保存しないで閉じる

■保存
	wb.Save '上書き保存
	wb.SaveAs "（フルパス）新ブック名.xlsm" '別名保存

■ブックを回す
	For i = 1 To Workbooks.Count
		Debug.Print Workbooks(i).Name
	Next i

	For Each wb In Workbooks
		Debug.Print wb.Name
	Next wb

	Sub 任意のブックが開いているかどうか()
		Dim wb As Workbook, flag As Boolean
		For Each wb In Workbooks
			If wb.Name = "Book1.xlsx" Then
				flag = True
				Exit For
			End If
		Next wb
		If flag = True Then
			MsgBox "Book1.xlsxを開いています"
		Else
			MsgBox "Book1.xlsxは開いていません"
		End If
	End Sub

■アクティブ化
	Workbooks("Book1.xlsx").Activate ' ブック「Book1.xlsx」をアクティブ

■■シート操作
■ワークシートの追加
	Dim ws As Worksheet
	Set ws = Worksheets.Add()     '戻り値を取得しなくてもいい
	Activesheet.Name = "シート名" ' 追加シートがActiveなのでアクティブシートに名前を付ける。
	str = ActiveSheet.Name '追加したシートがActiveに
	''シートを最後尾に作成する
	Worksheets.Add After:=Worksheets(Worksheets.Count) 

■ワークシートの複写
	Sheets("Sheets Name").Copy Before:=Sheets(Sheets.Count)  '最後尾にシートコピー
	Sheets("Sheets Name").Copy Before:=Sheets(Sheets.Count)  '最後尾の一つ前にシートコピー
	str = ActiveSheet.Name '複写したシートがActiveに

■ワークシートの削除
'アクティブシートを削除
	Application.DisplayAlerts = False ' 警告解除
	ws.Delete
	Application.DisplayAlerts = True
'指定シートを削除
	Worksheets(1).Delete ''インデックス番号1のシートを削除
	Worksheets("sheetA").Delete ''シート名「sheetA」を削除

■アクティブ化
	Worksheets("Sheet2").Activate

■シートを回す
	For Each objSheet In ThisWorkbook.Worksheets
		Debug.Print objSheet.Name
	Next

	For i = 1 To ThisWorkbook.Worksheets.Count
		Debug.Print ThisWorkbook.Worksheets(i).Name
	Next

■別シートのセル設定
	Sheets("Sheet2").Select
	Range("A1") = 1000

■指定した名前のワークシートがアクティブかどうか判定
	Private Function IsActiveSheet(sheetName As String) As Boolean
		On Error Resume Next
		Err.Clear
		Sheets(sheetName).Activate
		If Err.Number > 0 Then
		    IsActiveSheet = False
		Else
		    IsActiveSheet = True
		End If
		On Error GoTo 0
	End Function



■■行・列操作
■行の指定、列の指定、シート全体の指定
	Cells(1,2).EntireRow    '' 1行目全体
	Cells(1,2).EntireColumn '' B列全体
	Cells                   '' 全体。Cellsと記述するだけ

■行・列、範囲を削除
	Range("A1:C3").EntireRow.Delete    ''Rangeで指定した範囲の行全体を削除
	Range("A1:C3").EntireColumn.Delete ''Rangeで指定した範囲の列全体を削除
	Range("A1:C3").Delete              ''Rangeで指定した範囲を削除します。
	Range("A1:C3").Delete Shift:=xlShiftUp ''Rangeで指定した範囲を削除し、上方向にシフト


■幅・高さ設定
列の幅を取得
	d = Columns(2).ColumnWidth
	d = Columns("B").Width
セル「A1」の列
	d = Range("A1").EntireColumn.ColumnWidth
	d = Range("A1").EntireColumn.Width
複数列の幅を取得
	範囲のすべての列の幅が同じときに 1 列の幅を取得します。違う幅が含まれているときは Null になります。
	d = Range("B:C").ColumnWidth                  ' B ～ C 列の幅を取得
	d = Range(Columns(2), Columns(3)).ColumnWidth ' B ～ C 列の幅を取得
	d = Range("D:D").ColumnWidth                  ' D 列の幅を取得
範囲の幅の合計を取得
	d = Range("B:C").Width '  B ～ C 列の幅の合計を取得

列の幅を設定
	Columns(2).ColumnWidth = 20
	Columns("B").ColumnWidth = 20
	Worksheets("Sheet2").Columns(1).ColumnWidth = 20 ' Sheet2 の列の幅を設定
	Worksheets(2).Columns(1).ColumnWidth = 20        ' Sheet2 の列の幅を設定
セル「A1」の列に幅を設定
	Range("A1").EntireColumn.ColumnWidth = 20
複数列の幅を設定
	各列の幅が指定した幅になります。
	Range("B:C").ColumnWidth = 20
	Range(Columns(2), Columns(3)).ColumnWidth = 20
列の幅を自動設定
	Columns(2).AutoFit             ' B 列の幅を自動調整
	Range("B:C").Columns.AutoFit   ' B ～ C 列の幅を自動調整
	Range("B1:C3").Columns.AutoFit ' B ～ C 列の幅をセル「B1 ～ C3」に合わせて自動調整

■高さの設定
行の高さを取得
	d = Rows(2).RowHeight
	d = Rows(2).Height
セル「A1」の行を高さ
	d = Range("A1").EntireRow.RowHeight
	d = Range("A1").EntireRow.Height
複数行の高さを取得
	範囲のすべての行の高さが同じときに 1 行の高さを取得します。違う高さが含まれているときは Null になります。
	d = Range("2:3").RowHeight ' 2 ～ 3 行目の高さを取得
	d = Range("4:4").RowHeight ' 4 行目の高さを取得
範囲の高さの合計を取得
	d = Range("2:3").Height ' 2 ～ 3 行目の高さの合計を取得

行の高さを設定する
	Rows(2).RowHeight = 20
	Worksheets("Sheet2").Rows(1).RowHeight = 20 ' Sheet2 の行の高さを設定
	Worksheets(2).Rows(1).RowHeight = 20 ' Sheet2 の行の高さを設定
セル「A1」の行に高さを設定
	Range("A1").EntireRow.RowHeight = 20
複数行の高さを設定する
	各行の高さが指定した高さになります。
	Range("2:3").RowHeight = 20
行の高さを自動設定する
	Rows(2).AutoFit             ' 2 行目の高さを自動調整
	Range("2:3").Rows.AutoFit   ' 2 ～ 3 行目の高さを自動調整
指定したシートの行を高さする
	

■■セル操作
'// https://docs.microsoft.com/ja-jp/office/vba/api/excel.range(object)
'// https://excel-ubara.com/EXCEL/EXCEL906.html
'// https://excel-ubara.com/EXCEL/EXCEL907.html

■Range型が初期値チェック
'もし、aが空だったら
	If a Is Nothing Then　
'もし、aが空じゃなかったら
	If Not a Is Nothing Then

■空欄判定
'エラーでなくて、式がなくて、空である。
	If Not IsError(Cells(i, 1)) Then
		If Not Cells(i, 1).HasFormula Then
			If Cells(i, 1) = "" Then
				'処理
			End If
		End If
	End If

	                  |       |何も  |                 |                 |     
	セルの内容        |値"ABC"|入れて|=1/0             |=1/0             |=""  
	                  |       |いない|                 |                 |     
	------------------+-------+------+-----------------+-----------------+-----
	セル表示状態      |ABC    |      |#DIV/0           |#DIV/0           |     
	Range = ""        |FALSE  |TRUE  |型が一致しません |型が一致しません |TRUE 
	Len(Range) = 0    |FALSE  |FALSE |型が一致しません |型が一致しません |FALSE
	Range.HasFormula  |FALSE  |FALSE |TRUE             |FALSE            |TRUE 
	IsError(Range)    |FALSE  |FALSE |TRUE             |TRUE             |FALSE
	IsEmpty(Range)    |FALSE  |TRUE  |FALSE            |FALSE            |FALSE
	Range = Empty     |FALSE  |TRUE  |型が一致しません |型が一致しません |TRUE 
	Range.Formula = ""|FALSE  |TRUE  |FALSE            |FALSE            |FALSE


■セル指定
	Dim rng As range
	Set rng = sht.Range("A1")
	Set rng = sht.Range("A:A")
	Set rng = sht.Range("セルにつけた名前")
	Set rng = sht.Cells(1, 2)
	Range("A1:C3")
	Range("A1:C3, F5:G10")
	Range("E1", "E3").Value = "E"
	Range("F1,F3,F5").Value = "F"
	Range(Cells(a, b), Cells(c, d))

■クリア(Clear、ClearContents)
	Range("A1").Clear '値・書式設定・罫線、コメント全てクリア。未使用状態となる。
	Range("A1").ClearContents 'セルの数式と文字だけが消去。書式（フォント、背景色、罫線等）は残る。

■削除
	Cells(1, 1).Delete                'A1セルを削除
	Cells(1, 1).Delete(xlShiftToLeft) 'A1セルを削除し左方向にシフト
	Cells(1, 1).Delete(xlShiftUp)     'A1セルを削除し上方向にシフト

■セルの書式
With Range("A1") ' Cells()でも可能
	.Font.Size = 11                     'サイズを11にする
	.Font.ColorIndex = xlAutomatic      '文字色を自動に
	.Font.ColorIndex = 2                '文字色変更（インデックス表記）
	.Font.Color = RGB(0, 0, 0)          '文字色変更（RGB表記）
	.Interior.ColorIndex = xlNone       '背景色をなしに
	.Interior.ColorIndex = 2            '背景色変更（インデックス表記）
	.Interior.ColorIndex = RGB(0, 0, 0) '背景色変更（RGB表記）
End With

■最終端を取得
'連続したデータが入っている範囲の最終端を取得
	n = Range("A1").End(xlDown).Row       '縦方向
	n = Range("A1").End(xlToRight).Column '横方向
'最後のセルから最終端を取得
	n = Cells(Rows.Count, 1).End(xlUp).row           '縦方向
	n = Cells(1, Columns.Count).End(xlToLeft).Column '横方向
'シートで使われているセルの最終端を取得
	n = ActiveSheet.UsedRange.Columns.count '最終行
	n = ActiveSheet.UsedRange.Rows.count   '最終列
'値が入っている最終行・最終列の取得
With ActiveSheet.UsedRange
	MaxRow = .Find("*", , xlFormulas, , xlByRows, xlPrevious).Row
	MaxCol = .Find("*", , xlFormulas, , xlByColumns, xlPrevious).Column
End With

■変数を含んだ範囲指定
	Range(Cells(a, b), Cells(c, d)).Select

■選択範囲
	Selection.PasteSpecial xlPasteValues '選択範囲を値だけ貼り付け
	Range("A1").Select    ' A1を選択する
	Range("A1:B2").Select ' A1～B2を選択する
	Rows(1).Select        ' 1行目を選択する
	Rows("1:5").Select    ' 1～5行目を選択する
	Columns("A").Select   ' A列を選択する
	Columns(1).Select     ' これも同じ
	Columns("A:E").Select ' A～E列を選択する
	Rows(1).Value = 100   ' 1行目の全セルの値に100を設定する

■選択されてる範囲の一部を取得
	n = Selection.Cells(1).Row                  '最初のセルの行
	n = Selection.Cells(Selection.Count).Row    '最後のセルの行
	n = Selection.Cells(1).Column               '最初のセルの列
	n = Selection.Cells(Selection.Count).Column '最後のセルの列

■罫線の操作
'基本形
	Range("A1:E5").Borders.LineStyle = xlContinuous '実線をひく
 
'線の種類
With Range("A1:E5").Borders
	.LineStyle = xlContinuous '実線(既定値)
	.LineStyle = xlDash       '破線
	.LineStyle = xlDot        '点線
	.LineStyle = xlDouble     '二重線
	.LineStyle = xlNone       '削除
End With
 
'線の太さ
With Range("A1:E5").Borders
	.Weight = xlThin   '細(既定値)
	.Weight = xlMedium '中
	.Weight = xlThick  '太
End With
 
'線の色
With Range("A1:E5").Borders
	.ColorIndex = xlAutomatic '自動(既定値)
	.ColorIndex = 3 '赤
	.ColorIndex = 5 '青
End With
 
'線の細かい位置
With Range("A1:E5")
	.Borders.LineStyle                     = xlContinuous '枠と格子全部に適用
	.Borders(xlEdgeTop).LineStyle          = xlContinuous '上辺
	.Borders(xlEdgeRight).LineStyle        = xlContinuous '右辺
	.Borders(xlEdgeBottom).LineStyle       = xlContinuous '下辺
	.Borders(xlEdgeLeft).LineStyle         = xlContinuous '左辺
	.Borders(xlInsideHorizontal).LineStyle = xlContinuous '中横線
	.Borders(xlInsideVertical).LineStyle   = xlContinuous '中縦線
	.Borders(xlDiagonalUp).LineStyle       = xlContinuous '右上がり斜線
	.Borders(xlDiagonalDown).LineStyle     = xlContinuous '右下がり斜線
End With



■並び替え(Sort)
'A1:E100範囲をC1を基準に昇順に並び替え
	Range("A1:E100").Sort Key1:=Range("C1"), order1:=xlAscending '降順はxlDescending
 
'3つまで優先キーを設定できる
Range("A1:E100").Sort _
	Key1:=Range("C1"), order1:=xlAscending, _
	Key2:=Range("B1"), order2:=xlDescending, _
	Key3:=Range("D1"), order3:=xlAscending

■コピペ
	Range("A1").Copy         'コピー
	Range("A1").PasteSpecial 'ペースト
	Range("A1").PasteSpecial Paste:=xlPasteValues  '値だけペースト
	Range("A1").PasteSpecial Paste:=xlPasteFormats '書式だけペースト
	Range("A1").AutoFill Destination:=Range("A1:A5") 'オートフィル
	Application.CutCopyMode = False 'コピーモード解除

'セルの値コピー
	Cells(1,1).Copy Destination:=Cells(1,2)
'複数セルの値コピー
	Range("C1:C30").Value = Range("D1:D30").Value ' 高速

■Rangeをまわす
	For Each c In Range("A1:A10000")
		c.Value = 100
	Next c

■使用しているセル範囲
	Dim rng As Range
	Set rng = ActiveSheet.UsedRange
	rng.Select	

■範囲からオフセットした範囲を取得する(Offset)
'アクティブセルを動かす
	ActiveCell.Offset(動かす行数, 動かす列数).Activate
	ActiveCell.Offset(1, 2).Activate
'選択範囲を移動する
	Selection.Offset(動かす行数, 動かす列数).Select
	Selection.Offset(2, 3).Select
'Rangeで指定した範囲をOffsetで変更する方法
	Range("A1:B2").Offset(2, 3).Select
	Range.Resizeで指定した範囲をOffsetする方法
'ある単体セルから範囲に変更して、それをオフセットする場合に使う。
	Range("A1").Resize(6, 2).Offset(1, 5).Select
'ValueとOffsetで相対位置に値を入力する方法
	Rangeオブジェクト.Offset(行数, 列数).Value = 入力する値
	Set rng = Range("A1:B8")
	rng.Offset(0, 3).Value = rng.Value

'セル(範囲)の移動
	Rangeオブジェクト.Offset(行方向, 列方向）
	ActiveCell.Offset(1, 0).Select
	Selection.Offset(0, 2).Select 
	Range("A1:D5").Offset(0, 2).Select
'アクティブセルから右方向へ5列移動したセルまでのセル範囲を結合する(Merge)
	Range(ActiveCell, ActiveCell.Offset(0, 5)).Merge
'選択範囲を1行下まで拡張します。
	Range(Selection, Selection.Offset(1, 0)).Select

■範囲を変更する(Resize)
リサイズ（セル範囲のサイズ変更）を行い、サイズ変更した後のセル範囲を返す。
Rangeオブジェクト.Resize（変更後の行数, 変更後の列数）
選択範囲を行・列ともに1ずつ拡張する
	Selection.Resize(Selection.Rows.Count + 1, Selection.Columns.Count + 1).Select
	Selection.Resize(Selection.Rows.Count + 1).Select       '行だけを拡張
	Selection.Resize(, Selection.Columns.Count + 1).Select  '列だけを拡張
わかりやすいコードにする
	Dim cntRow As Long
	Dim cntCol As Long
	cntRow = Selection.Rows.Count + 1
	cntCol = Selection.Columns.Count + 1
	Selection.Resize(cntRow, cntCol).Select
入力範囲（A列－E列）をリサイズしてE列を除いたデータをコピー
	Sub Sample1()
		Dim cntRow As Long
		Dim cntCol As Long
		Dim Rng As Range
		Set Rng = ActiveSheet.Range("A1").CurrentRegion
		cntRow = Rng.Rows.Count
		cntCol = Rng.Columns.Count - 1
		Rng.Resize(cntRow, cntCol).Copy Destination:=Sheets("Sheet2").Range("A1")
	End Sub

'よく用いられるテクニック
1行目が項目行である場合に、Csvデータのデータ範囲のみ取得する(=ヘッダーを削る)
データ範囲をCurrentRegionで取得し、そのあとにOffsetで対象セル範囲を1行下にずらし、Resizeで1行削る
	Sub Sample3()
		Dim cntRow As Long
		Dim cntCol As Long
		Dim Rng As Range
		Set Rng = ActiveSheet.Range("A1").CurrentRegion
		cntRow = Rng.Rows.Count - 1
		cntCol = Rng.Columns.Count
		Set Rng = Rng.Offset(1, 0)              '※1行下にずらす
		Set Rng = Rng.Resize(cntRow, cntCol)    '※1行削る
		Rng.Copy Destination:=Sheets("Sheet2").Range("A2")
	End Sub

■ActiveCellとは？

■Selected？

■Activate？

■Rangeアドレス名
	rng.Address(False, False) ' セルアドレス名
	rng.Parent.Name           ' シート名

■CurrentRegionでざっくり範囲を持ってくる
	Dim rng As Range
	Set rng = ActiveSheet.Range("A1").CurrentRegion
	Debug.Print rng.Address(False, False)

■■シェイプ
'// https://excel-ubara.com/EXCEL/EXCEL917.html
■全回し
Dim e As Variant
For Each e In ActiveSheet.Shapes
	Debug.Print e.Name & e.Top & e.Left & e.Height & e.Width
Next

■テキスト操作
ActiveSheet.Shapes(1).TextFrame.Characters.Text = "サンプル文字列"

■色関係
'シェイプを新規作成と同時にパラメータを設定する。
With ActiveSheet.Shapes.AddShape(msoShapeRectangle, 120, 50, 100, 70) ' 矩形を作図
	.TextFrame.Characters.Text       = "文字！"           '表示文字の指定
	.TextFrame.Characters.Font.Color = RGB(255, 255, 255) '図形内テキストのフォントカラーを指定
	.TextFrame.HorizontalAlignment   = xlHAlignCenter     '図形内のテキスト水平方向を中央位置に
	.TextFrame.VerticalAlignment     = xlVAlignCenter     '図形内のテキスト縦方向を中央位置に
	.Line.ForeColor.RGB              = RGB(255, 0, 0)     '図形の枠線の色を指定
	.Fill.ForeColor.RGB              = RGB(255, 0, 0)     '図形の塗りつぶし色を指定
End With

■シェイプの種類を書き換える
	shape.AutoShapeType = msoShape32pointStar 

■削除
'全削除。Shapes レベル。
'画像のみならず図形は、とにかくなんでも消したい。
Private Sub IMAGE_DELETE_allShapes()
	Dim MyOb As Object
	Dim i1, i2, i3 As Long
	i1 = ActiveSheet.Shapes.Count
	i2 = ActiveSheet.DrawingObjects.Count
	i3 = ActiveSheet.Pictures.Count
	For Each MyOb In ActiveSheet.Shapes
		MyOb.Delete
	Next
	MsgBox ( _
		"シート内のオートシェイプ の数:" & i1 & "⇒" & ActiveSheet.Shapes.Count & vbCrLf & _('Drop Down
		"シート内のDrawingObjectの数:"   & i2 & "⇒" & ActiveSheet.DrawingObjects.Count & vbCrLf & _'←テキストボックスやフキダシ
		"シート内のPicture の数:"        & i3 & "⇒" & ActiveSheet.Pictures.Count _'←画像
	)
End Sub

■■文字列操作
■連結
str = "サンプルテキスト" & smp_txt & "sampletext" '変数が混ざっても大丈夫

■数値を文字列に変換
str = CStr(n) '変数nは数値であること

■総文字数を取得
n = Len(対象文字列)

■文字の抜き出し
str = Left(対象文字列, n)   '対象文字列の左からn文字抜き出す
str = Right(対象文字列, n)  '対象文字列の右からn文字抜き出す
str = Mid(対象文字列, n, i) '対象文字列の左からn文字目からi文字抜き出す

■置換
str = Replace(対象文字列, 置換前文字, 置換後文字)
'例
str = Replace(str, " ", "")
str = Replace(str, "　", "")

■変換
str = StrConv(対象文字列, vbNarrow) '半角へ
str = StrConv(対象文字列, vbWide) '全角へ
str = StrConv(対象文字列, vbLowerCase) '小文字へ
str = StrConv(対象文字列, vbUpperCase) '大文字へ
str = StrConv(対象文字列, vbKatakana) 'カタカナへ
str = StrConv(対象文字列, vbHiragana) 'ひらがなへ

■一致判定(文字列に含まれるのか)
if InStr(String1, String2) <> 0 Then
  MsgBox "match between two Strings"
Else
  MsgBox "mismatch between two Strings"
End If
'見つかればその最初の文字数を返し、見つからなければ0を返す

■結合
Dim cnt As Integer
For cnt = 1 To 100 Step 1
  Range("A" & cnt) = cnt
  Range("B" + cnt) = cnt
Next

■制御文字
・代表的な制御文字　一覧
値                  定数        内容
Chr(0)              vbNullChar  値0を持つ文字
Chr(9)              vbTab       タブ文字
Chr(8)              vbBack      バックスペース文字
Chr(10)             vbLf        ラインフィード文字
Chr(13)             vbCr        キャリッジリターン文字
Chr(13) + Chr(10)   vbCrLf      キャリッジリターンとラインフィードの組合せ

■キャスト
''StoI
	Dim s1 As String: s1 = CStr(100)
	Debug.Print s1 + s1
''ItoS
	Dim d1 As Double: d1 = CStr("100.2")
	Debug.Print d1 + d1

	CDbl 	倍精度浮動小数点実数型(Double)へのデータ変換
	CInt 	整数型(Integer)へのデータ変換
	CLng 	長整数型(Long)へのデータ変換
	CSng 	単精度浮動小数点実数型(Single)へのデータ変換
'文字or数字チェック
Sub Sample()
	Dim a As Variant
	a = "aa"
	If IsNumeric(a) Then
		MsgBox a & "Num"
	Else
		MsgBox a & "Str"
	End If
End Sub

■正規表現
'RegExpオブジェクト
'・プロパティ
	Pattern     正規表現で使用するパターンを設定(.*や[A-Z]、(田中|鈴木)などの文字列)
	IgnoreCase  大文字小文字を区別しないときにTrueを設定します。デフォルトはFalse
	Global      検索対象の文字列内で、文字列全体を検索するにはTrueを設定します。デフォルトではFalse
'・メソッド
	Test     正規表現によるマッチングを行います。パターンと一致した場合はTrueを返します。
	Replace  検索文字列の中で正規表現によるマッチングを行い、成功するとそのマッチした部分を置換文字列と置き換えます。
	Execute  マッチングの結果をMatchesコレクションとして返します。MatchesコレクションはMatchオブジェクトの集合。

'Matchesコレクション
	Executeメソッドによって返されるコレクション。
	Matchesコレクションには0個以上のMatchオブジェクトが含まれる。
	(Executeメソッドの結果返ってきたMatchesコレクションにMatchオブジェクトが存在しなかったら、マッチングは失敗したということ)

'Matchesコレクションのプロパティとメソッド
'・プロパティ
	Count  Matchesコレクション内でMatchオブジェクトの個数を返します。
	       これを調べるとマッチングの成功/失敗がわかります。
	Item   いわゆる一般的なコレクションのItemプロパティです。
	       普通はMatches(1)のようにアクセスするので使うことはないでしょう。

'Matchオブジェクトのプロパティとメソッド
'・プロパティ
	FirstIndex  最初にマッチングに成功した位置を返します。文字列の先頭は0から始まります。
	Length      マッチした文字列の長さを返します。
	Value       マッチした文字列を返します。

'主なメタ文字とエスケープシーケンス
	^    文字列の先頭
	$    文字列の末尾
	\n   改行
	.    改行を除く任意の1文字
	*    直前のパターンの0回以上の繰り返し
	+    直前のパターンの1回以上の繰り返し
	?    直前のパターンが0回または1回現れる
	\d   任意の数値
	\D   任意の数値以外の文字
	\s   任意のスペース文字
	\S   任意のスペース以外の文字
	()   パターンのグループ化
	|    パターンの論理和
	[]   キャラクタクラス

Sub Sample1()
	Dim RE, strPattern As String, r As Range
	Set RE = CreateObject("VBScript.RegExp")
	strPattern = "SUM\("
	With RE
		.Pattern = strPattern       ''検索パターンを設定
		.IgnoreCase = True          ''大文字と小文字を区別しない
		.Global = True              ''文字列全体を検索
		For Each r In ActiveSheet.UsedRange
			If .Test(r.Formula) Then r.Interior.ColorIndex = 3
		Next r
	End With
	Set RE = Nothing
End Sub

'！無理に正規表現オブジェクトを使わなくても、Like演算子で済むなら、その方が扱いやすい。
'！StrComp関数も使いやすい

■Like演算子
	?            任意の1文字                                  たな?   たなか、たなべ、たなし(など)
	*            0個以上の任意の文字                          たか*   たかだ、たかなか、たかやなぎ(など)
	#            1文字の数値(0～9)                            ##      01、26、95(など)
	[charlist]   charlistに指定した文字の中の1文字            [A-F]   A、B、C、D、E、F
	[!charlist]  charlistに指定した文字の中に含まれない1文字  [!A-F]  G 、H、I(など)

Sub Sample1()
	Dim i As Long
	For i = 1 To 8
		If Cells(i, 1).Value Like "[!東京,横浜,千葉]*" Then Cells(i, 1).Font.ColorIndex = 3
		If Cells(i, 2).Value Like "[タ-ト]*"           Then Cells(i, 2).Font.ColorIndex = 2
	Next i
End Sub

■■文法
■配列
'定義
Dim arr1(4)       As String '0, 1, 2, 3
Dim arr2(5, 3)    As String
Dim arr3(5 To 7)  As String '5, 6, 7
Dim arr4()        As Variant ' 可変型はVariantにしないとだめ
arr4 = Array("One", "Two", "Three", "Four")
arr(1) = "Once"

'メモリ開放
Erase(arr1)

'再定義
Redim arr1(5) As String

'配列の要素数
Dim arr1(2 To 5) As Long
i = UBound(arr1) '2
j = LBound(arr1) '5
k = Ubound(arr1) - LBound(arr2) + 1 ' (5 - 2 + 1 = 4)要素数

Dim arr2(1 To 3, 3 To 5, 5 To 11) As Long
Debug.Print LBound(arr2, 1) '1
Debug.Print LBound(arr2, 2) '3
Debug.Print LBound(arr2, 3) '5

'配列のForループ(UBound)
For n = LBound(arr1) To UBound(arr1)
	Debug.Print arr1(n)
Next n
'配列のForEachループ
For Each e In arr1
	Debug.Print e
Next e

'配列コピー
Dim arr5 As Variant '配列コピー元はVariant型
arr5 = arr4

'Rangeを利用したソート(Sort)
	arr = Array(1, 3, 5, 4, 2)
	'配列の要素をシートのセルにセット
	Dim i As Long
	For i = LBound(arr) To UBound(arr)
	    Cells(i + 1, 1).Value = arr(i)
	Next i

	'RangeオブジェクトのSortメソッドを使用
	Dim myrange As Range
	Set myrange = Range( Cells(LBound(arr) + 1, 1), Cells(UBound(arr) + 1, 1))
	myrange.Sort key1:=Cells(LBound(arr) + 1, 1), Order1:=xlDescending

	'Sort後の結果をシートのセルから取得表示
	For i = LBound(arr) To UBound(arr)
	    arr(i) = Cells(i + 1, 1).Value
	Next i

	Dim e As Variant '結果出力
	For Each e In arr
		Debug.Print(e)
	Next arr

'検索(Filter)
	Dim arr1() As Variant, arr2 As Variant
	arr1 = Array("東京都", "大阪府", "愛知県", "神奈川県", "広島県")
	arr2 = Filter(arr1, "県", True)
	Dim e As Variant
	For Each e In arr2
	    Debug.Print e
	Next e

'結合(Join)
	Dim arr1() As Variant
	arr1 = Array("東京都", "大阪府", "愛知県", "神奈川県", "広島県")
	Dim msg As String
	msg = Join(arr1, ",")
	Debug.Print msg

'分割(Split)
	Dim arr() As String
	arr = Split("東京 大阪 名古屋", " ")
	Dim e As Variant
	For Each e In arr
		Debug.Print e
	Next e

'配列の関数利用
	Function MethodSample(arr As String) As String()
		Dim ret() As String
		'...
		MethodSample = ret
	End Function


■値渡しByVal、参照渡しByRef
Sub Test
	Dim a As Integer
	a = 1
	Debug.Print "元値 a=" & a
	byVal = MyByVal(a)
	Debug.Print "値渡し後の a=" & a
	byRef = MyFunction(a)
	Debug.Print "参照渡し後の a=" & a
End Sub

Function MyByVal(ByVal arg As Integer) As Integer
	arg = arg + 1  ''←ByValでは、Subプロシージャ内のaは書き換えられない
	MyByVal = a
End Function

Function MyByRef(ByRef arg As Integer) As Integer
	arg = arg + 1  ''←ByRefでは、Subプロシージャ内のaも書き換えられる
	MyByRef = a
End Function

■数値の四捨五入・切り上げ・切り捨て
	WorksheetFunction.Round(数値, 小数点以下桁数)     '' 四捨五入
	WorksheetFunction.RoundUp(数値, 小数点以下桁数)   '' 切り上げ
	WorksheetFunction.RoundDown(数値, 少数点以下桁数) '' 切り捨て
	Int(-5.5)    ''VBA 切り捨て関数。負の値の結果が違う。この結果は-6
	Fix(-5.5)    ''VBA 切り捨て関数。負の値の結果が違う。この結果は-5

■Select...Case文
例1
	Select Case value
	  Case 1
	      myColor = 3 ''赤
	  Case 2
	      myColor = 6 ''黄
	  Case 3,4
	      myColor = 8 ''水色
	  Case Else
	      myColor = 10 ''緑
	End Select

例2
	Select Case value
	  Case Is < 10
	      myColor = 3 ''赤
	  Case Is < 20
	      myColor = 6 ''黄
	  Case Is < 30
	      myColor = 8 ''水色
	  Case Else
	      myColor = 10 ''緑
	End Select

例3
	Select Case value
	  Case 1 To 9
	      myColor = 3 ''赤
	  Case 10 To 19
	      myColor = 6 ''黄
	  Case 20 To 29
	      myColor = 8 ''水色
	  Case Else
	      myColor = 10 ''緑
	End Select

■繰り返し(While、Do)
'いずれも１から１０までの和
Sub sum1()
	Dim Sum As Integer
	Dim i As Long

	Sum = 0: i = 1 'While ~ Wend
	While i <= 10
			Sum = Sum + i: i = i + 1
	Wend

	Sum = 0: i = 1 'Do Until ~ Loop
	Do Until i > 10
			Sum = Sum + i: i = i + 1
	Loop

	Sum = 0: i = 1
	Do             'Do Loop ~ Until
		Sum = Sum + i: i = i + 1
	Loop Until i > 10

	Sum = 0: i = 1
	Do While i <= 10 'Do While ~ Loop
		Sum = Sum + i: i = i + 1
	Loop

	Sum = 0: i = 1
	Do               'Do ~ Loop While
		Sum = Sum + i: i = i + 1
	Loop While i <= 10
End Sub

■DictionaryとCollection
・共通点
	ハッシュ(連想配列)として使う。
1. Dictionary
	○：処理速度と便利なメソッドがある点においてDictionaryの方が上
	×：Set obj = CreateObject("Scripting.Dictionary") するので、
	    参照設定しないと入力予測がでない。
	×：重複を許さない
2. Collection
	○：値の型にバラツキがあっても問題ない→String,配列,Intが混ざっててもOK
	×：数が多いと遅くなる
	○：順番保障をしている
	△：重複を許す

■Continue(許されるGotoの使い方)
For Each e in items
	If e = 10 Then
		GoTo CONTINUE:
	End If
	Debug.Print e
CONTINUE:

■乱数
'1～6の乱数をダイアログに表示する
Sub RANDAM()
	Dim i As Integer
	Dim StrArray(4) As String
	Randomize
	For i = 0 To 4
		StrArray(i) = Int(6 * Rnd + 1)
	Next i
	MsgBox Join(StrArray, vbCrLf)
End Sub


■■日付型
'書式
Dim d1 As Date, d2 As Date, d3 As Date
d1 = #11/1/2017#            ' #月/日/年#           '2017/11/01
d2 = #9:10:30 AM#           ' #時/分/秒#           '9:01:30
d3 = #11/1/2017 9:10:30 AM# ' #月/日/年 時/分/秒#  '2017/11/01 9:10:30

'文字列型から日付型への自動変換
Dim d1 As Date, d2 As Date, d3 As Date
d1 = "11/1/2017"         '2017/11/01
d2 = "9:10:30"           '9:01:30
d3 = "11/1/2017 9:10:30" '2017/11/01 9:10:30

'現在時刻
Range("A1") = Date '2018/1/1
Range("A1") = Now  '2018/1/1 0:00:00
Range("A1") = Time '0:00:00

'年、月、日、時、分、秒、曜日を取得
Dim d As Date : d = "11/1/2017 9:10:30"
Year(d) & "_" & Month(d) & "_" & Day(d) & "_" & Hour(d) & "_" & Minute(d) & "_" & Second(d) & "_" & Weekday(d)
'2017_11_1_9_10_30_4

'日付を比較
Dim d1 As Date, d2 As Date, d3 As Date
d1 = Now
d2 = "11/1/2017"
d3 = "11/1/2017 23:59:59"
If d1 >= d2 And d1 <= d3 Then
	MsgBox d1 & vbCrLf & "期限範囲内です"
Else
	MsgBox d1 & vbCrLf & "期限範囲外です"
End If

'日付の計算
	d = DateAdd(設定値, 計算数, 対象) 
	'設定値：年→"yyyy", 月→"m", 日→"d", 週→"ww", 時→"h", 分→"n", 秒→"s"
	d = DateAdd("d", 1, Date) '1日プラス
	t = DateAdd("h", -1, Time) '1時間マイナス

'日付の間隔を取得
	DateDiff (interval, date1, date2 [, firstdayofweek] [, firstweekofyear]
	interval : 計算する時間間隔を書式で指定。
	           (intervalを指定する書式はDiffAdd関数と同じ)
	date1、date2 : 計算の対象となる2つの日付の値。
	firstdayofweek : 週の始まりの曜日を定数で指定します。(規定値は日曜日)
	firstweekofyear : 年の第1週を定数で指定します。(規定値は1月1日を含む週を第1週として指定)

	firstdayofweekの設定
		定数        値  説明
		vbSunday     1  日曜日(デフォルト値)
		vbMonday     2  月曜日
		vbTuesday    3  火曜日
		vbWednesday  4  水曜日
		vbThursday   5  木曜日
		vbFriday     6  金曜日
		vbSaturday   7  土曜日

	firstweekofyearの設定
		定数            値	説明
		vbFirstJan1      1  1月1日が含まれる週から開始(デフォルト値)
		vbFirstFourDays  2  4日以上が含まれる最初の週が年の第 1 週目
		vbFirstFullWeek  3  1週間全体が含まれる最初の週がその年の第 1 週目


	Dim d1 As Date, d2 As Date
	d1 = "11/1/2017"
	d2 = "4/1/2018"
	Debug.Print DateDiff("yyyy", d1, d2) & "年" '1年
	Debug.Print DateDiff("m", d1, d2) & "ヶ月"  '5か月
	Debug.Print DateDiff("d", d2, d1) & "日"    '-155日

'時間の初めからの経過時間を取得
	'年の初めや月の初めからの経過日数や日の初めからの経過時間など、時間の初めからの経過時間を取得する
	DatePart ( interval, date [, firstdayofweek] [, firstweekofyear] )

	Dim d As Date
	d = "11/1/2017"
	Debug.Print "今年も今週で" & DatePart("ww", d) & "週目です"
	Debug.Print "今年も今日で" & DatePart("y",  d) & "日目です"

'日付の妥当性をチェック判定する方法
	'有効な日付かチェック
	Debug.Print IsDate("11/1/2017") ' True


■日付のフォーマット変更
str = Format(対象物(Dateなどの日付), "yyyy/mm/dd")
PCの設定によってDateで取得した日付のフォーマットがバラバラだったりするので…


Dim str As String
str = "17年9月1日 2時2分2秒"

Debug.Print Format(str, "Long Date")                 ' 2017年9月1日
Debug.Print Format(Now, "Short Time")                ' 02:53
Debug.Print Format(str, "c")                         ' 2017/09/01 2:02:02
Debug.Print Format(str, "yyyy/mm/dd hh:nn:ss AM/PM") ' 2017/09/01 2:02:02 AM
Debug.Print Format("17年9月1日", "ggge年mm月dd日")   ' 平成29年09月01日
Debug.Print Format("平成29年9月1日", "yy/mm/dd")     ' 17/09/01
Debug.Print Format("17年9月1日 2時2分2秒", "ddd")    ' Fri
Debug.Print Format("17年9月1日 2時2分2秒", "aaaa")   ' 金曜日






・ユーザー定義文字列書式
	文字 |説明
	-----+--------------------------------------------------------------------------------------------
	@    |任意の1文字。存在しないとスペースを返す
	&    |任意の1文字。存在しないときスペースを返さない
	<    |小文字に変換
	>    |大文字に変換
	!    |文字を左から右に埋める
		Dim str As String
		str = "123"
		Debug.Print "[" & Format(str, "@、@、@") & "]"         '[1、2、3]
		Debug.Print "[" & Format(str, "@@") & "]"              '[123]
		Debug.Print "[" & Format(str, "@@@") & "]"             '[123]
		Debug.Print "[" & Format(str, "@@@@") & "]"            '[ 123]
		Debug.Print "[" & Format(str, "@@@@@") & "です" & "]"  '[  123です]
		Debug.Print "[" & Format(str, "!@@@@@") & "です" & "]" '[123  です]

		Debug.Print "[" & Format(str, "&&") & "]"              '[123] 
		Debug.Print "[" & Format(str, "&&&") & "]"             '[123]
		Debug.Print "[" & Format(str, "&&&&") & "]"            '[123]
		Debug.Print "[" & Format(str, "&&&&&") & "]"           '[123]
		Debug.Print "[" & Format("ABC", "<&&&&&") & "]"        '[abc]
		Debug.Print "[" & Format("abc", ">&&&&&") & "]"        '[ABC]

・ユーザー定義数値書式
	文字 |説明
	-----+--------------------------------------------------------------------------------------------
	0    |数値の1桁を表す。指定桁位置に値がない場合、0が入る。
	#    |数値の1桁を表す。指定桁位置に値がない場合、その桁には何も入らない。
	     | 桁が指定した「#」より多い場合、すべて表示される。	
	%    |数値を100倍にして「%」が付きます。
		Format(123,"0000")     '0123
		Format(123,"00")       '123
		Format(123,"####")     '123
		Format(123,"##")       '123
		Format(0.123,"0.0000") '0.1230
		Format(0.456,"0.00")   '0.46
		Format(0.456,"#.##")   '46
		Format(0.4567,"0.#%")  '45.7%

■■エラー処理
■エラー処理の定義
''デフォルトではOn Error Goto 0 が有効。
	On Error Resume Next ' エラーが発生しても無視して処理を続ける
	On Error Goto Hoge   ' エラーが発生すると、指定したラベルに処理を移す
	On Error Goto 0      ' エラー処理を無効にすうｒ。エラーが発生した行で処理を中断します。

1.On Error Resume Nextの例
	On Error Resume Next
	Dim i As Integer
	i = "文字"        ' ← 1.「型の不一致」でエラー発生
	MsgBox "処理終了" ' ← 2.次の行の処理を続ける
	On Error Goto 0 ' これ以降はエラー時に中断する

2.On Error Goto ラベルの例
	On Error Goto ErrorProc
	Dim i As Integer
	i = "文字"        ' ← 1.「型の不一致」でエラー発生
	MsgBox "処理終了"
ErrorProc:
	MsgBox "エラー発生" ' ← 2.「ラベル(ErrorProc)」に処理が移る。

3.On Error Goto 0の例
	On Error Goto 0
	Dim i As Integer
	i = "文字"        ' ← 1.「型の不一致」でエラー発生。ここで処理終了。
	MsgBox "処理終了"
	On Error Goto 0の場合、エラーが発生するとその行で処理が中断します。

■エラーの内容を知る
1.ErrオブジェクトとOn Error Goto ラベルの例
	On Error Goto ErrorProc
	Dim i As Integer
	i = "文字" ' 型の不一致でエラー発生
	MsgBox "処理終了"

	ErrorProc:
	MsgBox "エラー番号:" & Err.Number
	MsgBox "エラー内容:" & Err.Description

2.ErrオブジェクトとOn Error Resume Nextの例
	On Error Resume Next
	Dim i As Integer
	i = "文字" ' 型の不一致でエラー発生
	If Err.Number <> 0 Then
	  Msgbox "エラー発生"
	End If

■On Errorの使い分け方
・On Error Goto ラベル を使用するのが一般的。
・エラー発生時に処理を中断したくない場合は、On Error Resume Next
・アプリケーションでエラー箇所が特定できない場合やデバッグ時は
  On Error Goto 0 で処理を中断させて、調査を行います。
  処理を中断してしまうため、デバッグ時以外は利用しないのが一般的

■エラー情報無効化
Err.Clear


■■簡易ダイアログ
■ メッセージ入力 : InputBox
Dim message As String
message = InputBox("メッセージを入力してください")

■ メッセージ出力 : MsgBox
'メッセージのみ出力
MsgBox "Output Message"

'Yes/Noの選択
If MsgBox("Output Messag", vbYesNo) = vbYes Then
    ' Yesの処理
Else
    ' Noの処理
End If

'OK/Cancelの選択
If MsgBox("Output Messag", vbOKCancel) = vbOK Then
    ' OKの処理
End If

n = MsgBox("サンプルテキスト", vbOKCancel)    '戻り値(n) ：OK→1, キャンセル→2
n = MsgBox("サンプルテキスト", vbYesNoCancel) '戻り値(n)：はい→6, いいえ→7, キャンセル→2
n = MsgBox("サンプルテキスト", vbYesNo)       '戻り値(n)：はい→6, いいえ→7
 
'例
If MsgBox("○○です。続けますか？", vbOKCancel) <> 1 Then End
'↑yes以外が選択されたときは処理を終了します

'ボタン
定数 	ボタン
vbOkOnly 	OK のみ
vbOKCancel 	OK, キャンセル
vbAbortRetryIgnore 	中止、再試行、無視
vbYesNo 	はい、いいえ
vbRetryCancel 	再試行、キャンセル

'リターン
値 	対応する定数 	押されたボタン
1 	vbOK 	OK
2 	vbCancel 	キャンセル
3 	vbAbort 	中止
4 	vbRetry 	再試行
5 	vbIgnore 	無視
6 	vbYes 	はい
7 	vbNo 	いいえ


■FileDialogを出す
CreateObject("WScript.Shell").CurrentDirectory = 任意のパス '開くフォルダを指定
str = Application.GetOpenFilename("ﾌｧｲﾙ,*.*")
'strには選択されたファイルのフルパス、キャンセル時にはFalseが返る
'strをStringで宣言しているときには
If str = "False" Then Exit Sub
'のように""で括ってエラー処理をする（Variantで宣言したほうが楽かも）

■FileDialogからファイル名を取得
Function FileDialogからファイル名を取得()
    Dim fname As Variant
    fname = Application.GetSaveAsFilename( _
            InitialFileName:="test.txt", _
            filefilter:="テキストファイル,*.txt", _
            Title:="ファイル保存")
    If VarType(fname) = vbBoolean Then
        MsgBox "キャンセルされました。"
        printDialog = ""
    Else
        printDialog = fname
    End If
End Function


■■ちょっとしたスニペット

■フォルダ・ファイルの検索
str = Dir(対象物, 属性) '属性：0→ファイル,16→フォルダ
'存在する場合はその名前を、存在しなければ""を返す
 
'例
If Dir(fol, 16) = "" Then 'フォルダが存在しなければ
  If MsgBox("該当フォルダが存在しません。作成しますか？", vbOKCancel) = vbOK Then
    MkDir fol 'フォルダ作成（変数folはフルパスであること）
  End If
End If

■シート内検索
Dim fnd As Range, str As String
Dim row1 As Integer, col1 As Integer
 
str = "sample" '検索文字列
 
Set fnd = Range("A1:C100").Find(str) '検索
If Not fnd Is Nothing Then '見つかったとき
  fnd.Font.ColorIndex = 3 '該当セルのフォントを赤に
  row1 = fnd.Row '該当セルの行取得
  col1 = fnd.Column '該当セルの列取得
End If


■高速化
Application.ScreenUpdating = False
'処理
Application.ScreenUpdating = True ' これ不要な疑い。
進捗も合わせて使うといい

■関数の書き方
Sub sample() 'サブルーチン
  Dim beforeValue As 処理前の型, afterValue As 処理後の型
 
  afterValue = getFnc(beforeValue) 'beforeValueを関数へ引渡して返り値をafterValueへ
End Sub
	
Function getFnc(ByVal before_value As 処理前の型) As 処理後の型 '関数
  'before_valueを使っていろいろ処理する
  getFnc = 処理結果 'この値がサブルーチンのafterValueに入る
End Function

■列のアルファベットを数値へ、数値をアルファベットへ変換
Sub Range型の列名変換()
  Dim va As Variant, c_va As Variant
  va = "AA" '列のアルファベットor数値
  c_va = CNumAlp(va) 'vaを関数へ引渡してc_vaへ変換後の値を格納
  MsgBox "変換後の値は" & c_va & "です"
End Sub

Function CNumAlp(va As Variant) As Variant '変換する関数
  Dim al As String
  If IsNumeric(va) = True Then '数値だったら
    al = Cells(1, va).Address(RowAbsolute:=False, ColumnAbsolute:=False) '$無しでAddress取得
    CNumAlp = Left(al, Len(al) - 1)
  Else 'アルファベットだったら
    CNumAlp = Range(va & "1").Column '列番号を取得
  End If
End Function

■進捗をステータスバーに描画
Sub ステータスバー進捗表示()
    Dim i As Long
    For i = 1 To 20000
        If i Mod 1000 = 0 Then
            Range("A1") = i
            Application.StatusBar = "処理中..." & String(Int(i / 1000), "■")
        End If
    Next i
    Application.StatusBar = False ' 使ったら元に戻す！
End Sub

■タイトルバー制御
Application.Caption = "タイトル"
Application.Caption = vbNullString ' 使ったら元に戻す！


■テキスト入出力
Private Sub OutputTextList(datFile As String, doc As Collection)
    Open datFile For Output As #1
    Dim e As Variant
    For Each e In doc
        Print #1, e
    Next e
    Close #1
End Sub

Private Function InputText(path As String) As Collection
    Dim ret As Collection
    Set ret = New Collection
    Dim buf As String

    Open path For Input As #1 ' Input：入力
    Do Until EOF(1)
        Line Input #1, buf
        ret.Add buf
    Loop
    Close #1
    Set InputText = ret
End Function

■処理を一時的に渡す(DoEvents) 
'クリックやキー入力や描画の処理を直ちに実行します。
Dim i As Long
For i = 0 To 10000
	' 何らかの処理
	' 100 回に 1 回入力を受け付ける。
	If i Mod 100 = 0 Then
		DoEvents()
		' キャンセルボタンがクリックされたか判定するなど
	End If
Next

■配列にセルを代入する
'A列には名前が10,000個入っています。
'これらの名前に該当する数値が、セル範囲D2:E6に入力されている。
'A列の名前に該当する数値を、VLOOKUP関数で調べ、それぞれB列に代入していきます。

'例1_従来の例
Sub Sample1()
    Dim i As Long, B As Variant
    B = Range("A1:A10001")
    For i = 2 To 10001
        Cells(i, 2) = WorksheetFunction.VLookup(B(i, 1), Range("D2:E6"), 2, False)
    Next i
End Sub

'例1改、【Range = value】 を1回で済ませると、早い。
' ポイントは配列をセルに代入すること
Sub Sample2()
    Dim i As Long, B As Variant
    ReDim B(9999, 0)
    For i = 2 To 10001
        B(i - 2, 0) = WorksheetFunction.VLookup(Cells(i, 1), Range("D2:E6"), 2, False)
    Next i
    Range("B2:B10001") = B 'ここがクリティカル
End Sub

'例2、例1改を汎用的な範囲で示した例
Sub Sample3()
    Dim i As Long, B As Variant, n As Long
    n = Cells(Rows.Count, 1).End(xlUp).Row
    ReDim B(n - 2, 0)
    For i = 2 To n
        B(i - 2, 0) = WorksheetFunction.VLookup(Cells(i, 1), Range("D2:E6"), 2, False)
    Next i
    Range(Range("A2"), Cells(Rows.Count, 1).End(xlUp)).Offset(0, 1) = B
End Sub

■Sleep(外部API使用)
	Private Declare Sub Sleep Lib "kernel32" (ByVal ms As Long)'API宣言(32bit)
	Private Declare PtrSafe Sub Sleep Lib "kernel32" (ByVal ms As LongPtr)'API宣言(64bit)
	sleep 1000 '1秒スリープ

■データ型を確認する
'TypeName() でデータ型を文字列で取得できる。
Dim a As String
Dim b As Boolean
Dim c As New Collection
Debug.Print "a : " & TypeName(a) 'a : String
Debug.Print "b : " & TypeName(b) 'b : Boolean
Debug.Print "c : " & TypeName(c) 'c : Collection

■Collection ⇔ 配列 変換
'(1)Collection→配列変換関数
Private Function CollectionToArray(ByVal col As Collection) As Variant
	Dim resultArray() As Variant
	Dim idx As Long : idx = 0
	Dim e As Variant
	' 配列のサイズが0ではない場合、取得したサイズで配列を作成
	If col.Count <> 0 Then
		ReDim resultArray(col.Count - 1)
	End If
	' 再定義した配列にCollectionの内容を格納
	For Each e In col
		resultArray(idx) = e
		idx = idx + 1
	Next
	CollectionToArray = resultArray 
End Function

'運用例(Collection→配列変換関数)
Sub Collection→配列変換()
	Dim items As Collection
	Set items = New Collection
	items.Add 10
	items.Add 30
	items.Add 50
	Dim ary() As Variant
	ary = CollectionToArray(items)
	Dim e As Variant
	For Each e In items
	Debug.Print e
	Next e
End Sub

'(2)配列→Collection変換関数
Private Function ArrayToCollection(ary As Variant) As Collection
	Dim e As Variant
	Dim col As Collection
	Set col = New Collection
	For Each e In ary
		col.Add e
	Next e
	Set ArrayToCollection = col
End Function

'運用例(配列→Collection変換関数)
Sub 配列→Collection変換()
	Dim ary() As Variant
	Dim item As Collection
	Dim e As Variant
	ary = Array(1, 3, 5, 7, 9)
	Set item = ArrayToCollection(ary)
	For Each e In item
		Debug.Print e
	Next e
End Sub


■■FSO、WSOなど外部API
■Dictionaryオブジェクト
' 宣言
Dim 連想配列名 As Object
Set 連想配列名 = CreateObject(“Scripting.Dictionary”)
もしくは、
Dim 連想配列名 As New Scripting.Dictionary'参照設定「Microsoft Scripting Runtime」にチェック


■Gcad
Sub GetAcad()
    On Error Resume Next
    Set GcadApp = GetObject(, "GCAD.Application")
        If Err Then
           Set GcadApp = CreateObject("GCAD.Application")
           Err.Clear
        End If
    Set GcadDoc = GcadApp.ActiveDocument
    GcadApp.Visible = True
    '記入はここから------------------------------------
    Dim test As String
    For i = 1 To 9
        test = test & Cells(i, 1).Value & Chr(13)
    Next
    'IJCADに送信---------------------------------------
    GcadDoc.SendCommand "model" & Chr(13) & test
End Sub



' 
プロパティ：
Count              ：連想配列に格納された要素の数
Item(key)          ：キーに関連付けられた項目
Key(key)           ：キー
CompareMode        ：キーを区別する際に、大文字と小文字を区別するか否か指定
  vbBinaryCompare(0) ：大文字と小文字を区別
  vbTextCompare(1)   ：大文字と小文字を区別ぜず
'Dictionaryオブジェクトはキーを重複させることはできませんが、
'CompareModeプロパティを使ってキーの大文字小文字を区別するかどうか設定できます。

メソッド：
Add key, item   ：キーに項目を関連付けて追加
Exists(key)     ：指定したキーが存在すればTrueを返す
Items           ：連想配列の項目を配列にして返す。
Keys            ：連想配列のキーを配列にして返す。
Remove key      ：キーと項目のペアを削除
RemoveAll       ：全てのキーと項目を削除

'基本
Dim Map As New Dictionary
Map("hoge") = "Hoge"
Debug.Print "hoge: " & Map("hoge")

まわし方：
Set myDic = CreateObject("Scripting.Dictionary")
：
Dim Keys() As Variant
Keys = myDic.Keys
For i = 0 To 2
    Debug.Print Keys(i)
Next i
For Each e In myDic
    Debug.Print myDic.Item(e)
Next Var

追加時の注意
key = "hoge"
If myDic.Exists(key) Then '存在するか確認しないとエラーになる。
    myDicAdd key "fuga"
Else
    MsgBox key & "は削除できません", vbInformation
End If

削除時の注意：
key = "hoge"
If myDic.Exists(key) Then '存在するか確認しないとエラーになる。
    myDic.Remove key
Else
    MsgBox key & "は削除できません", vbInformation
End If


■■ワークブックイベント、ワークシートイベント

■■クラスの使い方
■クラスの使い方Lv.0(データクラス)
・構造体ではCollectionは使えない。。。(配列なら使える)
Sub データクラス()
    Dim cWordFileInfo As WordFileInfo
    Dim items As Collection
    Set items = New Collection
    Dim idx As Long
    For idx = 1 To 5
        Set cWordFileInfo = New WordFileInfo
        cWordFileInfo.ID = idx
        cWordFileInfo.Name = CStr(idx * 2)
        items.add cWordFileInfo
    Next idx
    Dim e As Variant
    For Each e In items
        Debug.Print "ID:"; e.ID & ",Name:" & e.Name
    Next e
End Sub

'/**************************************/
'クラスモジュール「WordFileInfo」
'Option Explicit
'
'Public ID As Long
'Public Name As String
'/**************************************/

■クラスの使い方Lv.1(メンバ関数)
Sub メンバー関数を使う()
    Dim p As Person
    Set p = New Person
    p.Name = "Kbt"
    Debug.Print "Age: " & p.GetAge()
    Call p.SayHello
    Set p = Nothing
End Sub

'Person(クラスモジュール)
'
'Option Explicit
'
' プロパティ
'Public  Name   As String
'Private Age    As Long
'Public  Mother As Person
'
' メソッド(引数なし、戻り値なし)
'Public Sub SayHello()
'    MsgBox "Hello, I'm " & Name & "!"
'End Sub
' メソッド(引数あり、戻り値なし)
'Public Function SayHelloTo(otherPersonName As String) As Person
'    MsgBox "Hello, " & otherPersonName & "!" & vbNewLine & "I'm " & Name & "!"
'End Function
' メソッド(引数なし、戻り値あり)
'Public Function GetAge() As Long
'    GetAge = Age
'End Function
' メソッド(引数なし、戻り値あり(オブジェクト))
'Public Function GetMother() As Person
'    Set GetMother = Mother  '（Setをつけること！）
'End Function
'
' 初期化処理(コンストラクタ)
'Private Sub Class_Initialize() ' 意図的にPrivateにすること
'    Name = "Bob"
'    Age = 30
'    Set Mother = Nothing
'    Debug.Print "initalized"
'End Sub
' 終了時処理(デストラクタ)
'Private Sub Class_Terminate() ' 意図的にPrivateにすること
'    Name = ""
'    Age = 0
'    Set Mother = Nothing
'    ' その他、使用しているリソースの解放処理を行う
'    Debug.Print "terminated"
'End Sub

■クラスの使い方Lv.2(プロパティ。Get、Set/Let)
標準モジュール
Sub test1()
    Dim p As Person
    Set p = New Person
    p.MyName = "Bob"
    Dim mom As Person
    Set mom = New Person
    Set p.MyMother = mom
    Debug.Print "age: " & p.MyAge
    Call p.SayHello
    Set p = Nothing
End Sub

'Person(クラスモジュール)
'
'Option Explicit
'
' プロパティ
'Private Name   As String 'メンバ変数を隠蔽
'Private Age    As Long   'メンバ変数を隠蔽
'Private Mother As Person 
'
' 初期化処理(コンストラクタ)
'Private Sub Class_Initialize()
'    Name = "Bob"
'    Age = 30
'    Set Mother = Nothing
'    Debug.Print "initalized"
'End Sub
' 終了時処理(デストラクタ)
'Private Sub Class_Terminate()
'    Name = ""
'    Age = 0
'    Set Mother = Nothing
'    ' その他、使用しているリソースの解放処理を行う
'    Debug.Print "terminated"
'End Sub
'
' メソッド(引数なし、戻り値なし)
'Public Sub SayHello()
'    MsgBox "Hello, I'm " & Name & "!"
'End Sub
' メソッド(引数あり、戻り値なし)
'Public Function SayHelloTo(otherPersonName As String) As Person
'    MsgBox "Hello, " & otherPersonName & "!" & vbNewLine & "I'm " & Name & "!"
'End Function
'
' プロパティプロシージャ
''・メンバ変数への直接アクセスを避け、直接加入を避ける
''  (例外値設定などをおこなう)
''・値を設定するとき、
''値型ならばLet、オブジェクト型ならばSetを使います。
''値の取得は、どちらもGetです。
'
'' 取得関数「Get」
'Property Get MyName() As String
'    MyName = Name
'End Property
'
'' 格納関数「Let」
'Property Let MyName(namae As String)
'    If namae = "" Then ' 氏名がブランクならエラー
'        Err.Raise 10000, , "名前がブランクです"
'    End If
'    Name = namae
'End Property
'
''「Get、Let、Set」の使い方
'Property Get MyAge() As Long ' 5歳さばを読んでみる
'    MyAge = Age - 5
'End Property
'Property Get MyMother() As Person
'    Set MyMother = Mother
'End Property
'Property Set MyMother(haha As Person)
'    Set Mother = haha
'End Property

■Lv3.デフォルトプロパティを設定する
・デフォルトプロパティとは、RangeオブジェクトのValueプロパティなど、
  プロパティを省略すると取得されるやつのこと。。

1.自作クラスを作成する。
	'Person(クラスモジュール)
	Private Name As String' プロパティ
	' メソッド
	Public Sub SayHello()
	    MsgBox "Hello, I'm " & Name & "!"
	End Sub
	' プロパティプロシージャ
	Property Get MyName() As String
	    MyName = Name
	End Property
	Property Let MyName(namae As String)
	    If namae = "" Then ' 氏名がブランクならエラー
	        Err.Raise 10000, , "名前がブランクです"
	    End If
	    Name = namae
	End Property

2.このPersonクラスをエクスポートする。（エクスプローラからD&D）
3.エクスポートした.clsファイルをテキストエディタで開き、
  デフォルトにしたいプロパティの先頭行にAttribute Value.VB_UserMemId = 0と入れます。
～clsファイル～
Property Get MyName() As String
    Attribute Value.VB_UserMemId = 0 ' 「MyName」プロパティをデフォルトにする。
    MyName = Name
End Property

4.このインポートする。
→見た目は変化はありませんが、デフォルトプロパティに設定されています。

5.標準モジュールにて動作を確認。
～標準モジュール～
Sub test1()
	Dim p As Person
	Set p = New Person
	p = "taro"
	Debug.Print p   ' => "taro"
End Sub

■Lv4.For Eachで回せるようにする
1.セルをたくさん入れて管理できるクラスを考える。
	For Eachで回したいものをCollectionに入れておき、
	自作クラスのNewEnumの中でCollectionの_NewEnumを呼び出します。

	CellManager(クラスモジュール)
		Option Explicit
		Public Cells As Collection
		Private Sub Class_Initialize()
		    Set Cells = New Collection
		End Sub
		Public Sub Add(newCell As Range)
		    Cells.Add newCell
		End Sub
		Public Function Item(Index As Long) As Range
		    Set Item = Cells(Index)
		End Function
		'NewEnumというのを定義するのが肝。
		Public Function NewEnum() As stdole.IUnknown
		    Set NewEnum = Cells.[_NewEnum]
		End Function

2.クラスモジュールをエクスポートしてテキストエディタで開く。
	clsファイル
		Public Function NewEnum() As stdole.IUnknown
		Attribute NewEnum.VB_UserMemId = -4   ' この行を追加！
		    Set NewEnum = CellCollection.[_NewEnum]
		End Function

3.インポートする
4.標準モジュールにて動作を確認。
～標準モジュール～
Sub test1()
    Dim cm As New CellManager
    Dim c As Range

    Range("A1").Value = "A"
    Range("A2").Value = "B"
    cm.Add Range("A1")
    cm.Add Range("A2")

    For Each c In cm
        Debug.Print c.Value
    Next
End Sub

■■Applicationクラスのメンバー(未まとめ)
GetOpenFilenameメソッド
ファイル選択ダイアログを使う。
Sub ファイル選択ダイアログ()
  Dim strFilePath As String
  strFilePath = Application.GetOpenFilename(Filefilter:="Excelブック,*.xlsx,CSVファイル,*.csv")   'ファイルパスを取得
  Debug.Print "選択したファイルパス：" & strFilePath
End Sub

Runメソッド
他のブックのマクロを動かすメソッド。
「Application.Run ブック名!プロシージャー名,引数1~30」で指定したマクロを実行できる。

Book1.xlsmサンプルコード：
Sub Test_Run()
  Application.Run "Book1.xlsm!Test", "別のブックのマクロを実行しました"  '別ブックのマクロ実行

Book2.xlsmサンプルコード：
Sub Test(strMessage As String)
  MsgBox strMessage
End Sub
このように、簡単に別ブックのマクロを実行することができます。。

Displayalertsプロパティ
ファイルの削除・ブックの削除・シートの削除で、削除確認メッセージを表示させたくないときに使う。
Trueで表示、Falseで非表示

Sub メッセージ非表示で各種処理でブック保存する()
  Dim wb1 As Workbook
  Set wb1 = Workbooks.Add   'ブック作成
  'ブック保存
  Application.DisplayAlerts = False 'メッセージを非表示
  wb1.SaveAs Filename:=ThisWorkbook.Path & "\" & "SaveAsで保存したファイル.xlsx" '"
  Application.DisplayAlerts = True  'メッセージを再表示
  wb1.Close   'ブックを閉じる
End Sub

Waitメソッド
処理を指定時間止めることができるメソッド

'Now関数で現在時刻を取得、TimeValue(“00:00:01”)を足すことで1秒処理を実現
Application.Wait Now() + TimeValue("00:00:01") 


CutCopyModeプロパティ
コピー状態(点線範囲)を解除するためのプロパティ
サンプルコード：
Sub Test()
  Activesheet.Range("A3:C3").Copy Activesheet.Range("A10:C10") 'コピー
  Application.CutCopyMode = False 'コピー状態のクリア
End Sub


OnTimeメソッド
指定した時刻に処理を実行する

OnTime(マクロ実行時刻, 実行するプロシージャ名)
'12時にshowMessageプロシージャを実行するサンプルコード：
'メイン処理
Sub Main()
  Application.OnTime TimeValue("12:00:00") , "showMessage"
End Sub

'メッセージを表示する処理
Sub showMessage()
  MsgBox "12:00時になりました。1時間休憩してください。"
End Sub

Calculationは、自動計算を手動計算に変えることができるプロパティ。
自動計算、手動計算を簡単に切り替えることができます。
セルに値が格納され再計算が走ると、速度に影響するので、値格納時は手動計算にしておくといい。

サンプルコード：
Sub Test()
  Application.Calculation = xlCalculationManual     '手動計算
  Application.Calculate   'セルの値を使って計算したいときのみ再計算実行
  Application.Calculation = xlCalculationAutomatic  '自動計算
End Sub


Statusbar プロパティ
ステータスバーに値を表示するためのプロパティ。
Sub Test()
    Dim i As Long
    For i = 1 To 1000
        Application.StatusBar = i & "回目の処理実行中…"
    Next i
    Application.StatusBar = False
End Sub



