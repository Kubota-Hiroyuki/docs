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
		Ready,// �A���[���ݒ�҂�
		Run,  // �A���[���N����
		Wait} // �A���[������

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
		Timer    timer1    = new Timer();   // �^�C�}�[ �N���X
		TextBox  label1   = new TextBox(); // ��������TextBox
		Button   btnExec  = new Button();  // ���s�E��~
		Button   btnInit  = new Button();  // ����������
		Button[] button1;                  // [0]�����{25��,[1] �����{5��, [2]�����{1��
		Button   btnRound = new Button();  // �b�j�؎̂�
		Mode     eMode    = Mode.Ready;    // �^�C�}�[�̏�ԑJ�� 
		CheckBox chkRemind= new CheckBox();

		// #tabPage2
		Timer    timer2    = new Timer();   // �^�C�}�[ �N���X
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
			// �t�H�[���̊J��/����Ɋւ���C�x���g
//			this.Load        += new EventHandler(this.Form_Load);
//			this.Shown       += new EventHandler(this.Form_Shown);
//			this.FormClosing += new FormClosingEventHandler(this.Form_FormClosing);
//			this.FormClosed  += new FormClosedEventHandler(this.Form_FormClosed);
			this.FormClosed += (sender, e)=>{
				notifyIcon3.Dispose(); // �^�X�N�g���C�̃A�C�R���̌�n��
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
			label21[0].Text = "Screen���W�F";
			label21[1].Text = "Client���W�F";
			label21[2].Text = "�v���Z�X���F";
			label21[3].Text = "Window Title�F";
			label21[4].Text = "Window Size �F";
			label21[5].Text = "Window Min�F";
			label21[6].Text = "Window Max�F";
			tabp2.Controls.AddRange(label21);
			tabp2.Controls.AddRange(text21);
			timer2.Interval = 10;
			timer2.Tick += new EventHandler(timer2_Tick);
			timer2.Start();

//			tabc.Selected += new TabControlEventHandler(tabControl1_SelectionChanged);
			tabc.Selected += (sender, e)=>{
				// �I�����ꂽ�^�u�̔ԍ����擾
				int idx = tabc.SelectedIndex + 1;
				if (idx == 1)
					this.Size = new Size(270, 170);
				else if (idx == 2)
					this.Size = new Size(270, 250);
				else
					this.Size = new Size(320, 200);
			};

			//ToolTip�̐ݒ���s��
			toolTip2.InitialDelay = 500;  //ToolTip���\�������܂ł̎���
			toolTip2.ReshowDelay = 1000;   //ToolTip���\������Ă��鎞�ɁA�ʂ�ToolTip��\������܂ł̎���
			toolTip2.AutoPopDelay = 10000; //ToolTip��\�����鎞��
			toolTip2.ShowAlways = true;    //�t�H�[�����A�N�e�B�u�łȂ����ł�ToolTip��\������
			toolTip2.SetToolTip(text21[3], "Default"); //�w��R���g���[����ToolTip���\�������悤�ɂ���

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
			label31[0].Text = "����xy�F";
			label31[1].Text = "�E��xy�F";
			label31[2].Text = "�v���Z�X���F";
			label31[3].Text = "cap����xy�F";
			label31[4].Text = "cap�E��xy�F";
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

			button3[0].Text = "�����L���v";
			button3[1].Text = "�t���L���v";
			button3[2].Text = "���W�i�[";
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
			label41[0].Text = "Label0�F";
			label41[1].Text = "Label1�F";
			label41[2].Text = "Label2�F";
			label41[3].Text = "Label3�F";
			label41[4].Text = "Label4�F";
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
//			MessageBox.Show("���߂ĕ\������钼�O�ɔ���", "Load�C�x���g");
//		}
//		private void Form_Shown(object sender, EventArgs e) {
//			MessageBox.Show("�t�H�[�����\�����ꂽ����", "Shown�C�x���g");
//		}
//		private void Form_FormClosing(object sender, FormClosingEventArgs e) {
//			DialogResult result = MessageBox.Show("�t�H�[�������钼�O�ɔ������܂��B\n���̒i�K�ł͂܂����鑀����L�����Z���ł���\n\n���Ă����ł���",
//			                                      "Closing�C�x���g",
//			                                      MessageBoxButtons.YesNo);
//			if (result != DialogResult.Yes)
//				e.Cancel = true;
//		}
//		private void Form_FormClosed(object sender, FormClosedEventArgs e) {
//			MessageBox.Show("�t�H�[������������ɔ������܂��B\n�Ō�ɓ��������������͂��̃^�C�~���O�ŋL�q�B", "FormClosed�C�x���g");
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

		//�R���g���[���̐F�ꊇ�ύX�̎Q�l��
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
			sp = Cursor.Position;        // �X�N���[�����W�̎擾
			cp = this.PointToClient(sp); // �N���C�A���g���W�ɕϊ�
			text21[0].Text = $"{sp.X.ToString()},{sp.Y.ToString()}";
			StringBuilder sb = new StringBuilder(65535);//65535�ɓ��ɈӖ��͂Ȃ�
			Win32Wrap.GetWindowText(Win32Wrap.GetForegroundWindow(), sb, 65535);
			text21[3].Text = $"{sb}";
			toolTip2.SetToolTip(text21[3], $"{sb}"); // �c�[���`�b�v�X�V

			IntPtr hWnd = Win32Wrap.GetForegroundWindow();        // 1. �t�H�A�O�����h�̃E�B���h�E�n���h�����擾
			int id = 0;
			try {
				Win32Wrap.GetWindowThreadProcessId(hWnd, ref id); // 2.�E�B���h�E�n���h�����v���Z�XID���擾
				text21[2].Text = Process.GetProcessById(id).ProcessName; // �v���Z�XID �� �v���Z�X��

				Win32Wrap.RECT windowRect = new Win32Wrap.RECT();
				// 3.�n���h�����E�C���h�E�̌����ڒʂ��Rect���擾����
				// (GetWindowRect���ƌ����ڂƂ̃Y�������邽�߁ADwmGetWindowAttribute()���g�p�B)
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
			if (btn.Text == "�����L���v") {
				int pt1x = Convert.ToInt32(text31[0].Text);
				int pt1y = Convert.ToInt32(text31[1].Text);
				int pt2x = Convert.ToInt32(text31[2].Text);
				int pt2y = Convert.ToInt32(text31[3].Text);
				this.CaptureScreen(pt1x, pt1y, pt2x, pt2y);
			}
			else if (btn.Text == "�t���L���v") {
				this.CaptureScreen(0, 0, 1920, 1080);
			}
			else if (btn.Text == "���W�i�[") {
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
				MessageBox.Show($"�w{btn.Text}�x��������܂����B");
			}
		}

		/// <summary>
		/// �v���C�}���X�N���[���̉摜���擾����
		/// </summary>
		/// <returns>�v���C�}���X�N���[���̉摜</returns>
		private void CaptureScreen(int p1x, int p1y, int p2x, int p2y) {
			//�v���C�}�����j�^�̃f�o�C�X�R���e�L�X�g���擾
			IntPtr disDC = Win32Wrap.GetDC(IntPtr.Zero);
			//Bitmap�̍쐬
			Bitmap bmp = new Bitmap(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
			//Graphics�̍쐬
			Graphics g = Graphics.FromImage(bmp);
			//Graphics�̃f�o�C�X�R���e�L�X�g���擾
			IntPtr hDC = g.GetHdc();
			//Bitmap�ɉ摜���R�s�[����
			Win32Wrap.BitBlt(hDC, 0, 0, bmp.Width, bmp.Height, disDC, 0, 0, Win32Wrap.SRCCOPY);
			//���
			g.ReleaseHdc(hDC);
			g.Dispose();
			Win32Wrap.ReleaseDC(IntPtr.Zero, disDC);
			
			// �摜��؂蔲��
			var ptx = p1x;
			var pty = p1y;
			var width = p2x - p1x;
			var height = p2y - p1y;
			Rectangle rect = new Rectangle(ptx, pty, width, height); // (����x, ����y, ��, ����)
			Bitmap bmpNew = bmp.Clone(rect, bmp.PixelFormat);

			// �摜��ۑ�
			DateTime dt = DateTime.Now;
			var capName = $@".\({p1x.ToString()}_{p1y.ToString()})_({p2x.ToString()}_{p2y.ToString()})_{dt.ToString(Win32Wrap.FORMAT_TIME)}.png";
			bmpNew.Save(capName);

			//(���܂�)�摜�t�@�C���̃t���p�X���N���b�v�{�[�h��
//			var absCapName = System.IO.Path.GetFullPath(capName).ToString();
//			Clipboard.SetText(absCapName); // �X���b�h��STA�łȂ��Ƃ��߁B
			//(���܂�)�摜���N���b�v�{�[�h��
			Clipboard.SetImage(bmpNew);

			// �摜���\�[�X�����
			bmp.Dispose();
			bmpNew.Dispose();

			// �L���v�`���ʒm���΂�
			this.notifyIcon3.BalloonTipTitle = "�L���v�`�����܂���";
			this.notifyIcon3.BalloonTipText = capName;
			this.notifyIcon3.ShowBalloonTip(1000);

			return;
		}
	}
}
