using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace TabletDriverGUI
{
    internal class ProcessFocusWatchdog
    {
        /// <summary>
        /// Constructs a <see cref="ProcessFocusWatchdog"/> with defaults.
        /// </summary>
        public ProcessFocusWatchdog()
        {
            Dispatcher.Tick += Dispatcher_Tick;
            UseList = false;
        }

        /// <summary>
        /// Constructs a <see cref="ProcessFocusWatchdog"/> using <paramref name="processNames"/> as the process list.
        /// </summary>
        /// <param name="processNames"></param>
        public ProcessFocusWatchdog(IEnumerable<string> processNames) : this()
        {
            ProcessNames = processNames.ToList();
            UseList = true;
        }

        /// <summary>
        /// Event raised when the focused process is changed.
        /// </summary>
        public event EventHandler<Process> ProcessFocused;

        #region Properties

        /// <summary>
        /// List of processes that will publish the <see cref="ProcessFocused"/> event when focused.
        /// </summary>
        public List<string> ProcessNames { set; get; } = new List<string>();

        /// <summary>
        /// If true, only processes in <see cref="ProcessNames"/> will push an <see cref="ProcessFocused"/> event.
        /// </summary>
        public bool UseList { set; get; }

        #endregion

        #region External Controls

        /// <summary>
        /// Internal dispatch timer.
        /// </summary>
        private DispatcherTimer Dispatcher = new DispatcherTimer()
        {
            Interval = TimeSpan.FromSeconds(5),
            Tag = "focusWatchdog",
            IsEnabled = false,
        };

        /// <summary>
        /// Starts the dispatcher.
        /// </summary>
        public void Start()
        {
            Dispatcher.Start();
        }

        /// <summary>
        /// Stops the dispatcher.
        /// </summary>
        public void Stop()
        {
            Dispatcher.Stop();
        }

        /// <summary>
        /// Controls dispatcher activity.
        /// </summary>
        public bool IsEnabled
        {
            set
            {
                if (Dispatcher.IsEnabled && value == false)
                    Dispatcher.Stop();
                else if (Dispatcher.IsEnabled == false && value == true)
                    Dispatcher.Start();
            }
            get => Dispatcher.IsEnabled;
        }

        #endregion

        /// <summary>
        /// Last focused process.
        /// </summary>
        private Process _lastproc = null;

        private void Dispatcher_Tick(object sender, EventArgs e)
        {
            int focalWindowId = NativeMethods.GetForegroundWindow().ToInt32();
            var proc = Process.GetProcessById(focalWindowId);

            if (UseList && ProcessNames.Contains(proc.ProcessName) && proc != _lastproc && ProcessNames.Count > 0)
                PushProcess(proc);
            else if (proc != _lastproc)
                PushProcess(proc);
        }

        /// <summary>
        /// Pushes the process for the <see cref="ProcessFocused"/> event.
        /// </summary>
        /// <param name="proc"></param>
        private void PushProcess(Process proc)
        {
            ProcessFocused?.Invoke(this, proc);
            _lastproc = proc;
        }
    }
}
