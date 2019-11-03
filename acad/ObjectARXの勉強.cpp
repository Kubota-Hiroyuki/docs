■■■トレーニングガイド■■■
■グラフィカルオブジェクトの選択
1.単一オブジェクトの選択
acedEntSel()を使用して、ads_name型のエンティティ名を取得

2.複数オブジェクトの選択(選択セット)
acedSSGet()を使用して、ads_name型のエンティティ名を取得
使用した選択セット領域はacedSSFree()で開放が必須

・ads_name型とは
C言語で使っていたADSカスタマイズ環境から継承
ObjectARXではオブジェクトIDを識別子として利用。
ads_name → AcDbObjectIdオブジェクト変換が必要。
acdbGetObjectID() グローバル関数を使用

■オブジェクト操作時の原則
	①. オブジェクト ID でオブジェクトを特定
	↓
	②. オブジェクト ID でオブジェクトを オープン ・ ここではじめてオブジェクトを取得
	↓
	③. オブジェクトへ アクション ・ メンバ関数にアクセス
	↓
	④. オブジェクトを クローズ ・ ここでオブジェクトへのアクションがコミットされる

■オブジェクト操作の基本
	例：・選択したオブジェクトの色を赤に変更 …
	ads_name ename;
	ads_point pt;
	acedEntSel( _T("\nオブジェクトを選択:"), ename, pt );
	AcDbObjectId objId;
	acdbGetObjectId( objId, ename );   //  ① オブジェクトIDでオブジェクトを特定
	AcDbEntity* pEnt;
	acdbOpenAcDbEntity( pEnt, objId, AcDb::kForWrite ); // ② オブジェクトIDでオブジェクトをオープン
	pEnt->setColorIndex( 1 ); // 1=赤  // ③ オブジェクトへアクション
	pEnt->close();                     // ④ オブジェクト をクローズ

■オープン モード
・オブジェクトのオープンにはオープンモードが必要
 ・ AcDb::kForWrite：書き込みオープン
 ・ AcDb::kForRead：読み込みオープン
 ・ AcDb::kForNotify：通知オープン
 ・ 読み込みモードで書き込み操作をするとクラッシュします !!

・クローズで処理が完結してアクションが反映されます
・ アクション後にはクローズが必須 !! 

■オブジェクトの特定方法は2通り
1. 選択操作でオブジェクトを特定する方法
 ・ 適用可能なのはグラフィカル オブジェクトのみ
 ・ ADS グローバル関数の利用が必須

2. 図面データベースからの追跡でオブジェクトを特定する方法
 ・ 下位テーブル オブジェクトへのアクセス
 ・ オブジェクト定義（クラス）のメンバ関数を利用

■メモリ上の図面構造（図面データベース）
・オブジェクトが階層的に整理されている
 ・ トップ オブジェクトが 図面データベース オブジェクト
 ・ オーナシップ接続 によって論理的な つながり を保持
 ・ オブジェクトは然るべき コンテナ オブジェクト に格納

■メモリ上の図面構造に線分を追加する場合は ...
1. 新しく AcDbLine クラスを元にオブジェクトを作成して
2. モデル空間を表す AcDbBlockTableRecord に論理上のつながり
 ・ オーナーシップ接続の確立
 ・AcDbBlockTableRecord オブジェクトを取得する必要あり

■メモリ上のオブジェクト階層の追跡
・ObjectARX は図面データベースを追跡
 ・ AcDbDatabase オブジェクトありき
 ・ 下位オブジェクトへは オブジェクトID と呼ばれる識別子を介してアクセス
 ・ 現在の 図面データベース（AcDbDatabase オブジェクト） を取得

■図面データベースからのオブジェクト追跡
 ・AcDbDatabase の下位テーブル オブジェクトをオープン
 ・テーブル オブジェクトからレコード オブジェクトをオープン

■現在の図面データベースの取得
・AcDbDatabase クラスで定義
 ・ AutoCAD 上は複数の図面をオープン可能
 ・ 複数の AcDbDatabase インスタンスを識別
・カレント図面データベース
 ・ 現在編集対象となっている図面データベース
 ・ AcDbHostApplicationServices::workingDatabase() を使って取得
  ・ AcDbDatabase オブジェクトを返す
 ・ AcDbHostApplicationServices オブジェクトの取得
  ・ acdbHostApplicationServices() 関数で取得可能

■非グラフィカル オブジェクトの追加例
・画層を追加する場合
AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
AcDbLayerTable* pTbl;
pDb->getLayerTable( pTbl, AcDb::kForWrite ); // 画層テーブルをオープン
if ( !pTbl->has( szBuf ) ){ 
	AcDbLayerTableRecord* pLayer = new AcDbLayerTableRecord;
	pLayer->setName( szBuf );
	pTbl->add( pLayer ); // 画層テーブルへアクション
	pLayer->close(); // 新規オブジェクトもオーナーシップ接続が 確立された時点でクローズが必要!
}
else
	acutPrintf( _T("\n画層 %s は既に登録されています..."), szBuf );
pTbl->close(); // 画層テーブルをクローズ

■グラフィカルオブジェクトの追加例
・モデル空間に線分を追加
AcGePoint3d ptStart( 0.0, 0.0, 0.0 );            ┬ オブジェクトの新規作成
AcGePoint3d ptEnd( 100.0, 100.0, 100.0 );        │
AcDbLine* pLine = new AcDbLine( ptStart, ptEnd );┘

AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase(); ┬  オブジェクトの追加処理
AcDbBlockTable* pTbl;                                               │
pDb->getBlockTable( pTbl, AcDb::kForRead );                         │// ブロックテーブルをオープン
AcDbBlockTableRecord* pRec;                                         │
pTbl->getAt( _T("*MODEL_SPACE"), pRec, AcDb::kForWrite );           │// ブロック テーブルへアクション、モデル空間をオープン
pTbl->close();                                                      │// ブロックテーブルをクローズ
pRec->appendAcDbEntity( pLine );                                    │// モデル空間にアクション
pRec->close();                                                      │// モデル空間をクローズ
pLine->close();                                                     ┘  →新規オブジェクトもオーナーシップ接続が 
                                                                          確立された時点でクローズが必要です

■コードの補足
コードの補足 － その1
・点 もオブジェクトとして扱う
	・ AcGePoint3d クラスで定義
・オブジェクトの追加は 2 つの手順で実施
	1. オブジェクトは new で新規作成が原則
	2. その後、オーナーシップ接続手続き実施
・appendAcDbEntity()
	・ グラフィカル オブジェクトを追加
	・ AcDbBlockTableRecord にオブジェクトをつなげます
	・ このメソッド呼び出しでオーナーシップ接続が確立
・オーナーシップ接続
	・ オーナーシップ接続を持つオブジェクトのみが DWG ファイル保存対象に
	・ オーナーシップ接続のあるオブジェクトはクローズは必須 
・getAt() でレコード オブジェクトを特定
	・ 特別なブロックテーブルレコード名
	・ "*MODEL_SPACE" ⇒ モデル空間 ・ "*PAPER_SPACE" ⇒ ペーパー空間 (レイアウト1)
	・ "*PAPER_SPACE0" ⇒ ペーパー空間 (レイアウト2)
	・ …以降数字の増加
・ブロックテーブルレコードはブロック定義を示します
	・ 上記以外は通常のブロック定義
	・ ユーザ作成時の任意名で取得可能

■ユーザとの対話
・ADS 環境からのグローバル関数を使用
	acedGetPoint()   座標を入力/指示させる
	acedGetReal()    実数を入力させる
	acedGetString()  文字列を入力させる
	acedGetInt()     整数を入力させる
	acedGetAngle()   システム変数 ANGBASE を基準にして角度を入力/指示させる
	acedGetCorner()  ボックス ラバーバンドで座標を入力/指示させる
	acedGetDist()    距離を入力/2点指示させる
	acedGetKword()   キーワードを入力させる
	acedGetOrient()  システム変数 ANGBASE を無視して角度を入力/指示させる
	acedInitGet()    キーワードの設定と入力関数の状態を制御する

■グラフィカルオブジェクトの追加例
・モデル空間に線分を追加
	ads_point pt1;
	acedInitGet( RSG_NONULL, NULL );
	int stat = acedGetPoint( NULL, _T("\n始点を指示:"), pt1 );
	if ( stat != RTNORM ) return;

	ads_point pt2;
	acedInitGet( RSG_NONULL, NULL );
	stat = acedGetPoint( pt1, _T("\n終点を指示:"), pt2 );
	if ( stat != RTNORM ) return;
	
	AcGePoint3d ptStart( pt1[X], pt1[Y], pt1[Z] );
	AcGePoint3d ptEnd( pt2[X], pt2[Y], pt2[Z] );
	AcDbLine* pLine = new AcDbLine( ptStart, ptEnd );

	AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pTbl;
	pDb->getBlockTable( pTbl, AcDb::kForRead );
	AcDbBlockTableRecord* pRec;
	pTbl->getAt( _T("*MODEL_SPACE"), pRec, AcDb::kForWrite );
	pTbl->close();
	pRec->appendAcDbEntity( pLine );
	pRec->close();
	pLine->close();

■エラー処理について
・例外処理（try~catch~throw）は利用しません
	・ 関数戻り値での処理
・2 種類の関数戻り値の存在
	・ ADS 環境から継承したグローバル関数
		・ 戻り値：int 型
		・ adscodes.h での RTNORM、RTERROR、RTCAN、…
・ ObjectARX ネイティブ
	・ メンバ関数、グローバル関数
	・ acadstrc.h での戻り値：Acad::ErrorStatus 列挙型

■■L4：オブジェクトの選択
■単一オブジェクトの選択例
・ads_name から AcDbObjectId への変換処理
	ads_name ename;
	ads_point pt;
	int stat = acedEntSel( _T("\nオブジェクトを選択:"), ename, pt );
	if ( stat != RTNORM ) return;
	AcDbObjectId objId;
	acdbGetObjectId( objId, ename );
	acutPrintf( _T("\n選択したオブジェクトのオブジェクトID は%x"), objId );

■複数オブジェクトの選択例
・ads_name から AcDbObjectId への変換処理
	ads_name sset;
	int stat = acedSSGet( NULL, NULL, NULL, NULL, sset );
	if ( stat != RTNORM ) return;

	long nTotal;
	acedSSLength( sset, &nTotal );

	ads_name ename;
	AcDbObjectId objId;
	AcDbObjectIdArray objIdary;
	for( long nIndex=0; nIndex<nTotal; 	nIndex++) {
		acedSSName( sset, nIndex, ename );
	acdbGetObjectId( objId, ename ); // 選択セット要素の取り出し (ads_name 型で)
	objIdary.append( objId );
	}
	acedSSFree( sset ); // 選択セット領域の解放

	for( long nIndex=0 ; nIndex<objIdary.length(); 	nIndex++ ) {
		acutPrintf( _T("\n選択したオブジェクトのObjID は %x"), objIdary.at(nIndex) );
	} 

■ランタイム タイプ識別子
・ランタイム タイプ識別子
	・ ObjectARX のルートクラスである AcRxObject に実装
	・ 実行時にクラス毎にクラス自身と継承関係を識別
・主要なメンバ関数
	AcRxObject::desc() オブジェクトのクラスか判明している場合にランタイム クラス識別子を返す。
	AcRxObject::isA() どのクラスを元にしたオブジェクトか不明の場合にランタイム クラス識別子を返す。
	AcRxObject::isKinfOf() オブジェクトが特定のクラス、または、派生クラスに属しているか系統を調べる。
	AcRxObject::cast() オブジェクトをランタイムクラス識別子に基づいて適切のダイナミックキャストする。 派生関係にないため変換できない場合には NULL を返す

■ランタイム タイプ識別子の利用例
・選択オブジェクトが円なら …
	ads_name ename;
	ads_point pt;
	int stat = acedEntSel( _T("\nオブジェクトを選択:"), ename, pt );
	if ( stat != RTNORM ) return;

	AcDbObjectId objId;
	acdbGetObjectId( objId, ename );
	AcDbEntity* pEnt;
	acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
	if ( pEnt->isA() == AcDbCircle::desc()) {
		double radius = AcDbCircle::cast( pEnt )->radius();
		acutPrintf( _T("\n円の半径は %lf"), radius );
	}
	pEnt->close();
	// ヒットしたエンティティの識別子 『pEnt->isA()』 (なんかの識別子)が、円識別子 『AcDbCircle::desc』 なら、
	// 円識別子にキャスト『AcDbCircle::cast』して、中の半径プロパティを取り出している。

■■L5イテレタ走査
■イテレタ オブジェクト
・コンテナ オブジェクトの要素を走査
 ・ 要素オブジェクトを列挙する機能 
 ・ 列挙したオブジェクトをオープンする機能 
 ・ Visual Basic の For Each ～ Next のような役割
・コンテナ オブジェクト毎に専用のイテレタ クラス 
 ・ 9 つのシンボル テーブル 
 ・ グラフィカル オブジェクトを持つレコード 
 ・ ブロック定義、モデル空間、ペーパー空間
  ・ AcDbBlockTableRecord

■テーブル オブジェクトのイテレタ
・コンテナ オブジェクト毎に専用のイテレタ クラス
	AcDbBlockTableIterator       モデル空間やペーパー空間を含む、登録されたブロック定義を走査するための
	                             ブロック テーブル イテレタ
	AcDbDimStyleTableIterator    登録された寸法スタイルを走査するための寸法スタイル テーブル イテレタ
	AcDbLayerTableIterator       登録された画層を走査するための画層 テーブル イテレタ
	AcDbLinetypeTableIterator    登録された線種を走査するための線種 テーブル イテレタ
	AcDbRegAppTableIterator      登録された拡張エンティティ用のアプリケーション名を走査するための
	                             アプリケーション名 テーブル イテレタ
	AcDbTextStyleTableIterator   登録された文字スタイルを走査するための文字スタイル テーブル イテレタ
	AcDbUCSTableIterator         登録されたユーザ座標系を走査するためのユーザ座標系 テーブル イテレタ
	AcDbViewportTableIterator    登録されたビューポートを走査するためのビューポート テーブル イテレタ
	AcDbViewTableIterator        登録されたビューを走査するためのビュー テーブル イテレタ
	AcDbDictionaryIterator       登録されたディクショナリを走査するためのディクショナリ イテレタ
	AcDbBlockTableRecordIterator ブロック定義やモデル空間、ペーパー空間内のグラフィカル オブジェクトを 走査するためのブロック テーブル イテレタ AcDbDictionaryIterator ディクショナリ要素を走査するためのディクショナリ イテレタ

・利用方法
	1. newIterator() で構築してイテレタ オブジェクトのポインタを取得
	2. for( ポインタ->start() ; ポインタ->done() ; ポインタ->step() ){ で走査
	3. 必要に応じて ポインタ->getRecord()、getEntity() でオープン
	4. delete でイテレタ オブジェクトを削除
・イテレタは図面には保存されません
	・ クローズの必要なし
	・ 使用後は delete 演算子での削除が必要

■イテレタの利用例
・全画層名の取得
AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
AcDbLayerTable* pTbl;
pDb->getLayerTable( pTbl, AcDb::kForRead );

AcDbLayerTableIterator* pItr;
pTbl->newIterator( pItr );
pTbl->close();

TCHAR* pszName;
AcDbLayerTableRecord* pRec;
for( pItr->start(); !pItr->done(); pItr->step()) {
	pItr->getRecord( pRec, AcDb::kForRead );
	pRec->getName( pszName );
	acutPrintf( _T("\n取得した画層の名前は[%s]です..."), pszName );
	pRec->close();
	free( pszName );
}
delete pItr;

■■L6 MFCダイアログ
■AutoCAD のユーザ インタフェース
・2 つの実装形態 
	・ .NET Framework による実装 
		・ .NET Framework クラス ライブラリを利用 
	・ MFC による実装 
		・ MFC ダイナミック リンク ライブラリを参照 
・両実装で AutoCAD 固有クラスを公開 
	・ AdUixxx クラス 
		・ AutoCAD 固有ではない一部 MFC 拡張クラス群 
	・ AcUixxx クラス 
		・ AdUixxx クラスを元にした AutoCAD 固有なクラス群
■AutoCAD 固有の MFC クラス
・例）
	CAcUiLineWeightComboBox クラス // 線太さドロンボックス
	CAcUiColorComboBox クラス // カラードロンボックス
	CAdUiPaletteSet クラス ┐// マーカー設定サブウインドウ
	CAdUiPalette クラス    ┘

■MFC 実装に関する問題
・ObjectARX 内定義の MFC ダイアログ 
	・ リソース ID によるリソース管理 
	・ ダイアログ、アイコン、イメージ…
・リソース競合が発生する可能性あり ! 
	1. アクティブ リソースを AutoCAD から ObjectARX へ切り替え
	2. ObjectARX 内定義のリソースを使用
	3. アクティブ リソースを ObjectARX から AutoCAD へ戻す
・CAcModuleResourceOverride インスタンスで回避する
※L6の演習はCDialog派生クラスを使ってMFCDLGを使っている。


   ■クラスライブラリ
	AcRx  アプリケーションのバインドと、ラインタイム クラス登録およびランタイム クラス識別に使うクラス
	AcEd  ネイティブ AutoCAD コマンドの登録と AutoCAD イベント通知のためのクラス
	AcDb  AutoCAD データベース クラス
	AcGi  AutoCAD エンティティをレンダリングするためのグラフィックス クラス
	AcGe  一般的な線形代数オブジェクトやジオメトリ オブジェクト用のユーティリティ クラス

■モードレス ダイアログからの図面データベース アクセス
ads_point pt1;
acedInitGet( RSG_NONULL, NULL );
int stat = acedGetPoint( NULL, _T("\n始点を指示:"), pt1 );
if ( stat != RTNORM ) return;

ads_point pt2;
acedInitGet( RSG_NONULL, NULL );
stat = acedGetPoint( pt1, _T("\n終点を指示:"), pt2 );
if ( stat != RTNORM ) return;

acDocManager->lockDocument( acDocManager->curDocument(), AcAp::kWrite ); // ドキュメントをロック !!

AcGePoint3d ptStart( pt1[X], pt1[Y], pt1[Z] );
AcGePoint3d ptEnd( pt2[X], pt2[Y], pt2[Z] );
AcDbLine* pLine = new AcDbLine( ptStart, ptEnd );

AcDbDatabase* pDb=acdbHostApplicationServices()->workingDatabase();
AcDbBlockTable* pTbl;
pDb->getBlockTable( pTbl, AcDb::kForRead );
AcDbBlockTableRecord* pRec;
pTbl->getAt( _T("*MODEL_SPACE"), pRec, AcDb::kForWrite );
pTbl->close();
pRec->appendAcDbEntity( pLine );
pRec->close();
pLine->close();

acDocManager->unlockDocument( acDocManager->curDocument() );

※ドキュメントの ロック が対象図面を特定します
（ドキュメント=図面ウィンドウは AcApDocument クラスで表現）

■■L7リアクタ
■AutoCAD イベントを監視
・ .NET Framework のデリゲートを利用
・ AutoCAD上で発生するイベントの例 
	・ コマンドの実行 
	・ ドキュメントのクローズ 
	・ 図面データベースへのオブジェクトの追加 …など
・ AutoCAD上で発生するイベントの監視 
	・ コマンドの監視         ：エディタ リアクタ 
	・ ドキュメントの監視     ：ドキュメント リアクタ 
	・ 図面データベースの監視 ：データベース リアクタ

■リアクタの種類と定義クラス
・一時リアクタ 
	・ 図面に保存されない 
	・ セッション間のみ有効 
	・ 利用時にはセッション毎に登録が必要 
	・ 一時リアクタ クラスからのクラス派生が必要
・不変リアクタ 
	・ 図面に保存される 
	・ セッション間でリアクタが保持される 
	・ カスタム クラスの定義が必要
■一時リアクタ
	AcDbObjectReactor 特定の 
	AcDbObject 派生オブジェクトを監視する 
	AcDbEntityReactor 特定の 
	AcDbEntity 派生オブジェクトを監視する 
	AcDbDatabaseReactor 図面データベースを全体監視する
	AcEditorReactor コマンドを監視する
	AcRxDLinkerReactor ObjectARX アプリケーションのロード/ロード解除を監視する
	AcApDocManagerReactor ドキュメントを監視する 
	AcDbLayoutManagerReactor レイアウトを監視する 
	AcDbSummaryInfoReactor 図面のプロパティを監視する
	AcApProfileManagerReactor プロファイルを監視する
	AcEdInputContextReactor 入力関数などを監視する
■不変リアクタ
	AcDbObject 以下の派生クラスは 図面ファイルへの保存機能を 継承 !!
	    ↑AcDbObject 派生クラス 特定の AcDbObject 派生オブジェクトを監視する

■リアクタの動作メカニズム
	・エディタ リアクタでコマンドを監視する場合
	シナリオ エディタ窓で
		"LINE" [Enter]を押した！
			→ AcEditorReactor::commandWillStart() ┐ // メモリ上のエディタ リアクタ オブジェクト
		"次の点を指定"で [Enter]を押した！         │
			→ AcEditorReactor::commandCancelled() │
		"次の点を指定"で [ESC]を押した！           │
			→ AcEditorReactor::commandEnded()     ┘

■一時リアクタの実装
1. 新しいリアクタクラスを作成
	・ 目的とするリアクタクラスから派生
2. 必要とするイベントを選択
	・ イベントに対応するメンバ関数をオーバーライド
3. リアクタオブジェクトの作成
	・ new 演算子
4. リアクタの登録
	・ addReactor()
5. リアクタ監視
6. リアクタの登録解除
	・ removeReactor()
7. リアクタ オブジェクトの削除
	・ delete 演算子 

■エディタ リアクタの実装
・エディタ リアクタの追加 
	・ AcEditor::addReactor() 
・エディタ リアクタの削除 
	・ AcEditor::removeReactor() 
・acedEditor マクロ 
	・ AcEditor をオブジェクト ポインタを返す 
	・ 実際のリアクタ登録、削除時に利用

■データベース リアクタ
・AcDbDatabaseReactor クラス
	・図面データベース上の動作を監視
		AcDbDatabaseReactor::headerSysVarChanged() 図面に保存されるシステム変数の変更後に通知される 
		AcDbDatabaseReactor::headerSysVarWillChange() 図面に保存されるシステム変数の変更前に通知される 
		AcDbDatabaseReactor::objectAppended() オブジェクトが追加された後に通知される 
		AcDbDatabaseReactor::objectErased() オブジェクトが削除された後に通知される 
		AcDbDatabaseReactor::objectModified() オブジェクトが修正された後に通知される 
		AcDbDatabaseReactor::objectOpenedForModify() オブジェクトが修正のためにオープンされた後に通知される 
		AcDbDatabaseReactor::objectReappended() 削除後のオブジェクトが再追加された後に通知される （Undo 後の Redo） 
		AcDbDatabaseReactor::objectUnappended() 追加後のオブジェクトが削除された後に通知される （Undo）

■データベース リアクタ
・データベース リアクタの追加 
	・ AcDbDatabase::addReactor() 
・データベース リアクタの削除 
	・ AcDbDatabase::removeReactor() 
・AcDbDatabase オブジェクト ポインタは ? 
	・ 実際のリアクタ登録、削除時に利用 
	・ SDI 環境時と MDI 環境時の差異を考慮 
	・ エディタ リアクタは AutoCAD につきに１つのみ 
	・ 図面データベースは複数存在する可能性あり

■ドキュメント毎データ

・適切な図面データベース オブジェクトの取得 
・SDI 環境： 
	・ カレント図面データベース考慮でOK 
	・ acdbHostApplicationServices()->workingDatabase() 
・MDI環境： 
	・ AutoCAD 上の表示図面に対応する必要 
	・ ドキュメント リアクタを併用してドキュメント切り替えを監視 
		・ AcApDocManagerReactor 
	・ 各ドキュメントのポインタを保持 
		・ AcApDocument 
		・ AcApDocument::database()

■ドキュメント毎データの実装
・標準的なアプローチ
	・グローバル ポインタでリスト チェインを構築
	・ドキュメント オブジェクト ポインタを保持



管理クラス                                データクラス                         データクラス                    
   +---------------------------------+   +-------------------------------+    +-------------------------------+
-->+最初のデータクラス へのポインタ  |-->+次のデータクラスへのポインタ   |--->+次のデータクラスへのポインタ   |
   +---------------------------------+   +-------------------------------+    +-------------------------------+
   |ドキュメントリアクタのポインタ   |   |対応するドキュメントのポインタ |    |対応するドキュメントのポインタ |
   +---------------------------------+   |AcApDocument*                  |    |AcApDocument*                  |
   |カレントドキュメントのデータクラ |   +-------------------------------+    +-------------------------------+
   |スポインタ（ドキュメントリアクタ |   |実際のデータ（複数可）         |    |実際のデータ（複数可）         |
   |通知で更新する）                 |   |通知で更新する）               |    |通知で更新する）               |
   +---------------------------------+   +-------------------------------+    +-------------------------------+
                ↑
                ↑
                ↑
	+---------------------------------+
	|『ドキュメントリアクタ』         |
	|ドキュメントの表示切り替えを通知 |
	|（アクティブになったドキュメント |
	|のポインタ）                     |
	+---------------------------------+

■■ドキュメント毎データの実装
・ObjectARX Wizard での実装 
	・ AcApDataManager クラス 
		・ ドキュメント リアクタ 
		・ template クラス 
		・ AcApDMgr.h 
	・ CDocData クラス 
		・ ドキュメント毎のデータ格納用管理クラス 
		・ AcApDataManager<CDocData> DocVars; 
		・ DocData.h、DocData.cpp 
	・ 実際のドキュメント毎データ取得 例 
		・ DocVars.docData().<リアクタオブジェクトポインタ> 
		・ <リアクタ オブジェクト ポインタ> は ドキュメント毎データ

■■L8カスタムオブジェクト
■カスタムオブジェクト
・独自オブジェクトの作成機能                   AcDbObject
・グラフィカル オブジェクト                     |  
	・ AcDbEntity 以下から派生                  +--MyObjectClass //カスタムオブジェクト (非グラフィカルオブジェクト)
	・ 描画情報を含むエンティティの実装         |  
・非グラフィカル オブジェクト                   +---cDbEntity
	・ AcDbObject 以下から派生                       | 
	・ 非描画情報のみのオブジェクトの実装            +-MyEntityClass //カスタムオブジェクト (グラフィカルオブジェクト)
	・ カスタム ディクショナリの作成は可能           |
	・ シンボル テーブルの作成は禁止

・独自オブジェクト作成時のコンテナ オブジェクトは ?
・グラフィカル オブジェクト 
	・ モデル空間、ペーパー空間、ブロック定義 
	・ AcDbBlockTableRecord
・非グラフィカル オブジェクト 
	・ カスタム ディクショナリ 
	・ AcDbDictionary
・数多くのメンバ関数オーバーライドが必要 
	・各メンバ関数は機能毎に AutoCAD 呼び出される 

	AutoCAD はオブジェクトの振る舞いを カスタムオブジェクトの定義 .arx に照会


■プロキシオブジェクト
・カスタム オブジェクトの定義 .arx がロード解除された状態でも発生
	・AutoCAD がオブジェクトの振舞いを照会できない

	カスタムオブジェクトの定義 .arx が ロード解除されて振る舞いを照会できない

■プロキシ オブジェクト の防止

・プロキシ オブジェクト防止の方策 
	・ AutoCAD からロード解除できない .arx ファイルで カスタム オブジェクトを定義 
	・ acrxDynamicLinker->unlockApplication() 
	・ acrxDynamicLinker->lockApplication() 
・DB/UI分離での実装（推奨） 
	・ DB 部分モジュール 
		・ カスタム オブジェクトの定義部分のみを実装 
		・ 拡張子を .dbx に変更 
	・ UI 部分モジュール 
		・ カスタム オブジェクトをコンテナへの追加処理をするコマンドを実装 
		・ 拡張子は .arx のまま 
・DB/UI 分離の意味
	・ UI 部分：カスタム オブジェクト作成、編集コマンド 
		・ UIがなければカスタム オブジェクトへはアクセス不可 
		・ 販売 ベース 
	・ DB 部分：カスタム オブジェクトの定義 
		・ 定義部のみでコマンドは内包せず 
		・ DWG への Read/Writeが可能 
		・ 無償ダウンロード ベース 
	・ AutoCAD 上での認識機能を図面と一緒に流通させる 
	・ .dbx のみあれば良いという考え方 
	・ .dbx 部を Object Enabler（オブジェクト イネーブラ）と呼

	※完全なオブジェクトの実装は相応の労力が必要です !

■■■開発実習応用編
■■L9外部図面アクセス
■外部図面アクセス
・AutoCAD エディタ図面 
	・ AutoCAD エディタ上に表示されて図面データベース化 
	・ AutoCAD によって図面オープンされてメモリ展開 
	・ acdbHostApplicationServices()->workingDatabase()

・外部図面 
	・ AutoCAD エディタに表示せずに図面データベース化 
	・ ObjectARX により動的にメモリ上に展開が可能 
	・ 新規に AcDbDatabase クラス オブジェクトを作成 
	・ 全くの新規状態と既存図面の読み込みの編集が可能

■外部図面データベースの構築

・図面データベースに最低限必須なオブジェクト 
	・ シンボルテーブル 
		・ ブロックテーブル、画層、線種、寸法スタイル、文字スタイル … 
	・ ハードオーナーシップを持つオブジェクト 
		・ モデル空間、ペーパー空間、0 画層、STANDARD 文字スタイル… 
・AcDbDatabase オブジェクトの新規作成時の注意 
	・ new AcDbDatabase にて図面データベース用に使用 
	・ 新規にゼロから構築する場合 
		・ new AcDbDatabase( true ) ：シンボルテーブル等の図面構造は存在 
	・ 後に既存図面ファイルを読み込む場合 
		・ new AcDbDatabase( false ) ：シンボルテーブル等の図面構造がない状態 
		・ 読み込む図面ファイルが構造を保持しているため構築は不要 

・新規にゼロから構築する場合 
	・ 基本構造が存在しなければオブジェクトの追加はできない
・後に既存図面ファイルを読み込む場合 
	・ 図面ファイルが基本構造を持つのでコンフリクトしてしまう

■図面ファイルの読込みと保存

・図面ファイルの図面データベース化 
	・ AcDbDatabase::readDwgFile() 
	・ 図面データベース化後の処理 
		・ オブジェクト操作はカレント図面データベース編集時と対象と同様 
		・ AcDbDatabase* 経由で適切なオブジェクトへアクセス 
		・ 画面に表示されないのでエンティティ選択は不可 
・図面データベースの図面ファイル化 
	・ AcDbDatabase::saveAs() 
・処理終了後は delete でメモリから削除 
	・ ObjectARX アプリが構築した図面データベースは必須 
	・ AutoCAD 上の図面は AutoCAD が削除を担当 

■外部図面操作 コード例
	AcDbDatabase* pDb = new AcDbDatabase( false );
	pDb->readDwgFile( _T("C:\\TEST.dwg“) );
	AcDbBlockTable* pTbl;
	pDb->getBlockTable( pTbl, AcDb::kForRead );
		:
		:
	pTbl->close();
	pDb->saveAs( _T("C:\\TEST.dwg“) );
	delete pDb;

■■L10外部図面への図形書き出し
■特定オブジェクトの図形書き出し
・カレント図面内からの特定オブジェクトの書き出し 
	・ WBLOCK コマンド相当の処理が可能 
	・ AcDbDatabase::wblock() 
	・ 3 つのオーバーロード関数あり 
		・ wblock(AcDbDatabase*&) 
		・ wblock(AcDbDatabase*&, AcDbObjectId) 
		・ wblock(AcDbDatabase*&, const AcDbObjectIdArray&, const AcGePoint3d&) 
		・ wblock (AcDbDatabase*, const AcDbObjectIdArray&, const AcGePoint3d&, AcDb::DuplicateRecordCloning) 
・ AcDbDatabase 領域は wblock() で新規構築される 
	・ new AcDbDatabase での作成は不要

	wblock() は新規に図面データベースを構築するので、
	wblockした後のデータベースに対し、AcDbDatabase::insert()の利用して図面に反映させる。

■オブジェクト ID 配列

・AcDbObjectIdArray クラス 
	・ オブジェクト ID の配列オブジェクト 
	・ 配列へのオブジェクトID の追加 
		・ AcDbObjectIdArray::append() 
	・ 要素数の確認 
		・ AcDbObjectIdArray::length() 
	・ AcArray クラスのテンプレートを使用
・AcArray クラス 
	・ <template> 汎用配列クラス


■■■ｃｈｍファイル■■■
// 線分を作成して、それをモデル空間ブロック テーブル レコードに追加します。
AcDbObjectId createLine() {
    AcGePoint3d startPt(4.0, 2.0, 0.0);
    AcGePoint3d endPt(10.0, 7.0, 0.0);
    AcDbLine *pLine = new AcDbLine(startPt, endPt);

    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    pBlockTable->close();
    AcDbObjectId lineId;
    pBlockTableRecord->appendAcDbEntity(lineId, pLine);
    pBlockTableRecord->close();
    pLine->close();
    return lineId;
}
// 円を作成して、それをモデル空間ブロック テーブル レコードに追加します。
AcDbObjectId createCircle() {
    AcGePoint3d center(9.0, 3.0, 0.0);
    AcGeVector3d normal(0.0, 0.0, 1.0);
    AcDbCircle *pCirc = new AcDbCircle(center, normal, 2.0);

    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    pBlockTable->close();
    AcDbObjectId circleId;
    pBlockTableRecord->appendAcDbEntity(circleId, pCirc);
    pBlockTableRecord->close();
    pCirc->close();
    return circleId;
}
// 画層を作成
void createNewLayer() {
    AcDbLayerTable *pLayerTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable, AcDb::kForWrite);
    AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
    pLayerTableRecord->setName(_T("ASDK_MYLAYER"));

    // 他に指定されていない場合は、レイヤの他のプロパティにデフォルトが使用されます。
    pLayerTable->add(pLayerTableRecord);
    pLayerTable->close();
    pLayerTableRecord->close();
}


// オブジェクトの色を変更する
Acad::ErrorStatus changeColor(AcDbObjectId entId, Adesk::UInt16 newColor) {
    AcDbEntity *pEntity;
    // オブジェクトを修正するには、まずオブジェクトをオープンする。
    // open 関数では、モード パラメータを使用して、読み取り、書き込み、通知のどの目的でオブジェクトをオープンするかを指定。
    acdbOpenObject(pEntity, entId, AcDb::kForWrite);
    pEntity->setColorIndex(newColor);
	// 修正が終わったら、モード関係なく明示的にクローズする。
    pEntity->close();
    return Acad::eOk;
}

// ■グループ ディクショナリへのグループの追加
//createLine() 関数 と createCircle() 関数で作成した線分と円からグループ (pGroup) を作成し、
// そのグループをグループ ディクショナリに追加します。
	void createGroup(AcDbObjectIdArray& objIds, TCHAR* pGroupName) { 
    AcDbGroup *pGroup = new AcDbGroup(pGroupName);

	// 指定されたオブジェクトディレクトリにあるグループディクショナリに格納する
    AcDbDictionary *pGroupDict;
    acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
    AcDbObjectId pGroupId;
    pGroupDict->setAt(pGroupName, pGroup, pGroupId);
    pGroupDict->close();

	// グループが追加されたので、オブジェクトIDがある。
	// グループが追加されたエンティティのための永続的なリアクタとなる(重要)
    for (int i = 0; i < objIds.length(); i++) {
        pGroup->append(objIds[i]);
    }
    pGroup->close();
}


■AutoCADデータベース
データベース
 │
 ├ 画層テーブル─ 図面テーブルレコード(複数)
 │
 ├ ブロックテーブル─ ブロックテーブルレコード(複数)─エンティティ(複数)
 │
 ├ 他のシンボルレコード(複数)─他のシンボルテーブルレコード(複数)
 │
 └ Named Object Dictinary─ オブジェクト(複数)

■ObjectARX アプリケーションを作成するには、次のような手順が必要です。
1.新しいコマンドを実装するカスタムクラスを作成します。
  カスタム クラスは、たいていの ObjectARX 階層およびシンボル テーブル クラスから派生できます。

2.ObjectARX アプリケーションが処理する AutoCAD メッセージを決定します。
  AutoCAD は、AutoCAD 内部に特定のイベントが発生したことを示す多様なメッセージを ObjectARX アプリケーションに送ります。アプリケーションがどのメッセージに応答するか、どのようなアクションを起動するかを特定します。

3.AutoCAD のエントリ ポイントを実装します。
  AutoCAD は、acrxEntryPoint() 関数を使用して ObjectARX アプリケーションを呼び出します。この関数は、C++ プログラムの main() 関数に代わるものです。アプリケーションで acrxEntryPoint() 関数を実装するのは、ユーザの責任になります。acrxEntryPoint() 関数は、これまでに特定の AutoCAD メッセージに関連付けられた関数を呼び出します。

4.初期化を実装します。
  ObjectARX アプリケーション内で、これまでに作成したカスタム クラスを初期化し、ObjectARX のランタイム クラス ツリーを再構築する必要があります。
  また、コマンドを追加する場合は、AutoCAD を使用して登録しなければなりません。

5.ロード解除の準備をします。
正常に動作する ObjectARX アプリケーションを作成するには、
アプリケーションをロード解除する際にカスタム クラスとコマンドを削除しなければなりません。


■AutoCAD メッセージへの応答
？
■ObjectARX アプリケーションでのイベントのシーケンス

図面をオープンしてからアプリケーションをロードすると、
kInitAppMsg メッセージと kLoadDwgMsg メッセージが続けて送られます。
編集セッションの途中で ObjectARX アプリケーションをロード解除すると、
kUnloadDwg メッセージと kUnloadApp メッセージが続けて送られます。

「時間」「イベント」        「メッセージ」
 │                        |
 ├─ 起動                 |kInitAppMsg
 │                        |AcedRegCmds->addCommand("Test2", test1)
 │                        |
 ├─ 図面を開く           |KLoadDwgMsg ads_defun "c:TEST1"
 │                        |
 │                        |
 ├─ test1コマンド呼出し  |kInvkSubr
 │                        |
 │                        |
 ├─ test2コマンド呼出し  |コントロールは直接 "Test2"ルーチンに移る
 │                        |
 │                        |
 ├─ saveコマンド呼出し   |kSaveMsg
 │                        |
 │                        |
 ├─ 終了                 |KUnloadDwgMsg
 │                        |KQuit
 │                        |KUnloadApp
 ↓                        |

■AutoCAD のエントリ ポイントの実装

// acrxEntryPoint() 関数の書式
extern "C"
AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt);
/*
 *  msg        ObjectARX カーネルからアプリケーションに送られるメッセージを示します。
 *  pkt        パケット データ値を保持します。
 *  AppRetCode AutoCAD に返されるステータス コードが格納されます。
 */

// 有効な switch ステートメントのスケルトン
extern "C"
AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
    switch(msg) {
        case AcRx::kInitAppMsg:
            break;
        case AcRx::kUnloadAppMsg:
            break;
        ...
        default:
            break;
    }
    return AcRx::kRetOK;
}

■アプリケーション例
 アプリケーションのロード時とロード解除時に呼び出される関数を実装している。
 その初期化関数は、2 つの新しいコマンド (CREATE と ITERATE) を AutoCAD に追加します。
さらに、新しいクラス AsdkMyClass を初期化し、それを acrxBuildClassHierarchy()により
ObjectARX 階層に追加します。
(AsdkMyClass については、カスタム オブジェクト クラスの例で説明しています。)

// The initialization function called from the acrxEntryPoint()
// function during the kInitAppMsg case is used to add commands
// to the command stack and to add classes to the ACRX class
// hierarchy.
//
void initApp() {
    acedRegCmds->addCommand(_T("ASDK_DICTIONARY_COMMANDS"), _T("ASDK_CREATE"), _T("CREATE"), ACRX_CMD_MODAL, createDictionary);
    acedRegCmds->addCommand(_T("ASDK_DICTIONARY_COMMANDS"), _T("ASDK_ITERATE"), _T("ITERATE"), ACRX_CMD_MODAL, iterateDictionary);
    AsdkMyClass::rxInit();
    acrxBuildClassHierarchy();
}

// The cleanup function called from the acrxEntryPoint() 
// function during the kUnloadAppMsg case removes this application's
// command set from the command stack and removes this application's
// custom classes from the ACRX runtime class hierarchy.
//
void unloadApp() {
    acedRegCmds->removeGroup(_T("ASDK_DICTIONARY_COMMANDS"));

    // Remove the AsdkMyClass class from the ACRX runtime
    // class hierarchy. If this is done while the database is
    // still active, it should cause all objects of class
    // AsdkMyClass to be turned into proxies.
    //
    deleteAcRxClass(AsdkMyClass::desc());
}

■新しいコマンドの登録
■ObjectARX アプリケーションのロード
■アプリケーションのロック解除

■エラー処理
必ず戻り値をチェックして適切なアクションを行う必要があります。
次の例は、データベースの手引きで示した例でのエラー チェックの使用法を示しています。

Acad::ErrorStatus createCircle(AcDbObjectId& circleId) {
    circleId = AcDbObjectId::kNull;
    AcGePoint3d center(9.0, 3.0, 0.0);
    AcGeVector3d normal(0.0, 0.0, 1.0);
    AcDbCircle *pCirc = new AcDbCircle(center, normal, 2.0);
    if (pCirc == NULL)
        return Acad::eOutOfMemory;
    AcDbBlockTable *pBlockTable;
    Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    if (es != Acad::eOk) {
        delete pCirc;
        return es;
    }
    AcDbBlockTableRecord *pBlockTableRecord;
    es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    if (es != Acad::eOk) {
        Acad::ErrorStatus es2 = pBlockTable->close();
        if (es2 != Acad::eOk) {
            acrx_abort(_T("\nApp X failed to close Block Table. Error: %d"), acadErrorStatusText(es2));
        }
        delete pCirc;
        return es;
    }
    es = pBlockTable->close();
    if (es != Acad::eOk) {
        acrx_abort(_T("\nApp X failed to close Block Table. Error: %d"), acadErrorStatusText(es));
    }
    es = pBlockTableRecord->appendAcDbEntity(circleId, pCirc);
    if (es != Acad::eOk) {
        Acad::ErrorStatus es2 = pBlockTableRecord->close();
        if (es2 != Acad::eOk) {
            acrx_abort(_T("\nApp X failed to close Model Space Block Record. Error: %s"), acadErrorStatusText(es2));
        }
        delete pCirc;
        return es;
    }
    es = pBlockTableRecord->close();
    if (es != Acad::eOk) {
        acrx_abort(_T("\nApp X failed to close Model Space Block Record. Error: %d"), acadErrorStatusText(es));
    }
    es = pCirc->close();
    if (es != Acad::eOk) {
        acrx_abort(_T("\nApp X failed to close circle entity. Error: %d"), acadErrorStatusText(es));
    }
    return es;
}
Acad::ErrorStatus createNewLayer() {
    AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
    if (pLayerTableRecord == NULL)
        return Acad::eOutOfMemory;
    Acad::ErrorStatus es = pLayerTableRecord->setName(_T("ASDK_MYLAYER"));
    if (es != Acad::eOk) {
        delete pLayerTableRecord;
        return es;
    }
    AcDbLayerTable *pLayerTable;
    es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable, AcDb::kForWrite);
    if (es != Acad::eOk) {
        delete pLayerTableRecord;
        return es;
    }
    // The linetype object ID default is 0, which is
    // not a valid ID.  Therefore, it must be set to a
    // valid ID, the CONTINUOUS linetype.
    // Other data members have valid defaults, so
    // they can be left alone.
    //
    AcDbLinetypeTable *pLinetypeTbl;
    es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
    if (es != Acad::eOk) {
        delete pLayerTableRecord;
        es = pLayerTable->close();
        if (es != Acad::eOk) {
            acrx_abort(_T("\nApp X failed to close Layer Table. Error: %d"), acadErrorStatusText(es));
        }
        return es;
    }
    AcDbObjectId ltypeObjId;
    es = pLinetypeTbl->getAt(_T("CONTINUOUS"), ltypeObjId);
    if (es != Acad::eOk) {
        delete pLayerTableRecord;
        es = pLayerTable->close();
        if (es != Acad::eOk) {
            acrx_abort(_T("\nApp X failed to close Layer Table. Error: %d"), acadErrorStatusText(es));
        }
        return es;
    }
    pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
    es = pLayerTable->add(pLayerTableRecord);
    if (es != Acad::eOk) {
        Acad::ErrorStatus es2 = pLayerTable->close();
        if (es2 != Acad::eOk) {
            acrx_abort(_T("\nApp X failed to close Layer Table. Error: %d"), acadErrorStatusText(es2));
        }
        delete pLayerTableRecord;
        return es;
    }
    es = pLayerTable->close();
    if (es != Acad::eOk) {
        acrx_abort(_T("\nApp X failed to close Layer Table. Error: %d"), acadErrorStatusText(es));
    }
    es = pLayerTableRecord->close();
    if (es != Acad::eOk) {
        acrx_abort(_T("\nApp X failed to close Layer Table Record. Error: %d"), acadErrorStatusText(es));
    }
    return es;
}

■データベース操作
9 つのシンボル テーブルのセット 
ブロック テーブル           │(AcDbBlockTable)
寸法スタイル テーブル       │(AcDbDimStyleTable)
画層テーブル                │(AcDbLayerTable)
線種テーブル                │(AcDbLinetypeTable)
登録アプリケーションテーブル│(AcDbRegAppTable)
文字スタイル テーブル       │(AcDbTextStyleTable)
ユーザ座標系テーブル        │(AcDbUCSTable)
ビューポート テーブル       │(AcDbViewportTable)
ビュー テーブル             │(AcDbViewTable)

・シンボル テーブルは、初めから 1 つまたは複数のレコードが入っているものもあります。
・初期状態のデータベースにある画層テーブルは、1 つのレコード (画層 0) を含んでいます。
・初期状態のブロック テーブルには、すでに 3 つのレコードが入っています。
	(*MODEL_SPACE、*PAPER_SPACE、*PAPER_SPACE0) 
・線種テーブルには常に、CONTINUOUS、BY_LAYER、BY_BLOCK の線種テーブル レコードが入っています。
・登録アプリケーション テーブルには、常に ACAD テーブル レコードがあります。
・文字スタイル テーブルには常に STANDARD テーブル レコードがあります。

・Named Object Dictionary。データベースを作成した時点で、
このディクショナリには既に 2 つのデータベース ディクショナリが入っています。
GROUP ディクショナリと MLINE スタイル ディクショナリです。
・MLINE スタイル ディクショナリには、STANDARD スタイルが常に存在します。
ヘッダー変数の固定セット (データベース オブジェクトはありません)。


■既存データベースからの新しいデータベースの作成
//WBLOCK* コマンドに相当します。
	Acad::ErrorStatus AcDbDatabase::wblock(AcDbDatabase*& pOutputDb);

■エンティティのある新しいデータベースの作成
//名前の付いたブロックのコピー
Acad::ErrorStatus AcDbDatabase::wblock(
	AcDbDatabase*& pOutputDb, AcDbObjectId blockId);

	blockId 引数は、入力データベースにあるブロック テーブル レコードを表します。
	このブロック テーブル レコードにあるエンティティが、
	新しいデータベースのモデル空間ブロック テーブル レコードにコピーされます。
	新しいデータベースの挿入基点は、ブロック テーブル レコードの原点です。

//エンティティの配列のコピー
次の関数は、WBLOCK コマンドを呼び出した後で、オプションを使用して特定のオブジェクトを選択し、
挿入基点を指定するケースに相当します。
Acad::ErrorStatus AcDbDatabase::wblock(
    AcDbDatabase*& pOutputDb,
    const AcDbObjectIdArray& outObjIds,
    const AcGePoint3d* basePoint);

	この関数は、outObjIds 引数で指定されたエンティティを含む新しいデータベースを作成します。
	指定されたエンティティ(入力データベースのモデル空間またはペーパー空間の
	どちらのブロック テーブル レコードのエンティティでもかまいません)は、
	新しいデータベースのモデル空間に置かれます。
	新しいデータベースにはそのほかに、これらのエンティティが所有または参照しているオブジェクトや、
	それらのオブジェクトを所有しているオブジェクトも取り込まれます。
	指定された点は、新しい図面のワールド座標の原点
	(つまり、新しいデータベースのモデル空間での挿入基点)です。

AcDbDatabase::wblock() 関数の最終バージョンも、エンティティの配列をコピーし、
挿入基点を指定しますが、新しいデータベースの重複シンボルやディクショナリ エントリの
処理方法を指定する追加パラメータがあります。

Acad::ErrorStatus
AcDbDatabase::wblock(
    AcDbDatabase*& pOutputDb,
    const AcDbObjectIdArray& outObjIds,
    const AcGePoint3d* basePoint,
    AcDb::DuplicateRecordCloning drc);

■データベースの挿入
// 標準図面の INSERT[挿入]コマンドと同等です。
Acad::ErrorStatus
AcDbDatabase::insert(
    AcDbObjectId& blockId,
    const ACHAR* pBlockName,
    AcDbDatabase* pDb,
    bool preserveSourceDatabase = true);

■現在のデータベース値の設定
1.色
エンティティの色を指定しないと、CECOLOR システム変数に保存されている、
データベースの現在の色の値が使われます。
次の関数は、データベースにある現在の色の値を設定して取り出します。

Acad::ErrorStatus AcDbDatabase::setCecolor(const AcCmColor& color);
AcCmColor AcDbDatabase::cecolor() const;

2.線種
次の関数は、データベースにある現在の線種の値を設定、取得します。

Acad::ErrorStatus AcDbDatabase::setCeltype(AcDbObjectId objId);
AcDbObjectId AcDbDatabase::celtype() const;

3.線種尺度
データベースには 3 つの線種尺度設定があります。

・現在のエンティティについての線種尺度設定。CELTSCALE システム変数に保存される。
・現在の図面についての線種尺度設定。LTSCALE システム変数に保存される。
・線種尺度を、エンティティが入っている空間に適用するか、ペーパー空間に表れるエンティティの外観に適用するかを指示するフラグ。この設定は PSLTSCALE システム変数に保存される。

図面を再作成するときは、LTSCALE と PSLTSCALE のグローバル設定が使用されます
(詳細は、エンティティを参照してください)。
これらの値の設定と照会を行うには、次の関数を使用します。

Acad::ErrorStatus AcDbDatabase::setLtscale(double scale);
double AcDbDatabase::ltScale() const;
 
Acad::ErrorStatus AcDbDatabase::setCeltscale(double scale);
double AcDbDatabase::celtscale() const;
 
Acad::ErrorStatus AcDbDatabase::setPsltscale(bool scale)
bool AcDbDatabase::psltscale() const;

4.画層
次の関数は、データベースにある現在の画層の値を設定、取得します。

Acad::ErrorStatus AcDbDatabase::setClayer(AcDbObjectId objId);
AcDbObjectId AcDbDatabase::clayer() const;

■データベース操作の例
次の例は、createDwg() ルーチンを表していますが、
このルーチンは新しいデータベースを作成し、
モデル空間ブロック テーブル レコードを取得し、
モデル空間に追加される 2 つの円を作成します。
これは AcDbDatabase::saveAs() 関数を使用して図面を保存します。

2 番目のルーチンである readDwg() は、保存された図面を読み込み、
モデル空間ブロック テーブル レコードをオープンし、それを走査、
中に含まれるエンティティのクラス名を出力します。

void createDwg() {
	AcDbDatabase *pDb = new AcDbDatabase();

	AcDbBlockTable *pBtbl;
	pDb->getSymbolTable(pBtbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBtblRcd;
	pBtbl->getAt(ACDB_MODEL_SPACE, pBtblRcd, AcDb::kForWrite);   pBtbl->close();

	pBtblRcd->appendAcDbEntity(pCir1);
	pCir1->close();
	pBtblRcd->appendAcDbEntity(pCir2);
	pCir2->close();
	pBtblRcd->close();

	// AcDbDatabase::saveAs() does not automatically
	// append a DWG file extension, so it
	// must be specified.

	pDb->saveAs(_T("c:\\test1.dwg"));
	delete pDb;
}

void readDwg(){
	// Set constructor parameter to kFalse so that the
	// database will be constructed empty.  This way only
	// what is read in will be in the database.
	AcDbDatabase *pDb = new AcDbDatabase(Adesk::kFalse);
	// The AcDbDatabase::readDwgFile() function
	// automatically appends a DWG extension if it is not
	// specified in the filename parameter.
	if(Acad::eOk != pDb->readDwgFile(_T("c:\\test1.dwg")))
		return;
	// Open the model space block table record.
	AcDbBlockTable *pBlkTbl;
	pDb->getSymbolTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlkTblRcd->newIterator(pBlkTblRcdItr);
	AcDbEntity *pEnt;
	{
	  pBlkTblRcdItr->getEntity(pEnt, AcDb::kForRead);
	  acutPrintf(_T("classname: %s\n"), (pEnt->isA())->name());
	  pEnt->close();
	}
	pBlkTblRcd->close();
	delete pBlkTblRcdItr;
	delete pDb;
}

■ロング トランザクション
ロング トランザクションは、AutoCAD 参照編集機能をサポートする際に使用される
■外部参照
外部参照 (xrefs) は、いくつかのグローバル関数を使用して作成し操作することができます。
■インデックスとフィルタ
インデックスとフィルタのクラスおよび関数は、
ブロック データのカスタム インデックスとカスタム フィルタを定義するための機構を
アプリケーションに提供します。
■ファイルの概要情報
AutoCAD ユーザは、[図面のプロパティ]ダイアログ ボックスを使用して、
補助データ (ファイルの概要情報と呼ばれる) を DWG ファイルに埋め込むことができます

■■データベース操作
データベースの作成、図面ファイルの読み込み、データベースの保存の方法などを含む、
基本的なデータベース プロトコルについて説明します。
wblock と insert のオペレーションについても説明します。

■オブジェクトのオープンクローズ
+-------------+
|DWG ハンドル |
+-------------+
Save または↑│図面をオープン
Wblock     ││
コマンド   │↓
		+----------------+
		|AcDbDatabase    |
		| オブジェクトID |
		+----------------+
			オブジェクト ↑│オブジェクト
			をクローズ   ││をオープン
					     │↓
					+----------+
					|C++       |
					|ポインタ  |
					+----------+

1.オブジェクトをオープンする
	・acdbOpenObject グローバル関数を使用します。
	・getAcDbObjectId 関数を使用して、オブジェクト ID にハンドルをマップすることができます。
	・オブジェクトをオープンしてそのハンドルを要求することもできます。

	AcDbObject* pObject;
	AcDbHandle handle;
	pObject->getAcDbHandle(handle);

	※注: データベース オブジェクトをオープンした場合、
	必ずそれをできるだけ早い機会にクローズする。
	AcDbObject::close() 関数を使用する。


	ads_name は AcDbObjectId に相当します。
	AcDb ライブラリには 2 つのスタンドアロン関数があり、
	それらを使用して AcDbObjectId と ads_name との間での変換ができます。

	//ads_name ⇔ AcDbObjectId
	acdbGetAdsName(ads_name& objName, AcDbObjectId objId); 
	acdbGetObjectId(AcDbObjectId& objId, ads_name objName); 


一般に、選択子の中からオブジェクトを取得すると、そのオブジェクトが ads_name 書式で返されます。
次に ads_name を AcDbObjectId と交換して、それをオープンする必要があります。


AcDbEntity* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode) {
    ads_name en;
    ads_point pt;
    acedEntSel(_T("\nエンティティを選択してください: "), en, pt);

    // ads_nameとObjectIDを変換する
    acdbGetObjectId(eId, en);
    AcDbEntity * pEnt;
    acdbOpenObject(pEnt, eId, openMode);
    return pEnt;
}

・openMode
次の 3 通りのモードでオブジェクトをオープンすることができます。
	1.kForRead
		既に書き込み用または通知用にオープンされている場合を除いて、
		オブジェクトは同時に 256 まで読み込み用にオープンできます。
	2.kForWrite
		既にオープンされている場合を除いて、
		オブジェクトを書き込み用にオープンできます。
		既にオープンされている場合は、オープンできません。
	3.kForNotify
		オブジェクトがクローズしている場合や、読み込み用にオープンされている場合、
		または書き込み用にオープンされている場合は、
		オブジェクトを通知用にオープンすることができます。
		ただし既に通知用にオープンされている場合はオープンできません。通知を参照してください。
		アプリケーションがオブジェクトを通知用にオープンしてオブジェクトに通知を
		送る必要があるのはごくまれです。

・帰ってきたエラーコードは図を参照

■オブジェクトの削除
AcDbObject オブジェクトのインスタンスを作成するときは、AcDbObject::new() 関数を使用します。

オブジェクトを最初に作成してデータベースに追加する前であれば、削除できます。
ただし、データベースに追加してしまったオブジェクトは、削除できません。
データベース常駐のオブジェクトの削除は、AutoCAD が管理します。

■オブジェクト固有データの追加
	AcDbObject::xData() 関数を使用して、

	オブジェクトの Xdata のコピーを含んだ resbuf チェーンを取得します。
	virtual resbuf* AcDbObject::xData(const ACHAR* regappName = NULL) const;

	AcDbObject::setXData() 関数を使用して、オブジェクトの Xdata を指定します。
	virtual Acad::ErrorStatus AcDbObject::setXData(const resbuf* xdata);

	// xData() を使用して選択されたオブジェクトの Xdata を取得し、
	// その Xdata を画面に出力します。
	// 次に、Xdata に文字列 (testrun) を追加して、
	// オブジェクトの Xdata を修正するために setXdata() を呼び出します。
	// さらに、この例は upgradeOpen() 関数と downgradeOpen() 関数の使用法も示しています。

// This function calls the selectObject() function to allow 
// the user to pick an object; then it accesses the xdata of 
// the object and sends the list to the printList() function 
// that lists the restype and resval values.
// 
void printXdata() {
    // オブジェクトを選択して開く
    AcDbObject *pObj;
    if ((pObj = selectObject(AcDb::kForRead)) == NULL) {
        return;
    }

    // 拡張データのアプリケーション名を取得します。
    //
    TCHAR appname[133];
    if (acedGetString(NULL, _T("\n目的のXdataAPP名を入力せよ: "), appname) != RTNORM) {
        return;
    }

    // アプリケーション名の拡張データを取得する
    struct resbuf *pRb;
    pRb = pObj->xData(appname);
    if (pRb != NULL) {
		// 存在する場合は、既存のxdataを出力します。
		// LISPには-3グループがないことに注意してください。
		// これはxdataだけなので、-3 xdata-start markerは必要ありません。
        printList(pRb);
        acutRelRb(pRb);

    } else {
        acutPrintf(_T("\n該当するAPP名に拡張データはありません"));
    }
    pObj->close();
}

void addXdata() {
	AcDbObject* pObj = selectObject(AcDb::kForRead);
	if (!pObj) {
		acutPrintf(_T("Error selecting object\n"));
		return;
	}
	// Get the application name and string to be added to
	// xdata.
	//
	TCHAR appName[132], resString[200];
	appName[0] = resString[0] = _T('\0');
	acedGetString(NULL, _T("Enter application name: "), appName);
	acedGetString(NULL, _T("Enter string to be added: "), resString);
	struct  resbuf  *pRb, *pTemp;
	pRb = pObj->xData(appName);
	if (pRb != NULL) {
		// If xdata is present, then walk to the
		// end of the list.
		for (pTemp = pRb; pTemp->rbnext != NULL; pTemp = pTemp->rbnext) { ; }
	}
	else {
		// If xdata is not present, register the application
		// and add appName to the first resbuf in the list.
		// Notice that there is no -3 group as there is in
		// AutoLISP. This is ONLY the xdata so
		// the -3 xdata-start marker isn't needed.
		//
		acdbRegApp(appName);
		pRb = acutNewRb(AcDb::kDxfRegAppName);
		pTemp = pRb;
		const size_t nSize = _tcslen(appName) + 1;
		pTemp->resval.rstring = (TCHAR*) malloc(nSize * sizeof(TCHAR));
		errno_t err = _tcscpy(pTemp->resval.rstring, appName);
		assert(err == 0)
	}
	// Add user-specified string to the xdata.   
	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;
	const size_t nSize = _tcslen(resString) + 1;
	pTemp->resval.rstring = (TCHAR*) malloc(nSize * sizeof(TCHAR));
	errno_t err = _tcscpy_s(pTemp->resval.rstring, nSize, resString);
	assert(err == 0)
	// The following code shows the use of upgradeOpen()   
	// to change the entity from read to write.   
	pObj->upgradeOpen();
	pObj->setXData(pRb);

	pObj->close();
	acutRelRb(pRb);
}

■ObjectARX 拡張ディクショナリの例
■グローバル関数での拡張ディクショナリの例

■オブジェクトの削除
データベース内のオブジェクトは、次の関数を使用して削除できます。

Acad::ErrorStatus AcDbObject::erase( Adesk::Boolean Erasing = true);

注:
	erase() 関数には、データベース オブジェクトとエンティティに対しての、
	削除と復活の 2 つの機能があります。
	データベース オブジェクトが削除されたとき、ディクショナリからオブジェクトについての情報も削除されます。
	オブジェクトが erase(kfalse) で復活しても、情報は自動では復活しません。
	setAt() 関数を使用して、再びディクショナリに追加する必要があります。
	エンティティが削除されたとき、ブロック テーブル レコードに削除フラグが設定されます。
	エンティティは erase(kfalse) で復活することができます。
	既定では、acdbOpenObject() 関数で削除されたオブジェクトをオープンすることはできません。
	もし、試みると eWasErased エラーコードが返されます。

	extern Acad::ErrorStatus
	acdbOpenObject(
	    AcDbObject*& obj,
	    AcDbObjectId objId,
	    AcDb::OpenMode openMode,
	    Adesk::Boolean openErasedObject = Adesk::kFalse);

	acdbOpenObject() 関数の最後の引数を kTrue に設定すると、
	削除されたオブジェクトをオープンできます。

	ポリラインやブロック テーブル レコードのようなコンテナ オブジェクトは通常、
	イテレイト (走査) を行う際に削除済み要素を無視するオプションを提供します。
	既定値では、削除されている要素は無視します。

	削除されたオブジェクトは DWG ファイルにも DXF ファイルにもファイル アウトされません。

■埋め込みオブジェクトとカプセル化オブジェクト

■エンティティ
	エンティティとは、グラフィック表現を持つデータベース オブジェクトです。
	すべてのエンティティが共通して持っているプロパティとオペレーションのリストも掲載します。
	その他、ブロック、挿入、複合エンティティの作成方法と、
	サブエンティティを選択してハイライト表示する方法の例を取りあげます。

■エンティティ定義
	エンティティの例としては、線分、円、円弧、文字、ソリッド、リージョン、スプライン、楕円があります。
	AcDbEntity クラスは、AcDbObject から派生しています。
	完全なクラス階層図は、ObjectARX の classmap フォルダの classmap.dwg
	を参照してください。

	いくつかの例外を除き、エンティティにはジオメトリについての必要情報がすべて含まれています。
	いくつかのエンティティは、ジオメトリ情報または属性の入った他のオブジェクトを含んでいます。
	複合エンティティは次のとおりです。

	AcDb2dPolyline     (AcDb2dVertex オブジェクトを所有)
	AcDb3dPolyline     (AcDb3dPolylineVertex オブジェクトを所有)
	AcDbPolygonMesh    (AcDbPolygonMeshVertex オブジェクトを所有)
	AcDbPolyFaceMesh   (AcDbPolyFaceMeshVertex オブジェクトと AcDbFaceRecord オブジェクトを所有)
	AcDbBlockReference (AcDbAttribute オブジェクトを所有)
	AcDbMInsertBlock   (AcDbAttribute オブジェクトを所有)

■エンティティ オーナーシップ
	データベース内のエンティティは、通常 AcDbBlockTableRecord に属しています。
	新しく作成されたデータベース内のブロック テーブルには、
		*MODEL_SPACE、*PAPER_SPACE、*PAPER_SPACE0
	の 3 つの定義済みレコードがあります
	(1 つはモデル空間を示し、あとの 2 つは定義済みのペーパー空間レイアウトを示します)。
	一般に BLOCK[ブロック登録]、HATCH[ハッチング]、または DIM[寸法記入]コマンドを実行して
	ユーザが新しいブロック (ブロック レコード) を作成すると、レコードが追加されます。

	データベース エンティティのオーナーシップ構造は次のとおりです。

	AcDbDatabase
	│
	└AcDbBlockTable
	  │
	  └AcDbBlockTableRecord(複数)
	    │
	    ├AcDbBlockBegin
	    │
	    ├AcDbEntity(複数)
	    │ │
	    │ ├ AcDbxxxVertexまたは、AcDbFaceRecordまたは、AcDbAttribute(複数)
	    │ │
	    │ └ AcDbDequenceEnd
	    │ 
	    └AcDbBlockEnd

■共通エンティティプロパティ
	ブロック テーブル レコードにエンティティを追加すると、
	AutoCAD は自動的に AcDbEntity::setDatabaseDefaults() 関数を呼び出します。
	この関数は、プロパティが明示的に指定されていない場合に既定値に設定します。

	1.色
		正しい色インデックスは、AcCmColor::getColorIndex() メンバ関数を使用して
		AcCmColor のインスタンスから取得。
		
		1~7：標準色
		 8~255：ディスプレイ装置によって定義
		 0   BYBLOCK
		 256 BYLAYER
		 257 無色。この値が存在するのは、
		     エンティティの最初のインスタンス生成から、0 ～ 256 の値で色を設定するまでの間、または
		     エンティティをデータベースに追加して、データベースの現在の色インデックスを受け継ぐときまでの間だけ

	2.線種
	エンティティを追加するデータベースの AcDbLineTypeTableRecord のオブジェクト ID で指定できます。
	特別な線種項目です。
	CONTINUOUS  線種シンボル テーブルで自動的に作成される、既定値の線種
	BYLAYER     エンティティの画層の線種値
	BYBLOCK     エンティティの周囲ブロック定義の現在のブロック参照の線種値

	setLinetype 関数
		名前またはオブジェクト ID でエンティティの線種を設定
	linetype 関数
		現在のエンティティの線種の名前を返す。
	linetypeId 関数
	線種を指定するシンボル テーブル レコードのオブジェクト ID を返す


	3.線種尺度
	・エンティティごとに指定される線種尺度
		setLinetypeScale 関数
		linetypeScale 関数
			エンティティの線種尺度を設定および照会
	4.表示
	・エンティティごとに表示/非表示を設定
		setVisibility 関数
		visibility 関数
			エンティティの表示を設定および照会
	5.画層
	setLayer 関数
		名前またはオブジェクト ID によりエンティティの画層を設定できます。
	layer 関数
		現在のエンティティの画層名を返します。
	layerId 関数
		現在の画層(タイプ AcDbLayerTableRecordのオブジェクト)のオブジェクト ID を返します。

■共通のエンティティ関数
	intersectWith() は、トリミング、延長、フィレット、面取り、部分削除、オブジェクト スナップ交差オペレーションに使用します。
	transformBy() は、オブジェクト内の点を、移動、尺度変更、または回転させる変換マトリックスを渡すために使用します。
	getTransformedCopy() は、オブジェクトのコピーを作成して、それに変換を適用します。
	getOsnapPoints() は、スナップ点およびスナップ点の種類を返します。
	getGripPoints() は、ストレッチ点のスーパーセットであるグリップ点を返します。
	getStretchPoints() は、getGripPoints() の既定値を取り、通常は同じ実装を持っています。
	moveStretchPointsAt() は、AutoCAD の STRETCH[ストレッチ]コマンドで使用して、指定された点を移動します。これは、transformBy() の既定値を取ります。
	moveGripPointsAt() は、AutoCAD グリップ編集で使用して、指定された点を移動します。これは、transformBy() の既定値を取ります。
	worldDraw() は、エンティティの表示に依存しないジオメトリック表示を作成します。
	viewportDraw() は、エンティティの表示に依存するジオメトリック表示を作成します。
	draw() は、エンティティとキュー内のすべてのものが描かれるように、エンティティをキューに入れてグラフィック キューをフラッシュします。
	list() は AutoCAD の LIST[オブジェクト情報]コマンドで使用され、acutPrintf() 文を作成します。
	getGeomExtents() は、エンティティの 3D 表示範囲を囲む直方体のコーナー点を返します。
	explode() はエンティティを分解して、単純な要素のセットにします。
	getSubentPathsAtGsMarker() は、GS マーカーと対応するサブエンティティ パスを返します(GS マーカーとサブエンティティを参照)。
	getGsMarkersAtSubentPath() は、サブエンティティ パスに対応する GS マーカーを返します。
	subentPtr() は、対応するサブエンティティ パスを返します。
	highlight() は、指定されたサブエンティティをハイライト表示します (GS マーカーとサブエンティティを参照)。

■オブジェクト スナップ ポイント
	オブジェクトには、中心点、中点、端点などの特有の点を定義することができます。
	AutoCAD がスナップモードで点を取得している場合、
	指定されたオブジェクト スナップ モードに関連するスナップ点を取得するために 
	getOsnapPoints()関数を呼び出します。

	オブジェクトスナップ モード
	モード      │説明
	----------------------------------
	kOsModeEnd  │端点
	kOsModeMid  │中点
	kOsModeCen  │中心
	kOsModeNode │点
	kOsModeQuad │四半円点
	kOsModeIns  │挿入基点
	kOsModePerp │垂線
	kOsModeTan  │接線
	kOsModeNear │近接点

■変換関数
	AcDbEntity クラスは、次の 2 つの変換関数を提供します。
	・transformBy() 関数
		指定されたマトリックスを使用してエンティティを修正します。
		AutoCAD では、グリップでの移動、回転、尺度変更、および鏡像化の各モードにより呼び出されます。
	・getTransformedCopy() 関数
		作成されるエンティティが元のエンティティとは別のクラスのインスタンスを作成できるようにします

	・尺度の一致しないブロック参照を分解すると、ブロック参照の中のエンティティで getTransformedCopy() 関数が呼び出され、
	  新しいエンティティを作成します (エンティティの分解を参照)。

■交点
	・intersectWith() 関数
		図面の中で、あるエンティティが別のエンティティと交差する点を返します。
		この関数への入力値はエンティティと交差タイプで、次のいずれかです。

		・kOnBothOperands (どちらのエンティティも延長しない)
		・kExtendThis 
		・kExtendArg 
		・kExtendBoth 

		例；次の図のように、図面に 3 本の線分が引かれているとします。
		Line 1 が「基準となる線分」でLine 3 が引数エンティティです。

		交差タイプが kExtendThis の場合、
			線分 1 を延長したときに線分 1 (「基準となる線分」) と線分 3 が交差する点として、点 A が返されます。

		交差タイプが kExtendArgument で線分 2 が引数エンティティの場合、
			線分 2 を延長しても線分 1 と交わらないので、データは返されません。

		交差タイプが kExtendBoth で、線分 2 が引数エンティティの場合は、
			点 B が返されます。
		交差タイプが kExtendNone で線分 2 が引数エンティティの場合は、
			データは返されません。

			              │
			              │
			      ・B     ・A
			    ／        │
			  ／  │      │
			／    │      │
			Line1 │      │
			      Line2   │Line3


	intersectWith() 関数は、2 つの書式を持つオーバーロードされた関数です。
	2 番目の 書式ではさらに別の引数も使用しますが、その引数は 2 つのエンティティの仮想交点を決定するための投影面です。

	virtual Acad::ErrorStatus AcDbEntity::intersectWith(
	    const AcDbEntity* ent,
	    AcDb::Intersect   intType,
	    AcGePoint3dArray& points,
	    int               thisGsMarker = 0,
	    int               otherGsMarker = 0) const;
	 

	virtual Acad::ErrorStatus AcDbEntity::intersectWith(
	    const AcDbEntity* ent,
	    AcDb::Intersect   intType,
	    const AcGePlane&  projPlane,
	    AcGePoint3dArray& points,
	    int               thisGsMarker = 0,
	    int               otherGsMarker = 0) const;

	返される点は常にエンティティ (「基準となる線分」) 上にあります。
	したがって仮想交点の場合は、交差した点は、返される前にエンティティに投影されます。

	どちらの書式の intersectWith() でも、この関数のパフォーマンスを最適化するためにオプションの GS マーカーを使用できます。
	このエンティティの intersectWith() 関数を GS マーカーを使用して実装した場合、
	GS マーカーによって、交差領域の位置を特定してテストの速度を上げることができます。
	たとえば次の図面で、ユーザがポリゴンの 1 辺を選択して、その辺についての GS マーカーを渡すと、
	ポリゴンの他の 5 辺をテストする必要がなくなります。


		     ／＼
		   ／    ＼／    
		 ／      ／＼←この交点を選択した場合 
		│     ／    ＼
		│   基準     │
		│   線分     │
		│            │
		└──────┘

■GS(グラフィックス システム) マーカーとサブエンティティ
	・スナップマーカー

■サブエンティティ パス
サブエンティティ パスは、図面の特定エンティティの中にあるサブエンティティを識別するためのパスで、
	同じ名前のものがあってはなりません。
このクラス AcDbFullSubentPath のパスは、
	・オブジェクト ID の配列 1 つ
	・サブエンティティ ID オブジェクト 1 つでできています。
		{AcDbObjectIdArray  mObjectIds;
		 AcDbSubentId       mSubentId;
		}

配列には、「メイン」エンティティへのパスを指定するオブジェクト ID が含まれています。
たとえばブロック参照が、それぞれタイプの AcDb3dSolid 2 つの直方体を含んでいるとします。
オブジェクト ID 配列には 2 つのエンティティ
(ブロック参照の ID と、その後にメイン エンティティの ID[InsertID, SolidID]) が含まれています。

AcDbFullSubentPath の 2 番目の要素は AcDbSubentId オブジェクトで、
これはリスト内にサブエンティティ タイプ (頂点、エッジ、面) とサブエンティティの索引が入っています。
メンバ データにアクセスするには、AcDbSubentId 関数の type() と index() を使用します。

前の例を使うと、solid1 の 2 番目のエッジの AcDbFullSubentPath は次のようになります。
{(InsertID, solid1ID)
 (kEdgeSubentType, 2)};
ソリッドのみがある場合、AcDbFullSubentPath は、ソリッドの最初の面では次のようになります。


■簡単なハイライト表示の例
	・サブエンティティをハイライト表示するには
	1.選択リストから、選択されたエンティティの GS マーカーを取得。
	2.getSubentPathsAtGsMarker() 関数を使用して、サブエンティティ パスに変換するエンティティ クラスに GS マーカーを渡します。
	  希望のサブエンティティ タイプ (頂点、エッジ、面) を指定してください。
	3.選択したサブエンティティへのパスができたら、正しいサブエンティティパスで渡して
	  highlight() 関数を呼び出せるようになります。


■エンティティの選択
 	選択するためには、グローバル関数を組み合わせて使用します。
	まずacedSSGet() 関数を使用して選択セットを取得します。
	次に acedSSNameX() 関数を使用して、選択されたエンティティのためのサブエンティティ GS マーカーを取得します。
	int acedSSGet(
	    const ACHAR * str,
	    const void * pt1,
	    const void * pt2,
	    const struct resbuf * filter,
	    ads_name ss);
	 
	int acedSSNameX(
	    struct resbuf ** rbpp,
	    const ads_name ss,
	    const long i);

■GS マーカーのサブエンティティ パスへの変換
	getSubentPathsAtGsMarker() 関数を使用して、acedSSNameX() により返される
	GS マーカーのためのサブエンティティを取得します。この関数の完全な構文は次のとおりです。

	virtual Acad::ErrorStatus AcDbEntity::getSubentPathsAtGsMarker(
	    AcDb::SubentType type,
	    int gsMark,
	    const AcGePoint3d& pickPoint,
	    const AcGeMatrix3d& viewXform,
	    int& numPaths,
	    AcDbFullSubentPath*& subentPaths
	    int numInserts = 0,
	    AcDbObjectId* entAndInsertStack = NULL) const;

	この関数への最初の引数は、対象となるサブエンティティ タイプ(頂点、エッジ、または面)です。
	サブエンティティのハイライト表示に記載したコード例では、対応するエッジをハイライト表示しようとしているため、
	この関数への最初の呼び出しでは kEdgeSubentType が指定されています。
	getSubentPathsAtGsMarker() 関数への 2 番目の呼び出しでは、
	選択されたサブエンティティに関連する各面をハイライト表示しようとしているので、kFaceSubentType が指定されています。

	一部のエンティティ (マルチラインなど) で、GS マーカーだけではサブエンティティパスを返すための
	十分な情報を提供できない場合、追加の情報として pickPoint と viewXform 引数が使われます。
	サブエンティティのハイライト表示に記載したコード例では使用されていません。

	numInserts と entAndInsertStack の引数は、ネストされた挿入で使用します。
	acedNEntSel() と acedNEntSelP() の関数は、どちらもリーフ レベル エンティティと、挿入のスタックを返します。

■サブエンティティのハイライト表示
	あとは highlight() 関数を呼び出してサブエンティティ パスに渡します。
	引数を指定せずに highlight() を呼び出すと、既定値によりエンティティ全体がハイライト表示されます。

	次のサンプル コードからは、エンティティの選択、サブエンティティ パスの取得、
	および GS マーカーと関連する異なるタイプのサブエンティティのハイライト表示を行う手順を示しています。
	また、その他の便利なサブエンティティ関数も示しています。

	virtual AcDbEntity* 
	AcDbEntity::subentPtr(const AcDbFullSubentPath& id) const;

	この関数は、指定されたパスにより記述されるサブエンティティのコピーにポインタを返します。
	これはその後(例にあるように)データベースに追加することができます。

	注:AcDbEntity の新しいサブクラスを作成するときは、
	getSubentPathsAtGsMarker()、getGsMarkersAtSubentPath()、および subentPtr() の
	それぞれに対応する仮想関数 subGetSubentPathsAtGsMarker()、subGetGsMarkersAtSubentPath()、および subSubentPtr() を
	オーバーライドすることによって、独自の実装を提供する必要があります(AcDbEntity からの派生を参照)。
	
	しかし highlight() 関数は AcDbEntity レベルで実装され、通常は上書きする必要はありません。
	
	ただし、上書きした場合、新しく実装されたこの関数は、ハイライト表示を行うために 
	AcDbEntity::highlight() を呼び出さなければなりません。

	// This function calls getObjectAndGsMarker() to get the
	// object ID of a solid and its gsmarker. It then calls
	// highlightEdge(), highlightFaces(), and highlightAll() to
	// highlight the selected edge, all faces surrounding that
	// edge, and then the whole solid.
	//
	void highlightTest() {
	    AcDbObjectId objId;
	    int marker;
	    if (getObjectAndGsMarker(objId, marker) != Acad::eOk)
	        return;
	    highlightEdge(objId, marker);
	    highlightFaces(objId, marker);
	    highlightAll(objId);
	}
	// This function uses acedSSGet() to let the user select a
	// single entity.  It then passes this selection set to
	// acedSSNameX() to get the gsmarker. Finally, the entity name
	// in the selection set is used to obtain the object ID of
	// the selected entity.
	//
	Acad::ErrorStatus getObjectAndGsMarker(AcDbObjectId& objId, int& marker) {
	    ads_name sset;
	    if (acedSSGet(_T("_:S"), NULL, NULL, NULL, sset) != RTNORM) {
	        acutPrintf(_T("\nacedSSGet has failed"));
	        return Acad::eInvalidAdsName;
	    }

	    // Get the entity from the selection set and its
	    // subentity ID. This code assumes that the user
	    // selected only one item, a solid.
	    //
	    struct resbuf *pRb;
	    if (acedSSNameX(&pRb, sset, 0) != RTNORM) {
	        acedSSFree(sset);
	        return Acad::eAmbiguousOutput;
	    }
	    acedSSFree(sset);

	    // Walk the list to the third item, which is the selected
	    // entity's entity name.
	    //
	    struct resbuf *pTemp;
	    int i;
	    for (i=1, pTemp = pRb;i<3;i++, pTemp = pTemp->rbnext)
	        { ; }
	    ads_name ename;
	    ads_name_set(pTemp->resval.rlname, ename);

	    // Move on to the fourth list element, which is the gsmarker.
	    //
	    pTemp = pTemp->rbnext;
	    marker = pTemp->resval.rint;
	    acutRelRb(pRb);
	    acdbGetObjectId(objId, ename);
	    return Acad::eOk;
	}

	// This function accepts an object ID and a gsmarker.
	// The object is opened, the gsmarker is used to get the
	// AcDbFullSubentIdPath, which is then used to highlight
	// and unhighlight the edge used to select the object.
	// Next, the object's subentPtr() function is used to get
	// a copy of the edge. This copy is then added to the
	// database. Finally, the object is closed.
	// 
	// Since the copy of the subentity was added to the database 
	// as a new AcDbLine entity, it remains visible in the drawing 
	// editor after the command exits and will be reported by the 
	// AutoCAD LIST command.
	// 
	void highlightEdge(const AcDbObjectId& objId, const int marker) {
	    TCHAR dummy[133]; // space for acedGetString pauses below
	    AcDbEntity *pEnt;
	    acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);

	    // Get the subentity ID for the edge that is picked
	    //
	    AcGePoint3d pickpnt;
	    AcGeMatrix3d xform;
	    int numIds;
	    AcDbFullSubentPath *subentIds;
	    pEnt->getSubentPathsAtGsMarker(AcDb::kEdgeSubentType, marker, pickpnt, xform, numIds, subentIds);

	    // At this point the subentId's variable contains the
	    // address of an array of AcDbFullSubentPath objects.
	    // The array should be one element long, so the picked
	    // edge's AcDbFullSubentPath is in subentIds[0].
	    //
	    // For objects with no edges (such as a sphere), the
	    // code to highlight an edge is meaningless and must
	    // be skipped.
	    //
	    if (numIds > 0) {
	        // Highlight the edge.
	        //
	        pEnt->highlight(subentIds[0]);
	        // Pause to let user see the effect.
	        //
	        acedGetString(0, _T("\npress <RETURN> to continue..."), dummy);

	        // Unhighlight the picked edge.
	        //
	        pEnt->unhighlight(subentIds[0]);

	        // Get a copy of the edge, and add it to the database.
	        //
	        AcDbEntity *pEntCpy = pEnt->subentPtr(subentIds[0]);
	        AcDbObjectId objId;
	        addToModelSpace(objId, pEntCpy);
	    }
	    delete []subentIds;
	    pEnt->close();
	}

	// This function accepts an object ID and a gsmarker.
	// The object is opened, the gsmarker is used to get the
	// AcDbFullSubentIdPath, which is then used to highlight
	// and unhighlight faces that share the edge used to
	// select the object. The object is then closed.
	//
	void highlightFaces(const AcDbObjectId& objId, const int marker) {
	    TCHAR dummy[133];
	    AcDbEntity *pEnt;
	    acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);

	    // Get the subentIds for the faces.
	    //
	    AcGePoint3d pickpnt;
	    AcGeMatrix3d xform;
	    int numIds;
	    AcDbFullSubentPath *subentIds;
	    pEnt->getSubentPathsAtGsMarker(AcDb::kFaceSubentType, marker, pickpnt, xform, numIds, subentIds);

	    // Walk the subentIds list, highlighting each face subentity.
	    //
	    for (int i = 0;i < numIds; i++) {
	        pEnt->highlight(subentIds[i]); // Highlight face.
	        // Pause to let the user see the effect.
	        //
	        acedGetString(0, _T("\npress <RETURN> to continue..."), dummy);
	        pEnt->unhighlight(subentIds[i]);
	    }
	    delete []subentIds;
	    pEnt->close();
	}

	// This function accepts an object ID. The object is opened,
	// and its highlight() and unhighlight() functions are
	// used with no parameters, to highlight and
	// unhighlight the edge used to select the object. The
	// object is then closed.
	//
	void highlightAll(const AcDbObjectId& objId) {
	    TCHAR dummy[133];
	    AcDbEntity *pEnt;
	    acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);

	    // Highlight the whole solid.
	    //
	    pEnt->highlight();

	    // Pause to let user see the effect.
	    //
	    acedGetString(0, _T("\npress <RETURN> to continue..."), dummy);
	    pEnt->unhighlight();
	    pEnt->close();
	}

	Acad::ErrorStatus addToModelSpace(AcDbObjectId &objId, AcDbEntity* pEntity) {
	    AcDbBlockTable *pBlockTable;
	    AcDbBlockTableRecord *pSpaceRecord;
	    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	    pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite);
	    pSpaceRecord->appendAcDbEntity(objId, pEntity);
	    pBlockTable->close();
	    pEntity->close();
	    pSpaceRecord->close();
	    return Acad::eOk;
	}

■ネストされたブロック参照のハイライト表示

■エンティティの分解
	・explode() 関数は、AcDbEntity から派生したオブジェクトの配列を作成します。

■注釈尺度
	特定のエンティティは、ペーパー空間で定義された注釈目的で使用されるオブジェクトの役割も果たします。
	以下のオブジェクトには、注釈サポートがついています。
		文字
		マルチテキスト
		寸法
		引出線
		バルーン
		許容差
		テーブル
		ブロック
		属性
		ハッチング
■異尺度対応オブジェクトの独自関数
■異尺度対応オブジェクトのサンプルの作成
■オブジェクト コンテキスト
	抽象基本クラス AcDbObjectContext から派生したオブジェクトは、
	オブジェクトの動作に影響を及ぼす特定のコンテキスト(この場合は注釈尺度)を表すことができます。
	アプリケーションは AcDbObjectContextCollection を実装して、
	カスタム オブジェクト コンテキストを定義します。
	また、アプリケーションはこのインスタンス生成を AcDbObjectContextCollectionManager に登録する必要もあります。

	現在のコンテキストは、AcDbContextColleciton::setCurrentContext()を呼び出すことで設定できます。
	コレクションと連動させるため、必ず AcDbContextCollectionIterator オブジェクトを作成してください。

■AutoCAD エンティティのインスタンス作成
■単純なエンティティの作成
//線分を作成してモデル空間ブロック テーブル レコードに追加するコードです。
AcDbObjectId createLine() {
    AcGePoint3d startPt(4.0, 2.0, 0.0);
    AcGePoint3d endPt(10.0, 7.0, 0.0);
    AcDbLine *pLine = new AcDbLine(startPt, endPt);
    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()
        ->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);
    pBlockTable->close();
    AcDbObjectId lineId;
    pBlockTableRecord->appendAcDbEntity(lineId, pLine);
    pBlockTableRecord->close();
    pLine->close();
    return lineId;
}
■単純なブロック テーブル レコードの作成
//新しいブロック テーブル レコードを作成してブロック テーブルに追加するコードです。
//このコードはさらに、線分を作成して新しいブロック テーブル レコードに追加します。
void makeABlock() {
     // Create and name a new block table record.
     AcDbBlockTableRecord *pBlockTableRec = new AcDbBlockTableRecord();
     pBlockTableRec->setName(_T("ASDK-NO-ATTR"));

     // Get the block table.
     AcDbBlockTable *pBlockTable = NULL;
     acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForWrite);

     // Add the new block table record to the block table.
     AcDbObjectId blockTableRecordId;
     pBlockTable->add(blockTableRecordId, pBlockTableRec);
     pBlockTable->close();

     // Create and add a line entity to the component's
     // block record.
     AcDbLine *pLine = new AcDbLine();
     AcDbObjectId lineId;
     pLine->setStartPoint(AcGePoint3d(3, 3, 0));
     pLine->setEndPoint(AcGePoint3d(6, 6, 0));
     pLine->setColorIndex(3);
     pBlockTableRec->appendAcDbEntity(lineId, pLine);
     pLine->close();
     pBlockTableRec->close();
}

■属性定義付きのブロック テーブル レコードの作成
■属性付きのブロック参照の作成
■ブロック テーブル レコードのイテレイト(走査)
// ブロック テーブル レコードの中にある要素をイテレイト (走査) して、その要素を出力する方法を示しています。

printAll() 関数は読み込みのためにブロック テーブルをオープンし、次にユーザが指定したブロック名をオープンします。
新しいイテレタがブロック テーブル レコード内を移動します。
レコードの中にエンティティが含まれている場合は、イテレタはそのエンティティについてのメッセージを出力します。

void printAll() {
    int rc;
    TCHAR blkName[50];
    rc = acedGetString(Adesk::kTrue, _T("Enter Block Name <CR for current space>: "), blkName);
    if (rc != RTNORM)
        return;
    if (blkName[0] == _T('\0')) {
        if (acdbHostApplicationServices()->workingDatabase()->tilemode() == Adesk::kFalse) {
            struct resbuf rb;
            acedGetVar(_T("cvport"), &rb);
            if (rb.resval.rint == 1) {
                _tcscpy(blkName, ACDB_PAPER_SPACE);
            } else {
                _tcscpy(blkName, ACDB_MODEL_SPACE);
            }
        } else {
            _tcscpy(blkName, ACDB_MODEL_SPACE);
        }
    }
    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(blkName, pBlockTableRecord, AcDb::kForRead);
    pBlockTable->close();
    AcDbBlockTableRecordIterator *pBlockIterator;
    pBlockTableRecord->newIterator(pBlockIterator);
    for (; !pBlockIterator->done();
        pBlockIterator->step())
    {
        AcDbEntity *pEntity;
        pBlockIterator->getEntity(pEntity, AcDb::kForRead);
        AcDbHandle objHandle;
        pEntity->getAcDbHandle(objHandle);
        TCHAR handleStr[20];
        objHandle.getIntoAsciiBuffer(handleStr);
        const TCHAR *pCname = pEntity->isA()->name();
        acutPrintf(_T("Object Id %lx, handle %s, class %s.\n"), pEntity->objectId(), handleStr, pCname);
        pEntity->close();
    }
    delete pBlockIterator;
    pBlockTableRecord->close();
    acutPrintf(_T("\n"));
}

■複合エンティティ
■複合エンティティの作成
この例では、AcDb2dPolyline オブジェクトを作成して、
オブジェクトのプロパティ (画層、カラー インデックス、"閉じる"パラメータ) をいくつか設定する方法を示しています。
次に 4 つの頂点オブジェクト (AcDb2dPolylineVertex) を作成し、

その位置を設定し、ポリライン オブジェクトに追加します。

最後に、オープンしているオブジェクト (頂点、ポリライン、ブロック テーブル レコード、ブロック テーブル) をすべてクローズします。

ポリライン オブジェクトをクローズすると、AutoCAD は自動的にそこに AcDbSequenceEnd オブジェクトを追加します。

void createPolyline() {
    // Set four vertex locations for the pline.
    AcGePoint3dArray ptArr;
    ptArr.setLogicalLength(4);
    for (int i = 0; i < 4; i++) {
        ptArr[i].set((double)(i/2), (double)(i%2), 0.0);
    }

    // Dynamically allocate an AcDb2dPolyline object,
    // given four vertex elements whose locations are supplied
    // in ptArr.  The polyline has no elevation, and is
    // explicitly set as closed.  The polyline is simple;
    // that is, not curve fit or a spline.  By default, the
    // widths are all 0.0 and there are no bulge factors.
    AcDb2dPolyline *pNewPline = new AcDb2dPolyline(AcDb::k2dSimplePoly, ptArr, 0.0, Adesk::kTrue);
    pNewPline->setColorIndex(3);

    // Get a pointer to a block table object.
    AcDbBlockTable *pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);

    // Get a pointer to the MODEL_SPACE BlockTableRecord.
    AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
    pBlockTable->close();

    // Append the pline object to the database and
    // obtain its object ID.
    AcDbObjectId plineObjId;
    pBlockTableRecord->appendAcDbEntity(plineObjId, pNewPline);
    pBlockTableRecord->close();

    // Make the pline object reside on layer "0".
    pNewPline->setLayer(_T("0"));
    pNewPline->close();
}

■ポリラインの頂点のイテレイト(走査)

次の例では、頂点イテレタを使用してポリラインの頂点をイテレイト (走査) します。
次に各頂点の座標を出力します。
// a vertex iterator. It then iterates through the vertices,
// printing out the vertex location.
// 
void iterate(AcDbObjectId plineId) {
    AcDb2dPolyline *pPline;
    acdbOpenObject(pPline, plineId, AcDb::kForRead);
    AcDbObjectIterator *pVertIter= pPline->vertexIterator();
    pPline->close();  // Finished with the pline header.
    AcDb2dVertex *pVertex;
    AcGePoint3d location;
    AcDbObjectId vertexObjId;
    for (int vertexNumber = 0; !pVertIter->done(); vertexNumber++, pVertIter->step()) {
        vertexObjId = pVertIter->objectId();
        acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead);
        location = pVertex->position();
        pVertex->close();
        acutPrintf(_T("\nVertex #%d's location is : %0.3f, %0.3f, %0.3f"), 
        	vertexNumber, location[X], location[Y], location[Z]);
    }
    delete pVertIter;
}

■複合エンティティの削除
AcDb2dPolyline のサブエンティティの削除方法を示します。
	void delete2dPoly(AcDb2dPolyline* pPline) { 
	  AcDbObjectIterator* pIter=pPline->vertexIterator();
	  AcDbEntity* pEnt; 
	  for (; !pIter->done(); )
	    {
	      pEnt=pIter->entity(); 
	      // Must step the iterator first so that it is no longer 
	      // sitting on the entity that's about to be deleted.
	      pIter->step();
	      delete pEnt;
	    }
	  delete pIter;
	  delete pPline;
	}

■座標系のアクセス
■エンティティ座標系
	独自のエンティティを定義した場合、ジオメトリック構成 (点、角度、ベクトル) を
	そのエンティティの相対座標系で保存しておくと便利です。
	たとえば、円弧は、Z 軸が円弧の平面と垂直になるような座標系を確立します。
	円弧の中心点はワールド座標系で返されますが、開始角度と終点での角度はその円弧のエンティティ座標系 (ECS) でしか変換できません。
	このような場合は、getEcs() 関数を実装することによって、
	そのエンティティを変換するのに使うマトリックスを、エンティティ座標系からワールド座標系に返してください。
	エンティティが、自身のエンティティ座標系で定義されていない場合は、getEcs() 関数は単位マトリックスを返します。
	言い換えれば、エンティティの getEcs() 関数が単位マトリックスを返したときは、
	そのエンティティがワールド座標で定義されていることがわかります。

	AutoCAD では、平面図形に ECS (エンティティ座標系) がありますが、3D エンティティにはありません。
	getEcs() 関数が非単位マトリックスを返すことがある AutoCAD エンティティは以下のものです。
■AcDb2dVertex クラス
	AcDb2dPolyline には、高度と、クラス AcDb2dVertex の X,Y 点の連続があります。
	AcDb2dVertex の position() と setPosition() 関数は、3D 位置を ECS (エンティティ座標系) で指定します。
	setPosition() に渡された Z 座標はエンティティに保存され、position() 関数によって返されますが、それ以外では無視されます。
	これはポリラインの高度には影響しません。

	AcDb2dPolyline クラスは vertexPosition() 関数を提供し、この関数は、渡された頂点のワールド座標系の値を返します。
	ポリラインの高度を変えるには、AcDb2dPolyline::setElevation() 関数を使う方法しかありません。

■曲線関数
抽象基本クラス AcDbCurve には、曲線を操作するための関数が多数あります。
これには曲線の投影用、延長用、オフセット用の関数の他、曲線パラメータを照会するための関数一式も含まれます。
曲線はパラメータ空間でもデカルト座標空間でも定義できます。
3D 曲線は 1 つのパラメータ (f(t)) の関数ですが、3D サーフェスは 2 つのパラメータ (f(u,v)) の関数です。
変換関数を使うと、データをパラメータ表示からデカルト座標系の点に変換できます。
たとえばスプラインは、パラメータ空間で最も良く表示できます。
スプラインを同じ 3 つの部分に分割するには、そのスプラインの点に対応するパラメータをまず見つけてから、
パラメータ空間でそのスプラインを操作してください。
曲線は、3D エンティティの作成で、トリム境界、拡張境界、構成オブジェクトとして使用できます。

次の例のように、ある方向の面に曲線を投影することができます。
// Accepts an ellipse object ID, opens the ellipse, and uses
// its getOrthoProjectedCurve member function to create a
// new ellipse that is the result of a projection onto the
// plane with normal <1,1,1>.  The resulting ellipse is
// added to the model space block table record.
//
void projectEllipse(AcDbObjectId ellipseId) {
    AcDbEllipse *pEllipse;
    acdbOpenObject(pEllipse, ellipseId, AcDb::kForRead);
    // Now project the ellipse onto a plane with a
    // normal of <1, 1, 1>.
    //
    AcDbEllipse *pProjectedCurve;
    pEllipse->getOrthoProjectedCurve(AcGePlane(
        AcGePoint3d::kOrigin, AcGeVector3d(1, 1, 1)),
        (AcDbCurve*&)pProjectedCurve);
    pEllipse->close();
    AcDbObjectId newCurveId;
    addToModelSpace(newCurveId, pProjectedCurve);
}
// Accepts an ellipse object ID, opens the ellipse, and uses
// its getOffsetCurves() member function to create a new
// ellipse that is offset 0.5 drawing units from the
// original ellipse.
//
void offsetEllipse(AcDbObjectId ellipseId) {
    AcDbEllipse *pEllipse;
    acdbOpenObject(pEllipse, ellipseId, AcDb::kForRead);
    // Now generate an ellipse offset by 0.5 drawing units.
    //
    AcDbVoidPtrArray curves;
    pEllipse->getOffsetCurves(0.5, curves);
    pEllipse->close();
    AcDbObjectId newCurveId;
    addToModelSpace(newCurveId, (AcDbEntity*)curves[0]);
}


■コンテナ オブジェクト
コンテナ オブジェクトの種類には、
	シンボル テーブル、
	ディクショナリ、
	グループ、
	Xrecord 
があります。
AutoCAD は、図面の一部としてシンボル テーブルの固定セットと Named Object Dictionary を作成します。
これには他に 2 つのディクショナリ (マルチライン スタイル ディクショナリ、グループ ディクショナリ) が入っています。
このセクションで取り上げる例では、シンボル テーブル、ディクショナリ、グループにエンティティを追加したり、
イテレタを使用してこれらのコンテナの内容を照会する方法を説明します。
その他、アプリケーション データとオブジェクトを管理するために独自のディクショナリと Xrecord を作成する方法も説明します。

■シンボル テーブルとディクショナリの比較
シンボル テーブルとディクショナリの実質上の働きは同じです。
どちらも、テキスト文字列キーを使用して閲覧できるデータベース オブジェクトである項目を含んでいます。
開発者はこれらのコンテナ オブジェクトに項目を追加したり、イテレタを使用して項目内を移動してその内容を照会したりできます。

AutoCAD データベースには常に 9 つのシンボル テーブルの固定セットが入っています。
シンボル テーブルを作成・削除できないが、レコードと呼ばれる項目の追加と変更はできる。
各シンボル テーブルは特定の 1 タイプのオブジェクトしか含みません。
たとえば AcDbLayerTable は AcDbLayerTableRecord タイプのオブジェクトだけを含みます。

AutoCAD データベースが新しい図面を作成するときは、必ず Named Object Dictionary が作成されます。
Named Object Dictionary は、図面の中の非エンティティ オブジェクト構造の主な「目次」として参照できます。
このディクショナリには、既定値で次の 9 個のフォルダがあります。
	GROUP ディクショナリ、
	MLINE スタイル ディクショナリ、
	レイアウト ディクショナリ、
	印刷スタイル名ディクショナリ、
	色ディクショナリ、
	材料ディクショナリ、
	印刷設定ディクショナリ、
	表スタイル ディクショナリ、
	SYSVAR ディクショナリ
です。
好きな数だけオブジェクトを作成して、Named Object Dictionary に追加することができます。
しかし、Named Object Dictionary に追加するオブジェクトは 1 つだけにして、
代わりにそのオブジェクトがアプリケーションに関連する他のオブジェクトを所有するような習慣にするのが最善の策でしょう。
一般に、所有する側のオブジェクトはディクショナリなどのコンテナ クラスのオブジェクトです。
このクラスの名前には、4 文字で登録されている自分の開発者シンボルを使用してください。

AcDbDictionary には、他のディクショナリを含む、どんなタイプの AcDbObject でも保持することができます。
ディクショナリ オブジェクトは、項目のタイプ チェックは行いません。
しかし、マルチライン スタイル ディクショナリにはクラス AcDbMlineStyle のインスタンスのみを挿入し
、グループ ディクショナリには AcDbGroup のインスタンスのみを挿入しなければなりません。
アプリケーションが作成とメンテナンスを行うディクショナリ内の項目は、特定のタイプである必要があります。
シンボル テーブル、シンボル テーブル レコード、ディクショナリ、イテレタのクラス階層は、次のとおりです。

・AcDbObject
	・AcDbSymbolTable
		・AcDbAbstractViewTable
			・AcDbViewportTable
			・AcDbViewTable
		・AcDbBlockTable
		・AcDbDimStyleTable
		・AcDbLayerTable
		・AcDbLinetypeTable
		・AcDbRegAppTable
		・AcDbTextStyleTable
		・AcDbUCSTable
	・AcDbSymbolTableRecord
		・AcDbAbstractViewTableRecord
			・AcDbViewportTableRecord
			・AcDbViewTableRecord
		・AcDbBlockTableRecord
		・AcDbDimStyleTableRecord
		・AcDbLayerTableRecord
		・AcDbLinetypeTableRecord
		・AcDbRegAppTableRecord
		・AcDbTextstyleTableRecord
		・AcDbUCSTableRecord
	・AcDbIictionary
		・AcDbDictionarywithDefault

・AcDbSymbolTableiterator
	・AcDbstractViewTableiterator
		・AcDbViewportTableiterator
		・AcDbViewTableiterator
	・AcDbBlockTableiterator
	・AcDbDimStyleTableiterator
	・AcDbFontTableTableiterator
	・AcDbLayerTableiterator
	・AcDbLinetypeTableiterator
	・AcDbRegAppTableiterator
	・AcDbRegAppTableiterator
	・AcDbTextStyleTableiterator
	・AcDbUCSTableiterator

・シンボル テーブルとディクショナリの重要な違い
	・シンボル テーブル レコードは ObjectARX アプリケーションから直接削除できない。
	  シンボル テーブル レコードを削除する方法は、
	  	PURGE コマンドを使う方法と、
	  	ブロック書き出し操作で選択的にフィルタリング アウトする方法
	  だけです。
	  ディクショナリが所有しているオブジェクトは、削除できません。

	・シンボル テーブル レコードでは、関連する閲覧名をクラス定義のフィールドに保存することです。
	一方ディクショナリでは、次の図のように関連するオブジェクトとは関係なく、
	名前キーをディクショナリの一部として保存します。

■シンボル テーブル
■ブロックテーブル
■画層テーブル
■画層プロパティ
	AcDbLayerTableRecord クラス
	・フリーズ/フリーズ解除
		Acad::ErrorStatus AcDbLayerTableRecord::setIsFrozen(bool frozen);
		bool AcDbLayerTableRecord::isFrozen() const;
	・表示オン/オフ
		void AcDbLayerTableRecord::setIsOff(bool off);
		bool AcDbLayerTableRecord::isOff() const;
	・ビューポート
		void AcDbLayerTableRecord::setVPDFLT(bool frozen);
		bool AcDbLayerTableRecord::VPDFLT() const;
			setVPDFLT() 関数は、既定値で新しいビューポートで画層をフリーズするかを指定します。
	・ロック/ロック解除
		void AcDbLayerTableRecord::setIsLocked(bool locked);
		bool AcDbLayerTableRecord::isLocked() const;
	・色
		void AcDbLayerTableRecord::setColor(const AcCmColor & color);
		AcCmColor AcDbLayerTableRecord::color() const;
	・線種
		void AcDbLayerTableRecord::setLinetypeObjectId(AcDbObjectId id);
		AcDbObjectId AcDbLayerTableRecord::linetypeObjectId() const;
■ 画層テーブル レコードの作成と修正
現在のデータベース用の画層テーブルを取得して、書き込み用にオープンします。

ここでは、
新しい画層テーブル レコード (AcDbLayerTableRecord) を作成して、
画層の特定の属性 (名前、フリーズ属性、オン/オフ、ビューポート、ロック) を設定、
次に色クラス オブジェクトを作成して、
画層の色を赤に設定します。

画層の線種を設定するために、
線種テーブルを読み込み用にオープンして、
希望の線種 (ここでは "DASHED"(破線)) に応じた線種のレコードのオブジェクト ID を取得します。
線種のオブジェクト ID を取得したら、線種テーブルをクローズし、
新しい画層テーブル レコードの線種を設定します。

この例では、add() 関数を使用して画層テーブルに画層テーブル レコードを追加します。
最後に画層テーブル レコードと画層テーブルそのものをクローズします。

	void addLayer() {
	    AcDbLayerTable *pLayerTbl;
	    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForWrite);
	    if (!pLayerTbl->has(_T("ASDK_TESTLAYER"))) {
	        AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
	        pLayerTblRcd->setName(_T("ASDK_TESTLAYER"));
	        pLayerTblRcd->setIsFrozen(0);// layer to THAWED
	        pLayerTblRcd->setIsOff(0);   // layer to ON
	        pLayerTblRcd->setVPDFLT(0);  // viewport default
	        pLayerTblRcd->setIsLocked(0);// un-locked
	        AcCmColor color;
	        color.setColorIndex(1); // set color to red
	        pLayerTblRcd->setColor(color);

	        // For linetype, we need to provide the object ID of
	        // the linetype record for the linetype we want to
	        // use. First, we need to get the object ID.
	        //
	        AcDbLinetypeTable *pLinetypeTbl;
	        AcDbObjectId ltId;
	        acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTbl, AcDb::kForRead);
	        if ((pLinetypeTbl->getAt(_T("DASHED"), ltId)) != Acad::eOk) {
	            acutPrintf(_T("\nUnable to find DASHED linetype. Using CONTINUOUS"));

	            // CONTINUOUS is in every drawing, so use it.
	            //
	            pLinetypeTbl->getAt(_T("CONTINUOUS"), ltId);
	        }
	        pLinetypeTbl->close();
	        pLayerTblRcd->setLinetypeObjectId(ltId);
	        pLayerTbl->add(pLayerTblRcd);
	        pLayerTblRcd->close();
	        pLayerTbl->close();
	    } else {
	        pLayerTbl->close();
	        acutPrintf(_T("\nlayer already exists"));
	    }
	}

■イテレタ
	AcDb##BASE_NAME##Table::newIterator() 関数で作成できます。

	Acad::ErrorStatus AcDb##BASE_NAME##Table::newIterator(
	    AcDb##BASE_NAME##TableIterator*& pIterator,
	    bool atBeginning = Adesk::kTrue,
	    bool skipErased = Adesk::kTrue) const;

	newIterator() 関数は、テーブルの内容の中を移動するのに使うオブジェクトを作成し、
	イテレタ オブジェクトを指すように pIterator を設定します。
	atBeginning が True の場合はイテレタはテーブルの最初から始まり、False の場合はテーブルの最後から始まります。
	skipErased 引数が True の場合、初めに、引数は最初 (または最後) の削除されていないレコードに置かれます。
	False の場合は、削除されているいないに関係なく、最初 (または最後) のレコードに置かれます。
	
	各イテレタ クラスで使用できる関数については、『ObjectARX Reference』を参照してください。

	新しいイテレタを作成したら、それを削除する責任があります。

	シンボル テーブル に加え、ブロック テーブル レコードは所有するエンティティで動作するイテレタを持ちます。
	AcDbBlockTableRecord クラスは、新しいイテレタのために要求された際に
	クラス AcDbBlockTableRecordIterator のオブジェクトを返します。
	このイテレタを使うと、ブロック テーブル レコードに含まれているエンティティ内を移動して
	特定のエンティティを探すことができます。

	注: イテレタは使用後に必ず削除する必要があります。
	演算子を削除しないと、データベースをクローズしたときにアサートを発生させる原因になります。
	acdbFree() を使用してポインタに参照を渡すか、独自の delete() メソッドを実装します。

■テーブルのイテレイト(走査)
	次の例のコードは、線種テーブルに入っているシンボル テーブル レコード内を移動するイテレタを作成します。
	これは各レコードを取得して読み込み用にオープンし、線種名を取得し、レコードをクローズして、
	線種名を出力します。最後にプログラムがイテレタを削除します。

	void iterateLinetypes() {
	    AcDbLinetypeTable *pLinetypeTbl;
	    acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLinetypeTbl, AcDb::kForRead);

	    // Create a new iterator that starts at table
	    // beginning and skips deleted.
	    AcDbLinetypeTableIterator *pLtIterator;
	    pLinetypeTbl->newIterator(pLtIterator);

	    // Walk the table, getting every table record and
	    // printing the linetype name.
	    AcDbLinetypeTableRecord *pLtTableRcd;
	    TCHAR *pLtName;
	    for (; !pLtIterator->done(); pLtIterator->step()) {
	        pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead);
	        pLtTableRcd->getName(pLtName);
	        pLtTableRcd->close();
	        acutPrintf(_T("\nLinetype name is:  %s"), pLtName);
	        free(pLtName);
	    }
	    delete pLtIterator;
	    pLinetypeTbl->close();
	}

■ディクショナリ
■グループとグループディクショナリ
	グループは、データベース エンティティの指示されたコレクションを保存するコンテナ オブジェクト。
	エンティティを削除すると、所属していたグループから自動的に削除されます。
	エンティティが削除されない場合は、グループに自動的に再挿入されます。

	AcDbGroup::newIterator() 関数を使用してイテレタを取得し、グループ内のエンティティの中を移動します。
	AcDbGroup クラスにはこの他にも、
		グループの最初や最後にエンティティを追加する関数、
		グループ内の特定インデックスにエンティティを挿入する関数、
		エンティティを削除する関数、
		グループ内の別の位置にエンティティを転送する関数。『ObjectARX Reference』の AcDbGroup を参照してください。

	また、AcDbGroup クラスの setColor()、setLayer()、setLinetype()、setVisibility()、setHighlight() 関数を使用して、グループの全メンバのプロパティを設定することもできます。この方法で設定しても、グループ内の各エンティティをオープンしてプロパティを直接設定するのと結果は同じになります。

	グループは必ずグループ ディクショナリに保存しなければなりません。グループ ディクショナリは次のように取得できます。

	AcDbDictionary* pGrpDict = acdbHostApplicationServices()->working Database()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	または、Named Object Dictionary にある "ACAD_GROUP" を閲覧してもグループ ディクショナリを取得することができます。

	次の関数は、「ASDK_GROUPTEST」というグループに入っているエンティティをいくつか選択するよう、最初にユーザにプロンプトを出すアプリケーションの一部です。この関数はその後、removeAllButLines() 関数を呼び出してグループをイテレイト (走査)、線分以外のすべてのエンティティを削除します。最後に、グループに残っているエンティティを赤に変えます。


	void groups()
	{
	    AcDbGroup *pGroup = new AcDbGroup(_T("grouptest"));
	    AcDbDictionary *pGroupDict;
	    acdbHostApplicationServices()->workingDatabase() ->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	    AcDbObjectId groupId;
	    pGroupDict->setAt(_T("ASDK_GROUPTEST"), pGroup, groupId);
	    pGroupDict->close();
	    pGroup->close();
	    makeGroup(groupId);
	    removeAllButLines(groupId);
	}

	// Prompts the user to select objects to add to the group,
	// opens the group identified by "groupId" passed in as
	// an argument, then adds the selected objects to the group.
	//
	void makeGroup(AcDbObjectId groupId)
	{
	    ads_name sset;
	    int err = acedSSGet(NULL, NULL, NULL, NULL, sset);
	    if (err != RTNORM) {
	        return;
	    }
	    AcDbGroup *pGroup;
	    acdbOpenObject(pGroup, groupId, AcDb::kForWrite);

	    // Traverse the selection set, exchanging each ads_name
	    // for an object ID, then adding the object to the group.
	    //
	    long i, length;
	    ads_name ename;
	    AcDbObjectId entId;
	    acedSSLength(sset, &length);
	    for (i = 0; i < length; i++) {
	        acedSSName(sset, i, ename);
	        acdbGetObjectId(entId, ename);
	        pGroup->append(entId);
	    }
	    pGroup->close();
	    acedSSFree(sset);
	}

	// Accepts an object ID of an AcDbGroup object, opens it,
	// then iterates over the group, removing all entities that
	// are not AcDbLines and changing all remaining entities in
	// the group to color red.
	//
	void removeAllButLines(AcDbObjectId groupId)
	{
	    AcDbGroup *pGroup;
	    acdbOpenObject(pGroup, groupId, AcDb::kForWrite);
	    AcDbGroupIterator *pIter = pGroup->newIterator();
	    AcDbObject *pObj;
	    for (; !pIter->done(); pIter->next()) {
	        pIter->getObject(pObj, AcDb::kForRead);

	        // If it is not a line or descended from a line,
	        // close it and remove it from the group.  Otherwise,
	        // just close it.
	        //
	        if (!pObj->isKindOf(AcDbLine::desc())) {

	            // AcDbGroup::remove() requires that the object
	            // to be removed be closed, so close it now.
	            //
	            pObj->close();
	            pGroup->remove(pIter->objectId());

	        } else {
	            pObj->close();
	        }
	    }
	    delete pIter;

	    // Now change the color of all the entities in the group
	    // to red (AutoCAD color index number 1).
	    //
	    pGroup->setColorIndex(1);
	    pGroup->close();
	}
	注: AcDbGroup のポインタは、そのすべてのイテレタを削除するまではクローズしてはいけません。































