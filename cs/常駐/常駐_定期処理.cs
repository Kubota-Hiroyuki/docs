using System;
using System.Drawing;
using System.Windows.Forms;
using System.Threading; // Add

// http://pineplanter.moo.jp/non-it-salaryman/2017/06/03/c-sharp-thread/
namespace TaskTest {
	class ResidentTest : Form{
		private Thread thread; // Add

		public ResidentTest(){
			this.ShowInTaskbar = false;
			this.SetComponents();
			// point1. Threadクラス実装
			thread = new Thread(IntervalCheck); // Add
			thread.Start();                     //
		}

		private void Close_Click(object sender, EventArgs e){
			// point3. スレッドの後始末
			thread.Abort(); // Add
			MessageBox.Show("おしまい");
			Application.Exit();
		}
		private void About_Click(object sender, EventArgs e){
			MessageBox.Show("Ver.1(190906)");
		}

		// Add Method
		// point2. 別スレッド処理
		private void IntervalCheck() {
			const int ms = 10000;
			int cnt = 0;
			while(true){
				System.Threading.Thread.Sleep(ms); // 10.000秒休止
				cnt++;
				MessageBox.Show($"{ms}ms経過しました。({cnt.ToString()}回目)");
			}
		}

		private void SetComponents(){
			// point3. NotifyIconを使い、タスクトレイにアイコンを表示させる
			NotifyIcon icon = new NotifyIcon();
			icon.Icon = new Icon("app.ico");
			icon.Visible = true;
			icon.Text = "常駐テスト(定期メッセージ)";

			// point4. 右クリックメニュー
			ContextMenuStrip menu = new ContextMenuStrip();
			ToolStripMenuItem menuItem1 = new ToolStripMenuItem();
			menuItem1.Text = "&終了";
			menuItem1.Click += new EventHandler(Close_Click);
			ToolStripMenuItem menuItem2 = new ToolStripMenuItem();
			menuItem2.Text = "&About";
			menuItem2.Click += new EventHandler(About_Click);
			menu.Items.AddRange(new ToolStripMenuItem[] {
				menuItem1,
				menuItem2
			});
			icon.ContextMenuStrip = menu;
		}
	}

	class MainWindow{
		static void Main(){
			 // point1. フォームを表示させない
			 // 「Application.Run(ResidentTest());」だと見える。
			ResidentTest rm = new ResidentTest();
			Application.Run();
		}
	}

}