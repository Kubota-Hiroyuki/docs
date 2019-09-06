using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

// http://hongliang.seesaa.net/

namespace MouseHook1.Utilities {
	///<summary>�}�E�X�����͂��ꂽ�Ƃ��Ɏ��s����郁�\�b�h��\���C�x���g�n���h���B</summary>
	public delegate void MouseHookedEventHandler(object sender, MouseHookedEventArgs e);
	///<summary>MouseHooked�C�x���g�̃f�[�^��񋟂���B</summary>
	public class MouseHookedEventArgs : EventArgs {
		///<summary>
		///�V�����C���X�^���X���쐬����B
		///</summary>
		///<param name="message">�}�E�X����̎�ނ�\��MouseMessage�l�̈�B</param>
		///<param name="state">�}�E�X�̏�Ԃ�\��MouseState�\���́B</param>
		internal MouseHookedEventArgs(MouseMessage message, ref MouseState state) {
			this.message = message;
			this.state = state;
		}
		private MouseMessage message;
		private MouseState state;
		///<summary>�}�E�X����̎�ނ�\��MouseMessage�l�B</summary>
		public MouseMessage Message {get {return message;}}
		///<summary>�X�N���[�����W�ɂ����錻�݂̃}�E�X�J�[�\���̈ʒu�B</summary>
		public Point Point {get {return state.Point;}}
		///<summary>�z�C�[���̏���\��WheelData�\���́B</summary>
		public WheelData WheelData {get {return state.WheelData;}}
		///<summary>XButton�̏���\��XButtonData�\���́B</summary>
		public XButtonData XButtonData {get {return state.XButtonData;}}
	}
	///<summary>�}�E�X����̎�ނ�\���B</summary>
	public enum MouseMessage {
		///<summary>�}�E�X�J�[�\�����ړ������B</summary>
		Move         = 0x200,
		///<summary>���{�^���������ꂽ�B</summary>
		LDown        = 0x201,
		///<summary>���{�^����������ꂽ�B</summary>
		LUp          = 0x202,
		///<summary>�E�{�^���������ꂽ�B</summary>
		RDown        = 0x204,
		///<summary>���{�^����������ꂽ�B</summary>
		RUp          = 0x205,
		///<summary>���{�^���������ꂽ�B</summary>
		MDown        = 0x207,
		///<summary>���{�^����������ꂽ�B</summary>
		MUp          = 0x208,
		///<summary>�z�C�[������]�����B</summary>
		Wheel        = 0x20A,
		///<summary>X�{�^���������ꂽ�B</summary>
		XDown        = 0x20B,
		///<summary>X�{�^����������ꂽ�B</summary>
		XUp          = 0x20C,
	}
	///<summary>�}�E�X�̏�Ԃ�\���B</summary>
	[StructLayout(LayoutKind.Explicit)] internal struct MouseState {
		///<summary>�X�N���[�����W�ɂ��}�E�X�J�[�\���̌��݈ʒu�B</summary>
		[FieldOffset(0)]
		public Point Point;
		///<summary>message��MouseMessage.Wheel�̎��ɂ��̏ڍ׃f�[�^�����B</summary>
		[FieldOffset(8)]
		public WheelData WheelData;
		///<summary>message��MouseMessage.XDown/MouseMessage.XUp�̎��ɂ��̏ڍ׃f�[�^�����B</summary>
		[FieldOffset(8)]
		public XButtonData XButtonData;
		///<summary>�}�E�X�̃C�x���g�C���W�F�N�g�B</summary>
		[FieldOffset(12)]
		public MouseStateFlag Flag;
		///<summary>���b�Z�[�W������ꂽ�Ƃ��̎���</summary>
		[FieldOffset(16)]
		public int Time;
		///<summary>���b�Z�[�W�Ɋ֘A�Â���ꂽ�g�����</summary>
		[FieldOffset(20)]
		public IntPtr ExtraInfo;
	}
	///<summary>�}�E�X�z�C�[���̏�Ԃ̏ڍׂ�\���B</summary>
	public struct WheelData {
		///<summary>�r�b�g�f�[�^�B</summary>
		public int State;
		///<summary>�z�C�[���̉�]�ꍏ�݂�\���B</summary>
		public static readonly int OneWheel = 120;
		///<summary>�z�C�[���̉�]�ʂ�\���B�N���b�N���ꂽ�Ƃ���-1�B</summary>
		public int WheelDelta {
			get {
				int delta = State >> 16;
				return (delta < 0) ? -delta : delta;
			}
		}
		///<summary>�z�C�[�����ꍏ�ݕ��������ꂽ���ǂ�����\���B</summary>
		public bool IsOneWheel {get {return (State >> 16) == OneWheel;}}
		///<summary>�z�C�[���̉�]������\���B</summary>
		public WheelDirection Direction {
			get {
				int delta = State >> 16;
				if (delta == 0) return WheelDirection.None;
				return (delta < 0) ? WheelDirection.Backward : WheelDirection.Forward;
			}
		}
	}
	///<summary>�z�C�[���̉�]������\���B</summary>
	public enum WheelDirection {
		///<summary>��]���Ă��Ȃ��B</summary>
		None     =  0,
		///<summary>���[�U���痣�������։�]�����B</summary>
		Forward  =  1,
		///<summary>���[�U�ɋ߂Â������։�]�����B</summary>
		Backward = -1,
	}
	///<summary>X�{�^���̏�Ԃ̏ڍׂ�\���B</summary>
	public struct XButtonData {
		///<summary>�r�b�g�f�[�^�B</summary>
		public int State;
		///<summary>���삳�ꂽ�{�^���������B</summary>
		public int ControlledButton {get {return State >> 16;}}
		///<summary>X�{�^��1�������ꂽ���ǂ����������B</summary>
		public bool IsXButton1 {get {return (State >> 16) == 1;}}
		///<summary>X�{�^��2�������ꂽ���ǂ����������B</summary>
		public bool IsXButton2 {get {return (State >> 16) == 2;}}
	}
	///<summary>�}�E�X�̏�Ԃ�⑫����B</summary>
	internal struct MouseStateFlag {
		///<summary>�r�b�g�f�[�^�B</summary>
		public int Flag;
		///<summary>�C�x���g���C���W�F�N�g���ꂽ���ǂ�����\���B</summary>
		public bool IsInjected {
			get {return (Flag & 1) != 0;}
			set {Flag = value ? (Flag | 1) : (Flag & ~1);}
		}
	}
	///<summary>�}�E�X���t�b�N���A�C�ӂ̃��\�b�h�����s����B</summary>
	public class MouseHook : System.ComponentModel.Component {
		[DllImport("user32.dll", SetLastError=true)]
		private static extern IntPtr SetWindowsHookEx(int hookType, MouseHookDelegate hookDelegate, IntPtr hInstance, uint threadId);
		[DllImport("user32.dll", SetLastError=true)]
		private static extern int CallNextHookEx(IntPtr hook, int code, MouseMessage message, ref MouseState state);
		[DllImport("user32.dll", SetLastError=true)]
		private static extern bool UnhookWindowsHookEx(IntPtr hook);
		private const int MouseLowLevelHook = 14;
		private delegate int MouseHookDelegate(int code, MouseMessage message, ref MouseState state);
		private MouseHookDelegate hookDelegate;
		private IntPtr hook;
		private static readonly object EventMouseHooked = new object();
		///<summary>�}�E�X�����͂��ꂽ�Ƃ��ɔ�������B</summary>
		public event MouseHookedEventHandler MouseHooked  {
			add {base.Events.AddHandler(EventMouseHooked, value);}
			remove {base.Events.RemoveHandler(EventMouseHooked, value);}
		}
		///<summary>
		///�C���X�^���X���쐬����B
		///</summary>
		public MouseHook() {
			if (Environment.OSVersion.Platform != PlatformID.Win32NT)
				throw new PlatformNotSupportedException("Windows 98/Me�ł̓T�|�[�g����Ă��܂���B");
			this.hookDelegate = new MouseHookDelegate(CallNextHook);
			IntPtr module = Marshal.GetHINSTANCE(typeof(MouseHook).Assembly.GetModules()[0]);
			hook = SetWindowsHookEx(MouseLowLevelHook, hookDelegate, module, 0);
		}
		///<summary>
		///�}�E�X�����͂��ꂽ�Ƃ��Ɏ��s����f���Q�[�g���w�肵�ăC���X�^���X���쐬����B
		///</summary>
		///<param name="handler">�}�E�X�����͂��ꂽ�Ƃ��Ɏ��s���郁�\�b�h��\���f���Q�[�g�B</param>
		public MouseHook(MouseHookedEventHandler handler) : this() {
			this.MouseHooked += handler;
		}
		///<summary>
		///MouseHooked�C�x���g�𔭐�������B
		///</summary>
		///<param name="e">�C�x���g�̃f�[�^�B</param>
		protected virtual void OnMouseHooked(MouseHookedEventArgs e) {
			MouseHookedEventHandler invoked = Events[EventMouseHooked] as MouseHookedEventHandler;
			if (invoked != null)
				invoked(this, e);
		}
		private int CallNextHook(int code, MouseMessage message, ref MouseState state) {
			if (code >= 0) {
				OnMouseHooked(new MouseHookedEventArgs(message, ref state));
			}
			return CallNextHookEx(hook, code, message, ref state);
		}
		///<summary>
		///�g�p����Ă���A���}�l�[�W���\�[�X��������A�I�v�V�����Ń}�l�[�W���\�[�X���������B
		///</summary>
		///<param name="disposing">�}�l�[�W���\�[�X���������ꍇ��true�B</param>
		protected override void Dispose(bool disposing) {
			if (!disposed) {
				disposed = true;
				UnhookWindowsHookEx(hook);
				hook = IntPtr.Zero;
				base.Dispose(disposing);
			}
		}
		private bool disposed = false;
	}
}