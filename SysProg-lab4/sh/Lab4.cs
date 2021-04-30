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

        bool Child = false;


        //[DllImport("..\\AnonymousDll.dll", CharSet = CharSet.Ansi)]
        //private static extern void Send(StringBuilder Str);


        public Lab4() { InitializeComponent(); }


        private void Start_Click(object sender, EventArgs e)
        {
            
        }

        private void Stop_click(object sender, EventArgs e)
        {
            
        }


        private void SendButton_Click(object sender, EventArgs e)
        {
            
        }

    }
} 