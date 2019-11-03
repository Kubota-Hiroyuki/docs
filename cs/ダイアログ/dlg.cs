using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace FormWithButton {
	public class Form1 : Form {
		public Button button1;
		public ListBox listBox1;
		public ListBox listBox2;

		public Form1() {
			this.SuspendLayout();
			this.AutoSize = true;
//			this.Size = new Size(200, 150);
			button1 = new Button();
			button1.Size = new Size(60, 30);
			button1.Location = new Point(20, 20);
			button1.Text = "Click me";
			this.Controls.Add(button1);
			button1.Click += new EventHandler(button1_Click);

			listBox1 = new ListBox();
			listBox1.Size = new System.Drawing.Size(200, 100);
			listBox1.Location = new System.Drawing.Point(20, 60);
			this.Controls.Add(listBox1);
			listBox1.MultiColumn = true; // true=複数選択可能
			listBox1.SelectionMode = SelectionMode.MultiExtended; // SelectionMode.MultiExtended = 横スクロール可

			listBox1.BeginUpdate();
			for (int x = 1; x <= 10; x++) {
				listBox1.Items.Add("Item " + x.ToString());
			}
			listBox1.EndUpdate();
			listBox1.SetSelected(1, true); // Item1,3,5(2,4,6番目)を選択状態に
			listBox1.SetSelected(3, true);
			listBox1.SetSelected(5, true);

			listBox2 = new ListBox();
			listBox2.Size = new Size(200, 100);
			listBox2.Location = new Point(300, 60);
			this.Controls.Add(listBox2);
			listBox2.BeginUpdate();
			for (int x = 1; x <= 10; x++) {
				listBox2.Items.Add("Item " + x.ToString());
			}
			listBox2.EndUpdate();
			this.ResumeLayout(false);
			
			this.listBox1.SelectedIndexChanged += new EventHandler(listBox1_SelectedIndexChanged);
		}

private void listBox1_SelectedIndexChanged(object sender, EventArgs e) {
	string curItem = listBox1.SelectedItem.ToString();
	int index = listBox2.FindString(curItem);
	MessageBox.Show(curItem + "_" + index.ToString());
	if(index == -1)
		MessageBox.Show("Item is not available in ListBox2");
	else
		listBox2.SetSelected(index,true);
}

		private void button1_Click(object sender, EventArgs e) {
			var msg = listBox1.SelectedItems[1].ToString() + "\n" // 選択された項目のコレクション(ListBox.SelectedObjectCollection)
			        + listBox1.SelectedIndices[0].ToString();     // 選択された項目のインテッククス番号コレクション(ListBox.SelectedIndexCollection)
			MessageBox.Show(msg);
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

