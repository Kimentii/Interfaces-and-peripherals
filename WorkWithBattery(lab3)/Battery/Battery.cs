using System;
using System.Windows.Forms;

namespace Battery
{
    public partial class Battery : Form
    {
        private readonly BatteryManager _manager = new BatteryManager();

        public Battery()
        {
            InitializeComponent();
        }

        //Метод для инициализации состояния батареи
        private void battery_Load(object sender, EventArgs e)
        {
            //Получаем состояние батареи
            _manager.Init();
            if (_manager.charging == "Online")
                timeoutBox.Enabled = false;
            UpdateBattery(null, null);
            //Установка события таймера
            UpdateTimer.Tick += UpdateBattery;              //Добавляем метод, который будет вызывать таймер
            UpdateTimer.Interval = 2000;
            UpdateTimer.Start();

            timeoutBox.SelectedIndex = timeoutBox.FindString(_manager.previous_screenTime.ToString());
            timeoutLabel.Text = "Display time: ";
        }

        private void UpdateBattery(object sender, EventArgs e)
        {
            _manager.update_data();
            State.Text = _manager.charging;
            Percentage.Text = _manager.percent_battery;
            timeLeft.Text = _manager.work_time;
            //Проверка нужно ли нам разблокировать спикок.
            if (_manager.previous_charging != State.Text)
            {
                if (_manager.previous_charging == "Offline")
                {
                    timeoutBox.Enabled = false;
                }
                else
                {
                    timeoutBox.Enabled = true;
                }
                _manager.previous_charging = State.Text;
            }
        }

        private void AppClosing(object sender, FormClosingEventArgs e)
        {
            timeoutBox.SelectedIndex = timeoutBox.FindString(_manager.previous_screenTime.ToString());

            BatteryManager battery = new BatteryManager();
            battery.SetDisplayOff(Int32.Parse(_manager.previous_screenTime.ToString()));
        }

        private void timeoutBox_ValueChanged(object sender, EventArgs e)
        {
            timeoutLabel.Text = "Display time: " + timeoutBox.SelectedItem.ToString() + " minutes";
            _manager.SetDisplayOff(Int32.Parse(timeoutBox.SelectedItem.ToString()));
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void timeoutBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void State_TextChanged(object sender, EventArgs e)
        {

        }

        private void Percentage_TextChanged(object sender, EventArgs e)
        {

        }
    }
}