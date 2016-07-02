﻿using System;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace WB_Client
{
    public partial class Authorization : Form
    {
        static public int port = 8000;
        static public int wrong_pass_code = 100;
        static public int server_ok_code = 0;
        static public int authorize_code = 1;

        public Authorization()
        {
            InitializeComponent();
        }

        private void Authorization_Load(object sender, EventArgs e)
        {

        }

        private void Login_TextChanged(object sender, EventArgs e)
        {

        }

        private void Password_TextChanged(object sender, EventArgs e)
        {

        }

        public bool authorizationServer(int port)
        {
            byte[] bytes = new byte[1024];

            IPAddress ipAddr = IPAddress.Parse("127.1.1.1");
            IPEndPoint ipEndPoint = new IPEndPoint(ipAddr, port);
            Socket client = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            client.Connect(ipEndPoint);
            int loginLength = Login.Text.Length;
            int passwordLength = Password.Text.Length;
            client.Send(Encoding.UTF8.GetBytes(authorize_code.ToString()));
            client.Send(Encoding.UTF8.GetBytes(loginLength.ToString()));
            client.Send(Encoding.UTF8.GetBytes(Login.Text));
            client.Send(Encoding.UTF8.GetBytes(passwordLength.ToString()));
            client.Send(Encoding.UTF8.GetBytes(Password.Text));

            client.Receive(bytes);            

            if (bytes[0] == server_ok_code)
                return true;

            else if (bytes[0] == wrong_pass_code)
                return false;
            else
                return false;
        }

        private void Enter_Click(object sender, EventArgs e)
        {
            if (authorizationServer(port))
            {
                Menu menuShow = new Menu();
                menuShow.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Неправильное имя или пароль!");
            }
        }

        private void registration_Click(object sender, EventArgs e)
        {
            Registration registrationShow = new Registration();
            registrationShow.Show();
            this.Close();
        }
    }
}
