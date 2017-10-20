using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Battery
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            // Включает визуальные стили для приложения.
            Application.EnableVisualStyles();
            // Задает значения по умолчанию во всем приложении для свойства UseCompatibleTextRendering, 
            // определенного в конкретных элементах управления.
            Application.SetCompatibleTextRenderingDefault(false);
            // Запускает стандартный цикл обработки сообщений приложения в текущем потоке и делает указанную форму видимой.
            Application.Run(new Battery());
        }
    }
}