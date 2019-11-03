using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MyNameSpace {
	public class MyClass {
		public static void Main(string[] args) {
			// 空ファイル作成
			System.IO.File.Create(@"Sample0.txt").Close();
			// 空ファイル作成(何か書くなら)
			using (System.IO.FileStream fs = System.IO.File.Create(@"Sample1.txt")) // using二重書き
			using (System.IO.StreamWriter sw = new System.IO.StreamWriter(fs)) {
				sw.WriteLine("aaa");
			}
			// ファイルコピー(上書きOK)
			System.IO.File.Copy(@"Sample1.txt", @"Sample2.txt", true);
			// ファイル移動(リネーム)
			System.IO.File.Move(@"Sample2.txt", @"Sample3.txt");
			// ファイル削除
			System.IO.File.Delete(@"Sample1.txt");

			System.IO.Directry.CreateDirectory(@"Sample1");

		}
	}
	// クラス
	public class SubClass {	
		public string wd1 { get; private set; }
		public string wd2 { get; private set; }

		public SubClass(string str1, string str2) {
			wd1 = str1;
			wd2 = str2;
		}
		public string SubMethod(){
			return this.wd1 + " " + this.wd2;
		}
	}
}