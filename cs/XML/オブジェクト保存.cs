using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Serialization; // XmlSerializer
using System.IO; //StreamWriter
using System.Text; // UTF8Encoding

namespace MyNameSpace {
	public class MyClass {
		public static void Main(string[] args) {

			// xml用データ作成
//			SampleClass sc = new SampleClass();
//			sc.Message = "テスト。";
//			sc.Number  = 124;
//
//			XmlSample.SaveXml(sc);
//
//			var ret = XmlSample.LoadXml();
//			Console.WriteLine($"Number  = [{ret.Number.ToString()}]");
//			Console.WriteLine($"Message = [{ret.Message}]");
			
			// xml用データ作成
			SampleClass[] ary = new SampleClass[2];
			for (int i = 0; i < ary.Length; i++) {
				ary[i] = new SampleClass();
				ary[i].Number = i;
				ary[i].Message = i.ToString() + "です。";
			}

			XmlSample.SaveXml(ary);
			var retAry = XmlSample.LoadXml2();
			foreach (var e in retAry) {
				Console.WriteLine($"Number  = [{e.Number.ToString()}]");
				Console.WriteLine($"Message = [{e.Message}]");
			}

		}
	}

	public class XmlSample {
		public XmlSample() {
		}

		public static void SaveXml(SampleClass obj){
			string fileName = @"sample.xml";
			//XmlSerializerオブジェクトを作成
			//オブジェクトの型を指定する
			XmlSerializer serializer = new XmlSerializer(typeof(SampleClass));
			//書き込むファイルを開く（UTF-8 BOM無し）
			StreamWriter sw = new StreamWriter(fileName, false, new UTF8Encoding(false));
			//シリアル化し、XMLファイルに保存する
			serializer.Serialize(sw, obj);
			//ファイルを閉じる
			sw.Close();
		}

		public static void SaveXml(SampleClass[] ary){
			string fileName = @"sample.xml";
			XmlSerializer serializer = new XmlSerializer(typeof(SampleClass[]));
			StreamWriter sw = new StreamWriter(fileName, false, new UTF8Encoding(false));
			serializer.Serialize(sw, ary);
			sw.Close();
		}

		public static SampleClass LoadXml(){
			string fileName = @"sample.xml";
			//XmlSerializerオブジェクトを作成
			XmlSerializer serializer = new XmlSerializer(typeof(SampleClass));
			//読み込むファイルを開く
			StreamReader sr = new StreamReader(
			fileName, new UTF8Encoding(false));
			//XMLファイルから読み込み、逆シリアル化する
			SampleClass obj = (SampleClass)serializer.Deserialize(sr);
			//ファイルを閉じる
			sr.Close();
			return obj;
		}

		public static SampleClass[] LoadXml2(){
			string fileName = @"sample.xml";
			//保存した内容を復元する
			XmlSerializer serializer = new XmlSerializer(typeof(SampleClass[]));
			StreamReader sr = new StreamReader(fileName, new UTF8Encoding(false));
			SampleClass[] loadAry;
			loadAry = (SampleClass[])serializer.Deserialize(sr);
			sr.Close();
			return loadAry;
		}


	}
	//XMLファイルに保存するオブジェクトのためのクラス
	public class SampleClass {
	    [System.Xml.Serialization.XmlIgnore]
	    public string hoge = "hogege";
		public int Number;
	    public string Message;
	    [System.Xml.Serialization.XmlIgnore]
	    public string huga = "hugaga";
	}
}

