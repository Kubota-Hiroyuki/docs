using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

using MouseHook1.Utilities;
using KeyboardHook1.Utilities.Gui;

namespace FormWithButton {
	public class Form1 : Form {
		public Button button1;
		public Label  label1;
		public KeyboardHook keyboardHook;
		public MouseHook    mouseHook;
		public bool isHookEvent; 

		public Form1() {
			this.SuspendLayout();
			button1 = new Button();
			button1.Size = new Size(100, 30);
			button1.Location = new Point(20, 20);
			button1.Text = "buton";
			this.Controls.Add(button1);
			label1 = new Label();
			label1.Size = new Size(100, 30);
			label1.Location = new Point(20, 50);
			label1.Text = "text";
			this.Controls.Add(label1);
			this.ResumeLayout(false);

			this.Load     += new EventHandler(Form1_Load);
			this.Closed   += new EventHandler(Form1_FormClosed);
			button1.Click += new EventHandler(button1_Click);
		}

		private void Form1_Load(object sender, EventArgs e) {
			isHookEvent = false;
			keyboardHook = new KeyboardHook();
			mouseHook = new MouseHook();
			button1.Text = "hook停止中";
		}

		private void Form1_FormClosed(object sender, EventArgs e) {
			MessageBox.Show("終了");
		}


		private void button1_Click(object sender, EventArgs e) {
			isHookEvent = !isHookEvent;
			button1.Text = "hook停止中";
			keyboardHook.KeyboardHooked -= new KeyboardHookedEventHandler(keyHookProc);
			mouseHook.MouseHooked       -= new MouseHookedEventHandler(mouseHookProc);
			if (isHookEvent) {
				keyboardHook.KeyboardHooked += new KeyboardHookedEventHandler(keyHookProc);
				mouseHook.MouseHooked       += new MouseHookedEventHandler(mouseHookProc);
				button1.Text = "hook起動中";
			}

		}

		private void mouseHookProc(object sender, MouseHookedEventArgs e) {
			if (e.Message == MouseMessage.LDown) {
				label1.Text = $"[左]クリックdownを検知";
			}
			else if (e.Message == MouseMessage.LUp) {
				label1.Text = $"[左]クリックUpを検知";
			}
		}

		private void keyHookProc(object sender, KeyboardHookedEventArgs e) {
			if(e.UpDown == KeyboardUpDown.Down) {
				if(e.KeyCode == Keys.Space) {
					label1.Text = $"[Space]キーDownを検知";
				}
				else {
					label1.Text = $"[他]キーDownを検知";
				}
			}
			else if(e.UpDown == KeyboardUpDown.Up) {
				if(e.KeyCode == Keys.Space) {
					label1.Text = $"[Space]キーUpを検知";
				}
				else {
					label1.Text = $"[他]キーUpを検知";
				}
			}
		}

	}

	public class Program {
		[STAThread]
		static void Main() {
			Application.EnableVisualStyles();
			Application.Run(new Form1());
		}
	}
}

