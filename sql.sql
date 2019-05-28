
--No.1　基本
SELECT *
FROM   MS_Book

/*
本マスタからデータを抽出する

＜POINT＞
SELECT		：抽出するの意
FROM　○○	：○○のテーブルから
*/

--No.2　WHERE
SELECT *
FROM   MS_Book
WHERE  BookCode = 'A011'

/*
本マスタから本コードA011のデータを抽出する

＜POINT＞
WHERE 「式」：「式」に一致するデータを抽出する
値は'で囲む必要がある
*/

--No.3　WHERE範囲指定
SELECT *
FROM   MS_Book
WHERE  BookCode >= 'A010'
AND    BookCode <= 'A015'

/*
本マスタから本コードがA010～A015の範囲のデータを抽出する

＜POINT＞
WHERE	「式1」 AND 「式2」	：「式1」かつ「式2」に一致する
「式」には不等号も使える
 なお<>は一致しないものという意味
*/


--No.4　BETWEEN
SELECT *
FROM   MS_Book
WHERE  BookCode BETWEEN 'A010' AND 'A015'

/*
本マスタから本コードがA010～A015の範囲のデータを抽出する

＜POINT＞
BETWEEN	○ AND △	：○～△の範囲
*/

--No.5　IN
SELECT *
FROM   MS_Book
WHERE  BookCode IN('A001','A004','A017')

/*
本マスタから本コードA001,A004,A017のデータを抽出する

＜POINT＞
IN (○,△,□)		：○もしくは△もしくは□
*/

--No.6　LIKE
SELECT *
FROM   MS_Book
WHERE  BookName LIKE '%エクセル%'

/*
本マスタから本の名前に"エクセル"が含まれているデータを抽出する

＜POINT＞
LIKE		：「っぽい」の意味
%			：LIKEで使うと全ての文字の意味
			　EXCELやWindowsの検索時の*と同じ
*/


--No.7　OR
SELECT *
FROM   MS_Book
WHERE  BookName LIKE '%上'
OR     BookName LIKE '%中'
OR     BookName LIKE '%底'

/*
本マスタから本の名前が"上"か"中"か"底"で終わるデータを抽出する

＜POINT＞
OR	「式1」 OR 「式2」	：「式1」もしくは「式2」に一致する
*/

--No.8　LIKE　文字範囲指定
SELECT *
FROM   MS_Book
WHERE  BookName LIKE '[1-9]%'

/*
本マスタから本の名前が"1～9"の文字で始まるデータを抽出する

＜POINT＞
[A-C]				：A～Cの文字
*/

--No.9　LIKE　％指定
SELECT *
FROM   MS_Book
WHERE  BookName LIKE '%[%]%'

/*
本マスタから本の名前に"%"が含まれたデータを抽出する

＜POINT＞
%は予約語なので特殊な書き方が必要
他にもESCAPE句とかいう方法もあるらしい（こっちのが好き）
その他の予約語
_ → [_]
[ → [[]
*/

--No.10　シングルクォーテーション指定
SELECT *
FROM   MS_Book
WHERE  BookName LIKE '%''%'

/*
本マスタから本の名前に"'"が含まれたデータを抽出する

＜POINT＞
もっと面倒くさい予約語
'は２個つなげると文字に認識される
*/

--No.11　列指定
SELECT BookName
,      Author
FROM   MS_Book

/*
本マスタから本の名前と著者名の列を抽出する

＜POINT＞
SELECT *			：全ての列を抽出
SELECT [列名]		：[列名]のみを抽出
*/

--No.12　ORDER BY
SELECT BookName
FROM   MS_Book
ORDER BY BookName ASC

/*
本マスタから本の名前の列を本の名前順に抽出する

＜POINT＞
ORDER BY [列名]		：[列名]の昇順で並べる
降順はORDER BY [列名] DESC
*/

--No.13　TOP
SELECT TOP 3 BookName
FROM   MS_Book
ORDER BY BookName ASC

/*
本マスタから本の名前の列を本の名前順に上から３件抽出する

＜POINT＞
SELECT TOP n		：上からn件抽出する
*/

--No.14　追加列の編集
SELECT BookName
,      'Hello Gensokyo!' AS [Message]
FROM   MS_Book

/*
本マスタから本の名前の列と横に"Hello Gensokyo!"の文字を書いた列名Messageを抽出する

＜POINT＞
SELECT '文字'		：'文字'を列として並べられる
AS [任意の名前]		：[任意の名前]を列名として名付けられる		
*/

--No.15　CASE
SELECT BookName
,      CASE WHEN CategoryName = 'コミック' THEN 'マンガ'
            ELSE '' END AS [マンガフラグ]
FROM   MS_Book

/*
本マスタから本の名前の列と分類名がコミックの場合"マンガ"と表示する列名マンガフラグを抽出する

＜POINT＞
CASE 式1 THEN 値1
     式2 THEN 値2
	 ELSE 値3 END	：式１が成立したら値1、式２が成立したら値2、それ以外は値3を表示した列
*/


--No.16　SUM
SELECT SUM(ReprintCount)
FROM   MS_Book

/*
本マスタから重版数の合計を抽出する

＜POINT＞
SUM（列）		：列の値の合計
集計関数は他にもCOUNT、MIN、MAXをよく使う
*/

--No.17　GROUP BY
SELECT CategoryName
,      SUM(ReprintCount)
FROM   MS_Book
GROUP BY CategoryName

/*
本マスタから分類名と分類名ごとの重版数の合計を抽出する

＜POINT＞
GROUP BY 列名	：列名ごとに集計する
集計関数があるときに使用する
*/

--No.18　DISTINCT
SELECT DISTINCT CategoryName
FROM   MS_Book

/*
本マスタから分類名の列を重複データを除いて抽出する

＜POINT＞
SELECT DISTINCT		：重複データを除いて抽出する
*/

--No.19　HAVING
SELECT CategoryName
,      COUNT(*)
FROM   MS_Book
GROUP BY CategoryName
HAVING COUNT(*) = 1

/*
本マスタから分類名と分類名ごとのデータ件数を抽出するが、そのうち件数が1件しかないものを抽出する

＜POINT＞
HAVING 式	：式に一致するデータのみ抽出
HAVINGの式には集計関数が使える（WHEREは不可）
*/


--No.20　実行順序
SELECT TOP 3 CategoryName
,      SUM(CASE WHEN CategoryName = 'コミック' THEN ReprintCount * 0.5
                ELSE ReprintCount END) AS [評価点]
FROM   MS_Book
WHERE  DeleteFlag = 0
GROUP BY CategoryName
HAVING COUNT(*) > 1
ORDER BY [評価点] DESC


/*
本マスタから削除フラグが立っていないデータを
分類名ごとに集計し、件数が1件より多い分類に対し、
評価点が最も多い３件を抽出する
評価点は本ごとの重版数の合計で計算する
ただし、コミックカテゴリだけは半分の値として集計する

＜POINT＞
これまでのを一通り混ぜたクエリ
クエリは以下の順番で処理されていく
↓FROM
↓WHERE
↓GROUP BY
↓SUM、COUNT等
↓HAVING
↓SELECT 列名
↓(DISTINCT)
↓ORDER BY
↓TOP
・WEHEREで集計関数が使えないのはGROUPBYより前のせい
・CASEはそれぞれ使われている句の位置で処理される（今回はSELECT 列名）
・ORDER BYで実際にはない[評価点]という列が使えるのは
　SELECT 列名の後に処理されているため（WHEREで[評価点]などは使えない）
*/
13_解説SQL.sql
