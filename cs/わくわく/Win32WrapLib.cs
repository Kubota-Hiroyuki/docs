using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Win32WrapLib {
	public static class Win32Wrap {

		public const int	SRCCOPY	 = 13369376;
		public const int	CAPTUREBLT  = 1073741824;
		public const string FORMAT_TIME = "yyyyMMddhhmmss";
		[DllImport("user32.dll")]
		public static extern IntPtr GetForegroundWindow();
		[DllImport("user32.dll", EntryPoint = "GetWindowText", CharSet = CharSet.Auto)]
		public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
		[DllImport("user32.dll")]
		public static extern int GetWindowThreadProcessId(IntPtr hWnd, ref int lpdwProcessId);
		[DllImport("user32.dll")]
		public static extern bool GetWindowRect(IntPtr hwnd, ref RECT lpRect);
		[DllImport("user32.dll")]
		public static extern IntPtr GetWindowDC(IntPtr hwnd);
		[StructLayout(LayoutKind.Sequential)]
		public struct RECT {
			public int left;
			public int top;
			public int right;
			public int bottom;
		}
		[DllImport("user32.dll")]
		public  static extern IntPtr GetDC(IntPtr hwnd);
		[DllImport("gdi32.dll")]
		public  static extern int BitBlt(IntPtr hDestDC,
			int x,
			int y,
			int nWidth,
			int nHeight,
			IntPtr hSrcDC,
			int xSrc,
			int ySrc,
			int dwRop);
		[DllImport("user32.dll")]
		public  static extern IntPtr ReleaseDC(IntPtr hwnd, IntPtr hdc);

		[DllImport("dwmapi.dll")]
		public static extern int DwmGetWindowAttribute(IntPtr hwnd, int dwAttribute, ref RECT rect, int cbAttribute);
		public const int DWMWA_EXTENDED_FRAME_BOUNDS = 9; 

	}
}