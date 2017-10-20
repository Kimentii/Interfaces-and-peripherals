using System;
using System.Windows.Forms;

namespace Battery
{
    public partial class Battery : Form
    {
        private readonly BatteryManager m_Manager = new BatteryManager();

        public Battery()
        {
            InitializeComponent();
        }

        // Метод для инициализации состояния батареи.
        private void BatteryLoad(object sender, EventArgs e)
        {
            // Получаем состояние батареи.
            m_Manager.Init();
            if (m_Manager.Charging == "Online")
                timeoutBox.Enabled = false;
            UpdateBattery(null, null);
            // Установка события таймера.
            // Добавляем метод, который будет вызывать таймер.
            UpdateTimer.Tick += UpdateBattery;
            UpdateTimer.Interval = 2000;
            UpdateTimer.Start();

            timeoutBox.SelectedIndex = timeoutBox.FindString(m_Manager.PreviousScreenTime.ToString());
            timeoutLabel.Text = "Display time: ";
        }

        private void UpdateBattery(object sender, EventArgs e)
        {
            m_Manager.UpdateData();
            State.Text = m_Manager.Charging;
            Percentage.Text = m_Manager.PercentBattery;
            timeLeft.Text = m_Manager.WorkTime;
            // Проверка нужно ли нам разблокировать спикок.
            if (m_Manager.PreviousCharging != State.Text)
            {
                if (m_Manager.PreviousCharging == "Offline")
                {
                    timeoutBox.Enabled = false;
                }
                else
                {
                    timeoutBox.Enabled = true;
                }
                m_Manager.PreviousCharging = State.Text;
            }
        }

        private void AppClosing(object sender, FormClosingEventArgs e)
        {
            timeoutBox.SelectedIndex = timeoutBox.FindString(m_Manager.PreviousScreenTime.ToString());

            BatteryManager battery = new BatteryManager();
            battery.SetDisplayOff(Int32.Parse(m_Manager.PreviousScreenTime.ToString()));
        }

        private void TimeoutBoxValueChanged(object sender, EventArgs e)
        {
            timeoutLabel.Text = "Display time: " + timeoutBox.SelectedItem.ToString() + " minutes";
            m_Manager.SetDisplayOff(Int32.Parse(timeoutBox.SelectedItem.ToString()));
        }
    }
}