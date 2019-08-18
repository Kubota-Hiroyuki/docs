■■1.要素の取得（単一）
	メソッド名           |機能
	---------------------+----------------------------------------------------------------------------
	ElementAt            |
	ElementAtOrDefault   |指定した位置(インデックス)にある要素を返します。
	First                |
	FirstOrDefault       |最初の要素を返します。
	Last                 |
	LastOrDefault        |最後の要素を返します。
	Single               |
	SingleOrDefault      |唯一の要素を返します。該当する要素が複数ある場合、例外をスローします。
	該当の要素がない場合は… 
	　「～OrDefault」が付いていないメソッドは例外をスロー。
	　「～OrDefault」が付いたメソッドは型の規定値を返す。

	var source = new[] { 3, 4, 5, 6, 7, 8, 9, 9 };
	Console.WriteLine(source.ElementAt(2));           // → 5
	Console.WriteLine(source.ElementAtOrDefault(10)); // → 0
	Console.WriteLine(source.First());                // → 3
	Console.WriteLine(source.First(e => e > 5));      // → 6
	Console.WriteLine(source.Last());                 // → 9
	Console.WriteLine(source.Last(e => e < 5));       // → 4
	Console.WriteLine(source.Single());               // → System.InvalidOperationException:
	                                                  //    シーケンスに複数の要素が含まれています
	Console.WriteLine(source.Single(e => e < 4 ));    // → 3

■■2.要素の取得（複数）
	メソッド名           |機能
	---------------------+-----------------------------------
	Where                |条件を満たす要素をすべて返します。
	Distinct             |重複を除いたシーケンスを返します。
	Skip                 |先頭から指定された数の要素をスキップし、残りのシーケンスを返します。
	SkipWhile            |先頭から指定された条件を満たさなくなるまで要素をスキップし、残りのシーケンスを返します。
	Take                 |先頭から指定された数の要素を返します。
	TakeWhile            |先頭から指定された条件を満たす要素を返します。

	var source = new[] { 3, 4, 5, 6, 7, 8, 9, 9 };
	Console.WriteLine(source.Where(e => e > 5).ToResult());     // → {6, 7, 8, 9, 9}
	Console.WriteLine(source.Distinct().ToResult());            // → {3, 4, 5, 6, 7, 8, 9}
	Console.WriteLine(source.Skip(5).ToResult());               // → {8, 9, 9}
	Console.WriteLine(source.SkipWhile(e => e < 5).ToResult()); // → {5, 6, 7, 8, 9, 9}
	Console.WriteLine(source.Take(5).ToResult());               // → {3, 4, 5, 6, 7}
	Console.WriteLine(source.TakeWhile(e => e < 5).ToResult()); // → {3, 4}

■■3.集計
	メソッド名 |機能
	-----------+------------------------------------------------
	Max        |最大値を返します。
	Min        |最小値を返します。
	Average    |平均値を返します。
	Sum        |合計を返します。
	Count      |要素数を返します。
	Aggregate  |アキュムレータ関数で処理した結果を返す。

	var source = new[] { 3, 4, 5, 6, 7, 8, 9, 9 };
	Console.WriteLine(source.Max());                                // → 9
	Console.WriteLine(source.Min());                                // → 3
	Console.WriteLine(source.Average());                            // → 6.375
	Console.WriteLine(source.Sum());                                // → 51
	Console.WriteLine(source.Count());                              // → 8
	// https://webbibouroku.com/Blog/Article/linq-aggregate
	Console.WriteLine(source.Aggregate((now, next) => now * next)); // → 1632960 (3*4*5*6*7*8*9*9)
	Console.WriteLine(source.Aggregate((count, elem) => count + 1); // → 8
	Console.WriteLine(source.Aggregate((n, elem) => n + elem);      // → 51 (3+4+5+6+7+8+9+9)
	Console.WriteLine(source.Aggregate(100, (n, elem) => n + elem); // → 151 100+(3+4+5+6+7+8+9+9)
	var data = new[] {"あ","い","う","え","お"};
	var d = data.Aggregate("", (str, elem) => {
	    Console.WriteLine(string.Format($"STR:{str}, ELEM:{elem}"));
	    return str + elem;
	});
	Console.WriteLine(d); // → あいうえお？
	
	// 参考：標本分散
	double ave = source.Average();
	Console.WriteLine(source.Sum(e => Math.Pow(e - ave, 2)) / source.Count()); // → 4.484375

■■4.判定
	メソッド名      |機能
	----------------+-------
	All             |すべての要素が条件を満たしてるか判定。
	Any             |条件を満たす要素が含まれているか判定。
	Contains        |指定した要素が含まれているかどうかを判定。
	SequenceEqual   |2つのシーケンスが等しいかどうかを判定。

	var source = new[] { 3, 4, 5, 6, 7, 8, 9, 9 };
	Console.WriteLine(source.All(e => e > 5));                     // → False
	Console.WriteLine(source.Any(e => e > 5));                     // → True
	Console.WriteLine(source.Contains(10));                        // → False
	Console.WriteLine(source.SequenceEqual(new[] { 6, 7, 8, 9 })); // → False

■■5.集合
	メソッド名      |機能
	----------------+-----------------------------------------
	Union           |指定したシーケンスとの和集合を返す。
	Except          |指定したシーケンスとの差集合を返す。
	Intersect       |指定したシーケンスとの積集合を返す。

	var first = new[] { 3, 4, 5, 6, 7, 8, 9, 9 };
	var second = new[] { 8, 9, 10 };
	Console.WriteLine(first.Union(second).ToResult());     // → {3, 4, 5, 6, 7, 8, 9, 10}
	Console.WriteLine(first.Except(second).ToResult());    // → {3, 4, 5, 6, 7}
	Console.WriteLine(first.Intersect(second).ToResult()); // → {8, 9}

■■6.ソート
	メソッド名            |機能
	----------------------+-----------------------------------------------------------------------------------------
	OrderBy               |昇順にソートしたシーケンスを返します。
	OrderByDescending     |降順にソートしたシーケンスを返します。
	ThenBy                |ソートしたシーケンスに対し、キーが等しい要素同士を昇順にソートしたシーケンスを返します。
	ThenByDescending      |ソートしたシーケンスに対し、キーが等しい要素同士を降順にソートしたシーケンスを返します。
	Reverse               |逆順にソートしたシーケンスを返します。

	var source = new[] {  
	    new{Name = "C#", Age = 11},
	    new{Name = "Java", Age = 16},
	    new{Name = "Groovy", Age = 8},
	    new{Name = "Scala", Age = 8},
	};
	Console.WriteLine(source.OrderBy(e => e.Age).ToResult());
		// → {{ Name = Groovy, Age = 8 },
		//     { Name = Scala, Age = 8 }, 
		//     { Name = C#, Age = 11 },
		//     { Name = Java, Age = 16 }}
	Console.WriteLine(source.OrderByDescending(e => e.Age).ToResult());
		// → {{ Name = Java, Age = 16 },
		//     { Name = C#, Age = 11 },
		//     { Name = Groovy, Age = 8 },
		//     { Name = Scala, Age = 8 }}
	Console.WriteLine(source.OrderBy(e => e.Age)
	                        .ThenBy(e => e.Name.Length).ToResult());
		// → {{ Name = Scala, Age = 8 },
		//     { Name = Groovy, Age = 8 },
		//     { Name = C#, Age = 11 },
		//     { Name = Java, Age = 16 }}
	Console.WriteLine(source.OrderBy(e => e.Age)
	                        .ThenByDescending(e => e.Name.Length).ToResult());
		// → {{ Name = Groovy, Age = 8 },
		//     { Name = Scala, Age = 8 },
		//     { Name = C#, Age = 11 },
		//     { Name = Java, Age = 16 }}
	Console.WriteLine(source.Reverse().ToResult());
		// → {{ Name = Scala, Age = 8 },
		//     { Name = Groovy, Age = 8 },
		//     { Name = Java, Age = 16},
		//     { Name = C#, Age = 11 }}

■■7.射影
	メソッド名            |機能
	----------------------+---------------
	Select                |1つの要素を単一の要素に射影します。
	SelectMany            |1つの要素から複数の要素に射影します。その結果を1つのシーケンスとして返します。
	GroupBy               |指定のキーで要素をグループ化します。その "キーとグループ" のシーケンスを返します。

	var source = new[] {  
	    new{Name = "C#", Age = 11},
	    new{Name = "Java", Age = 16},
	    new{Name = "Groovy", Age = 8},
	    new{Name = "Scala", Age = 8},
	};
	Console.WriteLine(source.Select(e => e.Name).ToResult()); 
		// → {C#, Java, Groovy, Scala}
	Console.WriteLine(source.SelectMany(e => e.Name.ToCharArray()).ToResult());
		// → {C, #, J, a, v, a, G, r, o, o, v, y, S, c, a, l, a}
	Console.WriteLine(source.GroupBy(e => e.Age).ToResult());
		// → {Key=11, Source={{ Name = C#, Age = 11 }},
		//     Key=16, Source={{ Name = Java, Age = 16 }},
		//     Key=8, Source={{ Name = Groovy, Age = 8 }, { Name = Scala, Age = 8 }}}
■■8.結合
	メソッド名            |機能
	----------------------+-------------------------------------------------------------------------------------------
	Join                  |内部結合を行ったシーケンスを返す。
	GroupJoin             |左外部結合を行って指定のキーでグループ化する。その "キーとグループ" のシーケンスを返す。
	Concat                |２つのシーケンスを連結します。
	                      |（Unionは同じ要素を一つにまとめますが、Concatは元の要素をすべて返します。）
	DefaultIfEmpty        |シーケンスを返す。シーケンスが空なら、規定値もしくは任意の要素を返す。
	Zip                   |指定した関数で、2つのシーケンスを1つのシーケンスにマージ。

	var outer = new[] {
	    new{Name = "C#", Age = 11},
	    new{Name = "Java", Age = 16},
	    new{Name = "Groovy", Age = 8},
	    new{Name = "Scala", Age = 8},
	};
	var outer2 = new[] {
	     new{Name = "Python", Age = 21},
	     new{Name = "COBOL", Age = 52},
	};
	var inner = new[] {
	    new{Name = "C#", DesignedBy = "Microsoft"},
	    new{Name = "Java", DesignedBy = "Sun Microsystems"},
	    new{Name = "Java", DesignedBy = "Oracle"},
	};

	Console.WriteLine(outer.Join(inner,
	                             o => o.Name,
	                             i => i.Name, 
	                             (o, i) => new { o.Name, o.Age, i.DesignedBy}).ToResult());
		// → {{ Name = C#,   Age = 11, DesignedBy = Microsoft },
		//     { Name = Java, Age = 16, DesignedBy = Sun Microsystems },
		//     { Name = Java, Age = 16, DesignedBy = Oracle }}
	Console.WriteLine(outer.GroupJoin(inner,
	                                  o => o.Name,
	                                  i => i.Name,
	                                  (o, i) => new { o.Name, o.Age, DesigndBy = i.Select(e => e.DesignedBy).ToResult() }).ToResult());
		// → {{ Name = C#,     Age = 11, DesigndBy = {Microsoft} },
		//     { Name = Java,   Age = 16, DesigndBy = {Sun Microsystems, Oracle} },
		//     { Name = Groovy, Age = 8,  DesigndBy = {} },
		//     { Name = Scala,  Age = 8,  DesigndBy = {} }}
	Console.WriteLine(outer.Concat(outer2).ToResult());
		// → {{ Name = C#,     Age = 11 },
		//     { Name = Java,   Age = 16 },
		//     { Name = Groovy, Age = 8 },
		//     { Name = Scala,  Age = 8 },
		//     { Name = Python, Age = 21 },
		//     { Name = COBOL,  Age = 52 }}
	Console.WriteLine(outer.DefaultIfEmpty().ToResult());
		// → {{ Name = C#,     Age = 11 },
		//     { Name = Java,   Age = 16 },
		//     { Name = Groovy, Age = 8 },
		//     { Name = Scala,  Age = 8 }}
	Console.WriteLine(outer.Zip(outer2, (o1, o2) => o1.Name + "&" + o2.Name).ToResult());
		// → {C#&Python, Java&COBOL}

■■9.変換
	メソッド名            |機能
	----------------------+-------------------------------------------------------------------------------
	OfType                |各要素を指定した型に変換。キャストできない要素は除外。
	Cast                  |各要素を指定した型に変換。キャストできない要素が含まれていた場合、例外をスロー。
	ToArray               |配列を作成します。
	ToDictionary          |連想配列(ディクショナリ)を作成します。
	ToList                |リストを生成します。
	ToLookup              |キーコレクション*1を生成します。
	AsEnumerable          |IEnumerable を返します。*2

	ArrayList mixed = new ArrayList { "C#", "Java", 3.141592653, "Groovy", "Scala" };
	Console.WriteLine(mixed.OfType<string>().ToResult());
		// → {C#, Java, Groovy, Scala}
	Console.WriteLine(mixed.Cast<string>().ToResult());
		// → System.InvalidCastException: 
		//    型 'System.Double' のオブジェクトを型 'System.String' にキャストできません。

■■補足 結果表示用の拡張メソッド
	static String ToResult<TSource>(this IEnumerable<TSource> source) {
	    return "{" + string.Join(", ", source) + "}";
	}

	static String ToResult<TKey, TSource>(this IEnumerable<IGrouping<TKey, TSource>> source) {
	    return source.Select(group => string.Format("Key={0}, Source={1}", group.Key, group.ToResult())).ToResult();
	}

	*1:1対多のディクショナリ。例えば、～.ToLookup()["hoge"] と実行すると、
	   "hoge" に紐付く要素の集合（IEnumerable）が返ってきます。
	*2:IEnumerable と同じ名前のメソッドがクラス内に定義されている場合に使います。
	    そのままだと、クラス内のメソッドが優先的に選択されて、IEnumerable の拡張メソッドが呼びだせないためです。

