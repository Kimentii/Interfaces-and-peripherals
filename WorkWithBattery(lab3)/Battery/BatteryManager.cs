using System;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace Battery
{
    public class BatteryManager
    {
        public int PreviousScreenTime { get; set; }
        public string Charging { get; set; }
        public string PercentBattery { get; set; }
        public string WorkTime { get; set; }
        public string PreviousCharging { get; set; }
        public bool StartApp { get; set; }

        public void Init()
        {
            StartApp = true;
            PreviousScreenTime = GetScreenTime();
            UpdateData();
        }

        public int GetScreenTime()
        {
            // Запуск консоли.
            var processCmd = new Process();
            processCmd.StartInfo.UseShellExecute = false;                         //Не нужно использовать оболочку системы для запуска процесса
            processCmd.StartInfo.RedirectStandardOutput = true;                   //Нужно записывать данные приложения в поток
            processCmd.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;         //Стиль окна
            processCmd.StartInfo.FileName = "cmd.exe";
            // Суть: /c - выполнение указанной команды с последующим завершением. powercfg - программа для выполнения, /q - аргумент
            // программе powercfg, указывающий вывести содержимое схемы управления питанием.
            processCmd.StartInfo.Arguments = "/c powercfg /q";
            processCmd.Start();
            // Нам нужно только VIDEOIDLE и его последняя строка
            var powerConfig = processCmd.StandardOutput.ReadToEnd();             //Считываем все символы с текущей позиции до конца.
            var lastString = new Regex("VIDEOIDLE.*\\n.*\\n.*\\n.*\\n.*\\n.*\\n.*");
            var videoIdle = lastString.Match(powerConfig).Value;
            // Нам нужно только 16 значений.
            var battery_State = videoIdle.Substring(videoIdle.Length - 11).TrimEnd();
            // Преобразуем строку в число.
            return Convert.ToInt32(battery_State, 16) / 60;
        }
        public void UpdateData()
        {
            // Получаем состояние зарядки/не зарядки.
            Charging = SystemInformation.PowerStatus.PowerLineStatus.ToString();
            // Иницилиазируем состояние заряда один раз.
            if (StartApp)
            {
                PreviousCharging = Charging;
                StartApp = false;
            }
            // Получаем текущий процент батареии.
            PercentBattery = SystemInformation.PowerStatus.BatteryLifePercent * 100 + "%";
            // Если зарядное устройство не подключено.
            if (Charging == "Offline")
            {
                // Тогда вычисляем оставшееся время жизни.
                var calcLife = SystemInformation.PowerStatus.BatteryLifeRemaining;
                // Если мы не получили релуьтата, то впродолжаем вычислять.
                if (calcLife != -1)
                {
                    // Если оставшееся время вычислено, то выводим его.
                    var span = new TimeSpan(0, 0, calcLife);
                    WorkTime = span.ToString("g");
                }
                else WorkTime = "Calculating";
            }
            else
            {
                WorkTime = "AC";
            }
        }

        // Метод для изменения вермени отключения экрана.
        public void SetDisplayOff(int value)
        {
            // Используем для выключения экрана cmd.
            var p = new Process();
            p.StartInfo.FileName = "cmd.exe";
            p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.Arguments = "/c powercfg /x -monitor-timeout-dc " + value;
            p.Start();
        }
    }
}