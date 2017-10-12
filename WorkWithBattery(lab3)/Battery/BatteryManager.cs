using System;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace Battery
{
    public class BatteryManager
    {
        public int previous_screenTime { get; set; }
        public string charging { get; set; }
        public string percent_battery { get; set; }
        public string work_time { get; set; }
        public string previous_charging { get; set; }
        public bool start_app { get; set; }

        public void Init()
        {
            start_app = true;
            previous_screenTime = get_screen_time();
            update_data();
        }
        //method for getting current state of time
        public int get_screen_time()
        {
            //Запуск консоли
            var proc_Cmd = new Process();
            proc_Cmd.StartInfo.UseShellExecute = false;                         //Не нужно использовать оболочку системы для запуска процесса
            proc_Cmd.StartInfo.RedirectStandardOutput = true;                   //Нужно записывать данные приложения в поток
            proc_Cmd.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;         //Стиль окна
            proc_Cmd.StartInfo.FileName = "cmd.exe";
            //Суть: /c - выполнение указанной команды с последующим завершением. powercfg - программа для выполнения, /q - аргумент
            //программе powercfg, указывающий вывести содержимое схемы управления питанием.
            proc_Cmd.StartInfo.Arguments = "/c powercfg /q";
            proc_Cmd.Start();
            //Нам нужно только VIDEOIDLE и его последняя строка
            var power_Config = proc_Cmd.StandardOutput.ReadToEnd();             //Считываем все символы с текущей позиции до конца.
            var last_String = new Regex("VIDEOIDLE.*\\n.*\\n.*\\n.*\\n.*\\n.*\\n.*");
            var video_Idle = last_String.Match(power_Config).Value;
            //Нам нужно только 16 значений
            var battery_State = video_Idle.Substring(video_Idle.Length - 11).TrimEnd();
            //Преобразуем строку в число
            return Convert.ToInt32(battery_State, 16) / 60;
        }
        public void update_data()
        {
            //Получаем состояние зарядки/не зарядки
            charging = SystemInformation.PowerStatus.PowerLineStatus.ToString();
            //Иницилиазируем состояние заряда один раз
            if (start_app)
            {
                previous_charging = charging;
                start_app = false;
            }
            //Получаем текущий процент батареии
            percent_battery = SystemInformation.PowerStatus.BatteryLifePercent * 100 + "%";
            //Если зарядное устройство не подключено
            if (charging == "Offline")
            {
                //Тогда вычисляем оставшееся время жизни
                var calc_life = SystemInformation.PowerStatus.BatteryLifeRemaining;
                //Если мы не получили релуьтата, то впродолжаем вычислять
                if (calc_life != -1)
                {
                    //Если оставшееся время вычислено, то выводим его
                    var span = new TimeSpan(0, 0, calc_life);
                    work_time = span.ToString("g");
                }
                else work_time = "Calculating";
            }
            else
            {
                work_time = "AC";
            }
        }

        //Метод для изменения вермени отключения экрана
        public void SetDisplayOff(int value)
        {
            //Используем для выключения экрана cmd
            var p = new Process();
            p.StartInfo.FileName = "cmd.exe";
            p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.Arguments = "/c powercfg /x -monitor-timeout-dc " + value;
            p.Start();
        }
    }
}