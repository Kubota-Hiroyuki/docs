using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace FormWithButton {
	public class Form1 : Form {
		public Button button1;
private StatusStrip          statusStrip1;
private ToolStripStatusLabel toolStripStatusLabel1;
private ToolStripStatusLabel toolStripStatusLabel2;

		public Form1() {
			this.SuspendLayout();
			this.Size = new Size(200, 150);
			button1 = new Button();
			button1.Size = new Size(60, 30);
			button1.Location = new Point(20, 20);
			button1.Text = "Click me";
			this.Controls.Add(button1);
			button1.Click += new EventHandler(button1_Click);
			this.ResumeLayout(false);

statusStrip1          = new System.Windows.Forms.StatusStrip();
toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
// statusStrip1
statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
	toolStripStatusLabel1, toolStripStatusLabel2}
);
//statusStrip1.Location = new System.Drawing.Point(0, 248);
//statusStrip1.Name = "statusStrip1";
//statusStrip1.Size = new System.Drawing.Size(292, 25);
//statusStrip1.TabIndex = 0;
//statusStrip1.Text = "statusStrip1";

// toolStripStatusLabel1
//toolStripStatusLabel1.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Top)
//            | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)
//            | System.Windows.Forms.ToolStripStatusLabelBorderSides.Bottom)));
toolStripStatusLabel1.BorderStyle = System.Windows.Forms.Border3DStyle.Raised;
//toolStripStatusLabel1.IsLink = true;
//toolStripStatusLabel1.Name = "toolStripStatusLabel1";
//toolStripStatusLabel1.Size = new System.Drawing.Size(246, 20);
//toolStripStatusLabel1.Spring = true;
toolStripStatusLabel1.Text = "ステータス";
//toolStripStatusLabel1.Alignment = ToolStripItemAlignment.Left;
// Form1
// 
Controls.Add(statusStrip1);
//statusStrip1.ResumeLayout(false);
//statusStrip1.PerformLayout();
//ResumeLayout(false);
//PerformLayout();

toolStripStatusLabel2.Text = "バー";

		}

		private void button1_Click(object sender, EventArgs e) {
			MessageBox.Show("Clicked");
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

