using System;
using System.Drawing;
using System.Windows.Forms;

// http://pineplanter.moo.jp/non-it-salaryman/2017/06/01/c-sharp-tasktray/
namespace TaskTest {
	class ResidentTest : Form{
		public ResidentTest(){
			// point2. タスクバーに表示させない
			this.ShowInTaskbar = false;
			this.SetComponents();
		}

		private void Close_Click(object sender, EventArgs e){
			MessageBox.Show("おしまい");
			Application.Exit();
		}
		private void About_Click(object sender, EventArgs e){
			MessageBox.Show("Ver.1(190906)");
		}

		private void SetComponents(){
			// point3. NotifyIconを使い、タスクトレイにアイコンを表示させる
			NotifyIcon icon = new NotifyIcon();
			icon.Icon = new Icon("app.ico");
			icon.Visible = true;
			icon.Text = "常駐アプリテスト";

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