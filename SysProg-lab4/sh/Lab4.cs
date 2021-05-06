﻿using System;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Drawing;
using System.Text;
using System.Linq;
using System.Data;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;


namespace l1
{
    public partial class Lab4 : Form
    {
        public int thread_id = 1;

        [DllImport("..\\NamedPipes.dll", CharSet = CharSet.Ansi)]
        private static extern void Send(int evType, int evId, StringBuilder msg);

        [DllImport("..\\NamedPipes.dll", CharSet = CharSet.Ansi)]
        private static extern void StartThread(int evType);

        [DllImport("..\\NamedPipes.dll", CharSet = CharSet.Ansi)]
        private static extern void StopThread(int evType);

        [DllImport("..\\NamedPipes.dll", CharSet = CharSet.Ansi)]
        private static extern void StartServer(int evType);

        [DllImport("..\\NamedPipes.dll", CharSet = CharSet.Ansi)]
        private static extern void PassTo(int evType, int thId = 0, StringBuilder msg = null);

        bool Child = false;

        public Lab4() { InitializeComponent(); }


        private void Start_Click(object sender, EventArgs e)
        {
            int thread_number = (int)thread_count.Value;
            if (Child == false)
            {
                StartServer(0);
                listBox1.Items.Add("All Threads\n");
                listBox1.Items.Add("Main Thread\n");
                Child = true;
            }
            else
            {
                StringBuilder start = new StringBuilder("Start");
                for (int i = 0; i < thread_number; i++)
                {
                    StartThread(0);
                    listBox1.Items.Add("id " + thread_id++.ToString() + "\n");
                }
            }
        }

        private void Stop_click(object sender, EventArgs e)
        {

            if (!(Child == true))
                return;
            StopThread(1);

            if (thread_id > 1)
                listBox1.Items.RemoveAt(thread_id--);
            else
            {
                Child = false;
                listBox1.Items.Clear();

            }
        }


        private void SendButton_Click(object sender, EventArgs e)
        {
            StringBuilder fileText = new StringBuilder(textBox1.Text, Encoding.Unicode.GetBytes(textBox1.Text).Length);
            var index = (int)listBox1.SelectedIndex - 1;
            if (!(listBox1.SelectedIndex >= 0))
            {
                label1.Text = "Thread is not choosed\n";
                label1.Visible = true;
            }
            else
            {
                label1.Visible = false;
                if (listBox1.SelectedItem.ToString() == "All Threads\n")
                {
                    for (int i = 1; i < thread_id + 1; i++)
                    {
                        Send(2, i, fileText);
                    }
                }
                else if (listBox1.SelectedItem.ToString() == "Main Thread\n")
                {
                    Send(2, 0,  fileText);
                }
                else
                {
                    Send(2, index, fileText);
                }
            }
        }

    }
} 