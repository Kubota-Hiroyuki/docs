using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//using System.Runtime.InteropServices; // DLL Import
using System.Diagnostics; // Process

using Win32WrapLib;

namespace AlermTimer {
	class Program {
		[STAThread]
		static void Main() {
			Application.EnableVisualStyles();
			Application.Run(new Form1());
		}
	}

	enum Mode {
		Ready,// アラーム設定待ち
		Run,  // アラーム起動中
		Wait} // アラーム鳴動中

	public class Form1 : Form {

		string strRemindTime = "163000";
		DateTime dtRemindTime;
		string currentProcessName = string.Empty;

		// #tabControl
		TabControl tabc;
		TabPage    tabp1;
		TabPage    tabp2;
		TabPage    tabp3;
		TabPage    tabp4;
		
		// #tabPage1
		Timer    timer1    = new Timer();   // タイマー クラス
		TextBox  label1   = new TextBox(); // 時刻入力TextBox
		Button   btnExec  = new Button();  // 実行・停止
		Button   btnInit  = new Button();  // 時刻初期化
		Button[] button1;                  // [0]時刻＋25分,[1] 時刻＋5分, [2]時刻＋1分
		Button   btnRound = new Button();  // 秒針切捨て
		Mode     eMode    = Mode.Ready;    // タイマーの状態遷移 
		CheckBox chkRemind= new CheckBox();

		// #tabPage2
		Timer    timer2    = new Timer();   // タイマー クラス
		Label[]   label21;
		TextBox[] text21;
		Point sp, cp;
		ToolTip toolTip2   = new ToolTip();
		int m_curId; 

		// #tabPage3
		Label[]   label31;
		TextBox[] text31;
		Button[] button3;
		NotifyIcon notifyIcon3;

		// #tabPage4
		Label[]   label41;
		TextBox[] text41;
		Button[] button4;

		public Form1() {
			this.Size    = new Size(270,170);

			System.Reflection.Assembly myAssembly = System.Reflection.Assembly.GetEntryAssembly();
			currentProcessName = System.IO.Path.GetFileNameWithoutExtension(myAssembly.Location);


		// #Form
			// フォームの開く/閉じるに関するイベント
//			this.Load        += new EventHandler(this.Form_Load);
//			this.Shown       += new EventHandler(this.Form_Shown);
//			this.FormClosing += new FormClosingEventHandler(this.Form_FormClosing);
//			this.FormClosed  += new FormClosedEventHandler(this.Form_FormClosed);
			this.FormClosed += (sender, e)=>{
				notifyIcon3.Dispose(); // タスクトレイのアイコンの後始末
			};

		// #tabControl
			Controls.Add(tabc = new TabControl() { Dock = DockStyle.Fill,});
			tabp1 = new TabPage() { Text = "ALM", Dock = DockStyle.Fill,};
			tabp2 = new TabPage() { Text = "PT", Dock = DockStyle.Fill,};
			tabp3 = new TabPage() { Text = "CAP", Dock = DockStyle.Fill,};
			tabp4 = new TabPage() { Text = "#4", Dock = DockStyle.Fill,};
			tabc.TabPages.Add(tabp1);
			tabc.TabPages.Add(tabp2);
			tabc.TabPages.Add(tabp3);
			tabc.TabPages.Add(tabp4);

			System.Diagnostics.Process p = System.Diagnostics.Process.GetCurrentProcess();
			m_curId = p.Id;

		// #tabPage1
			label1       = new TextBox()  { Location = new Point(10,  30), Size = new Size(100 ,20), Font     = new Font("Arial", 10), };
			label1.Text  = DateTime.Now.ToLongTimeString();
			btnExec      = new Button()   { Location = new Point(10,  80), Size = new Size(100, 20),};
			btnRound     = new Button()   { Location = new Point(110, 55), Size = new Size(50, 20), Text     = "Round",};
			btnInit      = new Button()   { Location = new Point(110, 80), Size = new Size(50, 20), Text     = "Init", };
			chkRemind    = new CheckBox() { Location = new Point(10,  10), Size = new Size(80, 20), Text     = "reminder", Checked  = false, };
			int u = 3;
			button1 = new Button[u];
			for (var i=0; i<u; i++) {
				button1[i] = new Button();
				button1[i].Size = new Size(50, 20);
				button1[i].Location = new Point(160, 30 + i * 25);
				tabp1.Controls.Add(button1[i]);
				button1[i].Click += new EventHandler(button1_Click);
			}
			tabp1.Controls.AddRange(button1);
			button1[0].Text     = "+25min";
			button1[1].Text     = "+5min";
			button1[2].Text     = "+1min";
			tabp1.Controls.Add(label1);
			tabp1.Controls.Add(btnExec);
			tabp1.Controls.Add(btnInit);
			tabp1.Controls.Add(btnRound);
			tabp1.Controls.Add(chkRemind);

			btnExec.Click  += new EventHandler(btnExec_Click);
			btnInit.Click  += new EventHandler(btnInit_Click);
			btnRound.Click += new EventHandler(btnRound_Click);

			timer1.Interval = 1000;
			timer1.Tick += new EventHandler(timer1_Tick);
			timer1.Start();
			this.ChangeMode(Mode.Ready);
			dtRemindTime = this.InitReminder(strRemindTime);

		// #tabPage2
			int w = 7;
			label21 = new Label[w];
			text21 = new TextBox[w];
			for (var i=0; i<w; i++) {
				label21[i] = new Label();
				label21[i].Size = new Size(90, 20);
				label21[i].Location = new Point(10, 10 + i * 25);
				text21[i] = new TextBox();
				text21[i].Size = new Size(140, 20);
				text21[i].Location = new Point(100, 10 + i * 25);
				text21[i].ReadOnly = true;
			}
			label21[0].Text = "Screen座標：";
			label21[1].Text = "Client座標：";
			label21[2].Text = "プロセス名：";
			label21[3].Text = "Window Title：";
			label21[4].Text = "Window Size ：";
			label21[5].Text = "Window Min：";
			label21[6].Text = "Window Max：";
			tabp2.Controls.AddRange(label21);
			tabp2.Controls.AddRange(text21);
			timer2.Interval = 10;
			timer2.Tick += new EventHandler(timer2_Tick);
			timer2.Start();

//			tabc.Selected += new TabControlEventHandler(tabControl1_SelectionChanged);
			tabc.Selected += (sender, e)=>{
				// 選択されたタブの番号を取得
				int idx = tabc.SelectedIndex + 1;
				if (idx == 1)
					this.Size = new Size(270, 170);
				else if (idx == 2)
					this.Size = new Size(270, 250);
				else
					this.Size = new Size(320, 200);
			};

			//ToolTipの設定を行う
			toolTip2.InitialDelay = 500;  //ToolTipが表示されるまでの時間
			toolTip2.ReshowDelay = 1000;   //ToolTipが表示されている時に、別のToolTipを表示するまでの時間
			toolTip2.AutoPopDelay = 10000; //ToolTipを表示する時間
			toolTip2.ShowAlways = true;    //フォームがアクティブでない時でもToolTipを表示する
			toolTip2.SetToolTip(text21[3], "Default"); //指定コントロールにToolTipが表示されるようにする

		// #tabPage3
			int v = 9;
			label31 = new Label[v];
			text31 = new TextBox[v];
			button3 = new Button[v];
			for (var i=0; i<v; i++) {
				label31[i] = new Label();
				label31[i].Size = new Size(80, 20);
				label31[i].Location = new Point(10, 10 + i * 25 + 3);
				text31[i] = new TextBox();
				text31[i].Size = new Size(40, 20);
				text31[i].Leave += (sender, e) => {
					TextBox txt = (TextBox)sender;
						txt.SelectAll();
				};
				button3[i] = new Button();
				button3[i].Size = new Size(90, 20);
				button3[i].Location = new Point(200, 10 + i * 25);

				button3[i].Click += new EventHandler(button3_Click);
			}
			label31[0].Text = "左上xy：";
			label31[1].Text = "右下xy：";
			label31[2].Text = "プロセス名：";
			label31[3].Text = "cap左上xy：";
			label31[4].Text = "cap右下xy：";
			text31[0].Location = new Point(100, 10 + 0 * 25);
			text31[1].Location = new Point(150, 10 + 0 * 25);
			text31[2].Location = new Point(100, 10 + 1 * 25);
			text31[3].Location = new Point(150, 10 + 1 * 25);
			text31[4].Size = new Size(90, 20);
			text31[4].Location = new Point(100, 10 + 2 * 25);
			text31[5].Location = new Point(100, 10 + 3 * 25);
			text31[6].Location = new Point(150, 10 + 3 * 25);
			text31[7].Location = new Point(100, 10 + 4 * 25);
			text31[8].Location = new Point(150, 10 + 4 * 25);
			text31[4].ReadOnly = true;
			text31[5].ReadOnly = true;
			text31[6].ReadOnly = true;
			text31[7].ReadOnly = true;
			text31[8].ReadOnly = true;

			button3[0].Text = "部分キャプ";
			button3[1].Text = "フルキャプ";
			button3[2].Text = "座標格納";
			button3[3].Text = "Button3";
			tabp3.Controls.AddRange(label31);
			tabp3.Controls.AddRange(text31);
			tabp3.Controls.AddRange(button3);

			this.notifyIcon3 = new NotifyIcon();
			this.notifyIcon3.Icon = new System.Drawing.Icon(@"waku.ico");
			notifyIcon3.Visible = true;

		// #tabPage4
			int x = 5;
			label41 = new Label[x];
			text41 = new TextBox[x];
			button4 = new Button[x];
			for (var i=0; i<x; i++) {
				label41[i] = new Label();
				label41[i].Size = new Size(90, 20);
				label41[i].Location = new Point(10, 10 + i * 25);
				text41[i] = new TextBox();
				text41[i].Size = new Size(140, 20);
				text41[i].Location = new Point(100, 10 + i * 25);
				text41[i].ReadOnly = true;
				button4[i] = new Button();
				button4[i].Size = new Size(90, 20);
				button4[i].Location = new Point(250, 10 + i * 25);

				button4[i].Click += (sender, e) => {
					Button btn = (Button)sender;
						MessageBox.Show($"{btn.Text}");
				};
			}
			label41[0].Text = "Label0：";
			label41[1].Text = "Label1：";
			label41[2].Text = "Label2：";
			label41[3].Text = "Label3：";
			label41[4].Text = "Label4：";
			button4[0].Text = "Button0";
			button4[1].Text = "Button1";
			button4[2].Text = "Button2";
			button4[3].Text = "Button3";
			button4[4].Text = "Button4";
			tabp4.Controls.AddRange(label41);
			tabp4.Controls.AddRange(text41);
			tabp4.Controls.AddRange(button4);
		}

		// #Form
//		private void Form_Load(object sender, EventArgs e) {
//			MessageBox.Show("初めて表示される直前に発生", "Loadイベント");
//		}
//		private void Form_Shown(object sender, EventArgs e) {
//			MessageBox.Show("フォームが表示された直後", "Shownイベント");
//		}
//		private void Form_FormClosing(object sender, FormClosingEventArgs e) {
//			DialogResult result = MessageBox.Show("フォームが閉じる直前に発生します。\nこの段階ではまだ閉じる操作をキャンセルできる\n\n閉じていいですか",
//			                                      "Closingイベント",
//			                                      MessageBoxButtons.YesNo);
//			if (result != DialogResult.Yes)
//				e.Cancel = true;
//		}
//		private void Form_FormClosed(object sender, FormClosedEventArgs e) {
//			MessageBox.Show("フォームが閉じた直後に発生します。\n最後に動かしたい処理はこのタイミングで記述。", "FormClosedイベント");
//		}

	// #tabPage1
		private void timer1_Tick(object sender, EventArgs e) {
			DateTime datetime = DateTime.Now;
			this.Text = datetime.ToLongTimeString();
			
			DateTime datetime_set;
			if (DateTime.TryParse(label1.Text, out datetime_set) && (eMode == Mode.Run)) {
				if ((datetime.Hour   == datetime_set.Hour  ) && 
					(datetime.Minute == datetime_set.Minute) && 
					(datetime.Second == datetime_set.Second)) {
					this.ChangeMode(Mode.Wait);
					eMode = Mode.Wait;
				}
			}
			if (chkRemind.Checked) {
				var nowTime = DateTime.Now;
				if ((nowTime.Hour   == dtRemindTime.Hour  ) && 
					(nowTime.Minute == dtRemindTime.Minute) && 
					(nowTime.Second == dtRemindTime.Second)) {
					this.ChangeMode(Mode.Wait);
					eMode = Mode.Wait;
				}
			}

		}
		private void btnExec_Click(object sender, EventArgs e){
			DateTime datetime_set;
			if (DateTime.TryParse(label1.Text, out datetime_set) ) {
				if (eMode == Mode.Ready) {
					this.ChangeMode(Mode.Run);
					eMode = Mode.Run;
				} else if (eMode == Mode.Run) {
					this.ChangeMode(Mode.Ready);
					eMode = Mode.Ready;
				} else if (eMode == Mode.Wait) {
					this.ChangeMode(0);
					eMode = Mode.Ready;
				}
			}
		}
		private void btnInit_Click(object sender, EventArgs e){
			label1.Text = DateTime.Now.ToLongTimeString();
		}
		private void btnRound_Click(object sender, EventArgs e){
			if (DateTime.TryParse(label1.Text, out DateTime datetime_set) ) {
				datetime_set = datetime_set.AddSeconds(-datetime_set.Second);
				label1.Text = datetime_set.ToLongTimeString();
			}
		}

		private void button1_Click(object sender, EventArgs e){
			Button btn = (Button)sender;
			string text = btn.Text;
			DateTime datetime_set;
			if (DateTime.TryParse(label1.Text, out datetime_set) ) {
				TimeSpan addMin = new TimeSpan(0,0,0);
				switch(text) {
				  case "+25min":
					addMin = new TimeSpan(0, 25, 0);
				    break;
				  case "+5min":
					addMin = new TimeSpan(0,  5, 0);
				    break;
				  case "+1min":
					addMin = new TimeSpan(0,  1, 0);
				    break;
				}
				label1.Text = (datetime_set + addMin).ToLongTimeString();
			}
		}

		//コントロールの色一括変更の参考例
		//foreach( Control c in this.Controls) {
		//	if( c.GetType() == typeof(Button)) {
		//		Button button = (Button)c;
		//		c.BackColor = Color.Red;
		//	}
		//}
		private void ChangeMode(Mode eMode){
			if (eMode == Mode.Ready) {
				this.BackColor         = Color.White;
				tabp1.BackColor        = Color.White;
				this.Opacity           = 1.0;
				this.TopMost           = false;
				label1.BackColor       = Color.White;
				label1.Enabled         = true;
				btnExec.Text           = "Exec!";
				btnInit.Enabled        = true;
				btnRound.Enabled       = true;
				button1[0].Enabled     = true;
				button1[1].Enabled     = true;
				button1[2].Enabled     = true;
			} else if (eMode == Mode.Run) {
				label1.Enabled         = false;
				btnExec.Text           = "Stop";
				btnInit.Enabled        = false;
				btnRound.Enabled       = false;
				button1[0].Enabled     = false;
				button1[1].Enabled     = false;
				button1[2].Enabled     = false;
			} else if (eMode == Mode.Wait) {
				if (this.WindowState != FormWindowState.Normal) {
					this.WindowState = FormWindowState.Normal;
				}
				this.BackColor         = Color.Yellow;
				tabp1.BackColor        = Color.Yellow;
				this.Opacity           = 0.7;
				this.TopMost           = true;
				label1.BackColor       = Color.Yellow;
				label1.Enabled         = false;
				btnExec.Text           = "Reset";
				btnInit.Enabled        = false;
				btnRound.Enabled       = false;
				button1[0].Enabled     = false;
				button1[1].Enabled     = false;
				button1[2].Enabled     = false;
			}
		}

		private DateTime InitReminder(string strTime) {
			DateTime dt1 = System.DateTime.ParseExact(strTime,
			"HHmmss",
			System.Globalization.DateTimeFormatInfo.InvariantInfo,
			System.Globalization.DateTimeStyles.None);
			return dt1;
		}

	// #tabPage2
		private void timer2_Tick(object sender, EventArgs e) {
			sp = Cursor.Position;        // スクリーン座標の取得
			cp = this.PointToClient(sp); // クライアント座標に変換
			text21[0].Text = $"{sp.X.ToString()},{sp.Y.ToString()}";
			StringBuilder sb = new StringBuilder(65535);//65535に特に意味はない
			Win32Wrap.GetWindowText(Win32Wrap.GetForegroundWindow(), sb, 65535);
			text21[3].Text = $"{sb}";
			toolTip2.SetToolTip(text21[3], $"{sb}"); // ツールチップ更新

			IntPtr hWnd = Win32Wrap.GetForegroundWindow();        // 1. フォアグランドのウィンドウハンドルを取得
			int id = 0;
			try {
				Win32Wrap.GetWindowThreadProcessId(hWnd, ref id); // 2.ウィンドウハンドル→プロセスIDを取得
				text21[2].Text = Process.GetProcessById(id).ProcessName; // プロセスID → プロセス名

				Win32Wrap.RECT windowRect = new Win32Wrap.RECT();
				// 3.ハンドル→ウインドウの見た目通りのRectを取得する
				// (GetWindowRectだと見た目とのズレがあるため、DwmGetWindowAttribute()を使用。)
				Win32Wrap.DwmGetWindowAttribute(hWnd, Win32Wrap.DWMWA_EXTENDED_FRAME_BOUNDS, ref windowRect, 4 * 4);
				int width = windowRect.right - windowRect.left;
				int height = windowRect.bottom - windowRect.top;
				text21[1].Text = $"{((sp.X-windowRect.left).ToString())},{((sp.Y-windowRect.top).ToString())}";
				text21[4].Text = $"{width.ToString()}x{height.ToString()}";
				text21[5].Text = $"{windowRect.left.ToString()},{windowRect.top.ToString()}";
				text21[6].Text = $"{windowRect.right.ToString()},{windowRect.bottom.ToString()}";
				if (m_curId != id) {
					text31[4].Text = Process.GetProcessById(id).ProcessName;
					text31[5].Text = windowRect.left.ToString();
					text31[6].Text = windowRect.top.ToString();
					text31[7].Text = windowRect.right.ToString();
					text31[8].Text = windowRect.bottom.ToString();
				}
			}
			catch(Exception ex) {
				MessageBox.Show(ex.ToString());
			}
		}

	// #tabPage3
		private void button3_Click(object sender, EventArgs e){
			Button btn = (Button)sender;
			if (btn.Text == "部分キャプ") {
				int pt1x = Convert.ToInt32(text31[0].Text);
				int pt1y = Convert.ToInt32(text31[1].Text);
				int pt2x = Convert.ToInt32(text31[2].Text);
				int pt2y = Convert.ToInt32(text31[3].Text);
				this.CaptureScreen(pt1x, pt1y, pt2x, pt2y);
			}
			else if (btn.Text == "フルキャプ") {
				this.CaptureScreen(0, 0, 1920, 1080);
			}
			else if (btn.Text == "座標格納") {
				text31[0].Text = text31[5].Text;
				text31[1].Text = text31[6].Text;
				text31[2].Text = text31[7].Text;
				text31[3].Text = text31[8].Text;
			}
			else if (btn.Text == "On") {
				btn.Text = "Off";
			}
			else if (btn.Text == "Off") {
				btn.Text = "On";
			}
			else {
				MessageBox.Show($"『{btn.Text}』が押されました。");
			}
		}

		/// <summary>
		/// プライマリスクリーンの画像を取得する
		/// </summary>
		/// <returns>プライマリスクリーンの画像</returns>
		private void CaptureScreen(int p1x, int p1y, int p2x, int p2y) {
			//プライマリモニタのデバイスコンテキストを取得
			IntPtr disDC = Win32Wrap.GetDC(IntPtr.Zero);
			//Bitmapの作成
			Bitmap bmp = new Bitmap(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
			//Graphicsの作成
			Graphics g = Graphics.FromImage(bmp);
			//Graphicsのデバイスコンテキストを取得
			IntPtr hDC = g.GetHdc();
			//Bitmapに画像をコピーする
			Win32Wrap.BitBlt(hDC, 0, 0, bmp.Width, bmp.Height, disDC, 0, 0, Win32Wrap.SRCCOPY);
			//解放
			g.ReleaseHdc(hDC);
			g.Dispose();
			Win32Wrap.ReleaseDC(IntPtr.Zero, disDC);
			
			// 画像を切り抜く
			var ptx = p1x;
			var pty = p1y;
			var width = p2x - p1x;
			var height = p2y - p1y;
			Rectangle rect = new Rectangle(ptx, pty, width, height); // (左上x, 左上y, 幅, 高さ)
			Bitmap bmpNew = bmp.Clone(rect, bmp.PixelFormat);

			// 画像を保存
			DateTime dt = DateTime.Now;
			var capName = $@".\({p1x.ToString()}_{p1y.ToString()})_({p2x.ToString()}_{p2y.ToString()})_{dt.ToString(Win32Wrap.FORMAT_TIME)}.png";
			bmpNew.Save(capName);

			//(おまけ)画像ファイルのフルパスをクリップボードに
//			var absCapName = System.IO.Path.GetFullPath(capName).ToString();
//			Clipboard.SetText(absCapName); // スレッドがSTAでないとだめ。
			//(おまけ)画像をクリップボードに
			Clipboard.SetImage(bmpNew);

			// 画像リソースを解放
			bmp.Dispose();
			bmpNew.Dispose();

			// キャプチャ通知を飛ばす
			this.notifyIcon3.BalloonTipTitle = "キャプチャしました";
			this.notifyIcon3.BalloonTipText = capName;
			this.notifyIcon3.ShowBalloonTip(1000);

			return;
		}
	}
}
