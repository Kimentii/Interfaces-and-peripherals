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
            //Starting our app and get the state of time of the battery
            start_app = true;
            previous_screenTime = get_screen_time();
            //Updating app
            update_data();
        }
        //method for getting current state of time
        public int get_screen_time()
        {
            //Describing process for cmd
            var proc_Cmd = new Process();
            proc_Cmd.StartInfo.UseShellExecute = false;
            proc_Cmd.StartInfo.RedirectStandardOutput = true;
            proc_Cmd.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            proc_Cmd.StartInfo.FileName = "cmd.exe";
            proc_Cmd.StartInfo.Arguments = "/c powercfg /q";
            proc_Cmd.Start();
            //From all output we need only VIDEOIDLE and last string
            var power_Config = proc_Cmd.StandardOutput.ReadToEnd();
            var last_String = new Regex("VIDEOIDLE.*\\n.*\\n.*\\n.*\\n.*\\n.*\\n.*");
            var video_Idle = last_String.Match(power_Config).Value;
            //After that we need only 16 value
            var battery_State = video_Idle.Substring(video_Idle.Length - 11).TrimEnd();
            //Convert string value to Int
            return Convert.ToInt32(battery_State, 16) / 60;
        }
        public void update_data()
        {
            //Получаем состояние заряда
            charging = SystemInformation.PowerStatus.PowerLineStatus.ToString();
            //If we start our app, then initialize it one time
            if (start_app)
            {
                previous_charging = charging;
                start_app = false;
            }
            //Getting the current percent of battery
            percent_battery = SystemInformation.PowerStatus.BatteryLifePercent * 100 + "%";
            //If we dont use AC adapter
            if (charging == "Offline")
            {
                //Then we can start calculating lifetime
                var calc_life = SystemInformation.PowerStatus.BatteryLifeRemaining;
                //If we dont get the lifetime, we continue calculating
                if (calc_life != -1)
                {
                    //Or initialize variable for storing it in formatted way
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

        //Method for changing time in system directly
        public void SetDisplayOff(int value)
        {
            //Describing process for cmd
            var p = new Process();
            p.StartInfo.FileName = "cmd.exe";
            p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.Arguments = "/c powercfg /x -monitor-timeout-dc " + value;
            p.Start();
        }
    }
}