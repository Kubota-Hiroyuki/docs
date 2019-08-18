
■■特定のセルが空白の場合、その行を削除
'/**************************************************/
' 機能:特定のセルが空白の場合、その行を削除
'
' 引数:(I) col 列番号
'
' 返り値:   なし
'
' 機能説明:
'
' 備考:
'
'/**************************************************/
Sub 特定のセルが空白の場合その行を削除(col As Long)
	Dim lRow As Long
	Dim i As Long
	Application.ScreenUpdating = False
	lRow = Cells(Rows.Count, col).End(xlUp).Row ' = 最終行の値(65536)
	For i = lRow To 2 Step -1
		If Cells(i, 1).Value = "" Then
			Cells(i, 1).EntireRow.Delete
		End If
	Next i
End Sub

■■画像のみならず図形は、とにかくなんでも消す
'/**************************************************/
' 機能:画像のみならず図形は、とにかくなんでも消す
'
' 引数:なし
'
' 返り値:なし
'
' 機能説明:
'
' 備考:
'
'/**************************************************/
Private Sub IMAGE_DELETE_allShapes()
    Dim MyOb As Object
    Dim i1, i2, i3 As Integer
    i1 = ActiveSheet.Shapes.Count
    i2 = ActiveSheet.DrawingObjects.Count
    i3 = ActiveSheet.Pictures.Count
    For Each MyOb In ActiveSheet.Shapes
        MyOb.Delete
    Next
    MsgBox ( _
        "シート内のオートシェイプ の数:" & i1 & "⇒" & ActiveSheet.Shapes.Count & vbCrLf & _('Drop Down
        "シート内のDrawingObjectの数:" & i2 & "⇒" & ActiveSheet.DrawingObjects.Count & vbCrLf & _'←テキストボックスやフキダシ
        "シート内のPicture の数:" & i3 & "⇒" & ActiveSheet.Pictures.Count)'←画像
End Sub

