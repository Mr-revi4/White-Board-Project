﻿using System;
using System.Windows.Forms;

namespace WB_Client
{
    static class Program
    {
        
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Authorization());
            Application.Run(new Registration());
            Application.Run(new Menu());
            Application.Run(new Board());
        }
    }
}
