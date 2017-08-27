#define test_code
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.Management;
namespace Demo
{

    public partial class Form1 : Form
    {
        private int FSystemID, FChlAddr;
        float[,] output = {{0F,0F,0F,0F,0F,4.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,4.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,10F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,.8F,.8F,.8F,.8F,.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,4F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,10F,10F,10F,0F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,0F,0F,10F,10F,10F,10F,10F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{4F,7.92F,0F,0F,3.92F,4F,0F,0F,3.92F,0F,8F,8F,4F,7.92F,4F,8F,7.92F,4F,4F,4F,10.5F,8F,4F,4F,7.92F,4F,0F,0F,3.92F,0F,4F,4F,3.92F,0F,0F,4F,3.92F,0F,0F,0F,8F,4F,0F,0F,3.92F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F}};


        /*{{0F,0F,0F,0F,0F,4.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,4.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,10F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,.8F,.8F,.8F,.8F,.8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,8F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,10F,10F,10F,0F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,7F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,5F,0F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,10F,0F,0F,10F,10F,10F,10F,10F,10F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{4F,7.92F,0F,0F,3.92F,4F,0F,0F,3.92F,0F,8F,8F,4F,7.92F,4F,8F,7.92F,4F,4F,4F,11.92F,8F,4F,4F,7.92F,4F,0F,0F,3.92F,0F,4F,4F,3.92F,0F,0F,4F,3.92F,0F,0F,0F,8F,4F,0F,0F,3.92F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,1.5F,0F,0F,1.5F,0F,0F,1.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,3.5F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,2F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F},
{0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F,0F}};
temp*/
        int loop_count = 0;
        const float RUntime = 285;
        const float Stoptime = 195;
        public void ShowLog(string AMsg)
        {

        }
        int runtime_i;
        public void ReadMeasure()
        {
        }
        public Form1()
        {
            InitializeComponent();
            FTXObjCtrl.LoadObj();
            Control.CheckForIllegalCrossThreadCalls = false;

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            FTXObjCtrl.FreeObj();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            //this.Close();
            if (test_string == false)
            {
                test_string = true;
                btnExit.Text = "Pause";
            }
            else
            {
                test_string = false;
                btnExit.Text = "Continue";
            }
        }
        Boolean start = true;
        Boolean stop_thread = false;
        /*
        string[] Device_Ip = { "192.168.0.121" , "192.168.0.122" , "192.168.0.123" , "192.168.0.124" , "192.168.0.125" };
        int[] FSsystemID = { 1, 62, 61, 59, 55, };
        */
        
        string[] Device_Ip = { "192.168.0.121", "192.168.0.122", "192.168.0.123", "192.168.0.124", "192.168.0.125" };
        int[] FSsystemID = { 1, 61, 59, 8, 47, }; //notebook
        int[] FChladdr = { 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171};
        private void btnOnekeyTest_Click(object sender, EventArgs e)
        {
            int i,k;
            FSystemID = 0;
            FChlAddr = 161;
            string sOutBSTR;
            int n1, nRet;
            Thread myThread = new Thread(ThreadMethod);
            starttime.Text = System.DateTime.Now.ToString();
            //if (start == true)
            btnOnekeyTest.Enabled = false;
            btnExit.Enabled = true;
            runtime_i = 0;
            runtime.Text = "0:0";
            //myThread.Start();
            //start = true;
            stop_thread = false;
            test_string = true;
            second_count = 0;
            loop_count = 0;
            output_ampare = 0;
            runtime_hour = 0;
            {
                stop_thread = false;
                start = false;
                FTXObjCtrl.LoadObj();
//                float temp = Convert.ToSingle(textBox1.Text);
                for (i = 0; i < 5; i++)
                {
                    FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                    for (k = 0; k < 12; k++)
                    {
                        FTXObjCtrl.SetProtect(FSsystemID[i], FChladdr[k], 1, 20F);
                        FTXObjCtrl.SetProtect(FSsystemID[i], FChladdr[k], 3, 150F);
                        FTXObjCtrl.SetFunctionMode(FSsystemID[i], FChladdr[k], 0);
                        
                        FTXObjCtrl.SetInput(FSsystemID[i], FChladdr[k], 1);
                        //FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], Convert.ToSingle(ampere.Text));
                    }
                    FTXObjCtrl.Close();
                }

            
                myThread.Start();
            }
 
        }
        Boolean test_string = true;
        UInt32 second_count = 0;
        float output_ampare, output_ampare1;
        UInt32 delaytime;
        int j1 = 0;
        public void ThreadStart()
        {
            int nRet,i,j,k;
            if (loop_count < RUntime * 60)
            {
                if (second_count == 3600)
                    second_count = 0;

                if (second_count <= 2760)
                {
                    if (second_count == 0)
                    {
                        label6.Text = (second_count / 60 + 1).ToString();
                        output_ampare = 0;
                        output_ampare1 = 0;
                        j = (int)(second_count / 60);
                        for (i = 0; i < 2; i++)
                        {
                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], output[i * 12 + k, j]);
                                output_ampare = output[i * 12 + k, j] + output_ampare;
                            }

                            FTXObjCtrl.Close();
                        }
                        for (i = 0; i < 3; i++)
                        {
                            FTXObjCtrl.OpenLAN(Device_Ip[i + 2], 7000, 1);
                            if (i == 0)
                            {
                                for (k = 6; k < 12; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k - 6, j]);
                                    output_ampare1 = output[k - 6, j] + output_ampare1;
                                }
                            }
                            else if (i == 1)
                            {
                                for (k = 0; k < 12; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k + 6, j]);
                                    output_ampare1 = output[k + 6, j] + output_ampare1;
                                }
                            }
                            else if (i == 2)
                            {
                                for (k = 0; k < 6; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k + 18, j]);
                                    output_ampare1 = output[k + 18, j] + output_ampare1;
                                }
                            }
                            FTXObjCtrl.Close();

                        }
                        FTXObjCtrl.OpenLAN(Device_Ip[2], 7000, 1);
                        FTXObjCtrl.SetCCValue(FSsystemID[2], FChladdr[0], 0);
                        FTXObjCtrl.Close();
                        FTXObjCtrl.OpenLAN(Device_Ip[4], 7000, 1);
                        FTXObjCtrl.SetCCValue(FSsystemID[4], FChladdr[6], 0);
                        FTXObjCtrl.Close();
                        ampere.Text = output_ampare.ToString();
                        label7.Text = output_ampare1.ToString();

                    }
                    else if ((second_count % 60) == 0)
                    {
                        delaytime = second_count;
                        label6.Text = (second_count / 60 + 1).ToString();
                        output_ampare = 0;
                        output_ampare1 = 0;
                        j1 = (int)(second_count / 60);
                        i = 0;

                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {
                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], output[i * 12 + k, j1]);
                            output_ampare = output[i * 12 + k, j1] + output_ampare;
                        }

                        FTXObjCtrl.Close();
                    }
                    else if(delaytime != 0 )
                    {
                        if ((second_count - delaytime) == 1)
                        {
                            i = 1;

                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], output[i * 12 + k, j1]);
                                output_ampare = output[i * 12 + k, j1] + output_ampare;
                            }

                            FTXObjCtrl.Close();
                            ampere.Text = output_ampare.ToString();
                        }
                        else if ((second_count - delaytime) == 2)
                        {
                            i = 0;
                            FTXObjCtrl.OpenLAN(Device_Ip[i + 2], 7000, 1);
                            if (i == 0)
                            {
                                for (k = 6; k < 12; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k - 6, j1]);
                                    output_ampare1 = output[k - 6, j1] + output_ampare1;
                                }
                            }

                            FTXObjCtrl.Close();
                        }
                        else if ((second_count - delaytime) == 3)
                        {
                            i = 1;
                            FTXObjCtrl.OpenLAN(Device_Ip[i + 2], 7000, 1);
                            if (i == 1)
                            {
                                for (k = 0; k < 12; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k + 6, j1]);
                                    output_ampare1 = output[k + 6, j1] + output_ampare1;
                                }
                            }
                            FTXObjCtrl.Close();
                        }
                        else if ((second_count - delaytime) == 4)
                        {
                            i = 2;
                            FTXObjCtrl.OpenLAN(Device_Ip[i + 2], 7000, 1);
                            if (i == 2)
                            {
                                for (k = 0; k < 6; k++)
                                {
                                    FTXObjCtrl.SetCCValue(FSsystemID[i + 2], FChladdr[k], output[k + 18, j1]);
                                    output_ampare1 = output[k + 18, j1] + output_ampare1;
                                }
                            }
                            FTXObjCtrl.Close();
                            label7.Text = output_ampare1.ToString();
                            FTXObjCtrl.OpenLAN(Device_Ip[2], 7000, 1);
                            FTXObjCtrl.SetCCValue(FSsystemID[2], FChladdr[0], 0);
                            FTXObjCtrl.Close();
                            FTXObjCtrl.OpenLAN(Device_Ip[4], 7000, 1);
                            FTXObjCtrl.SetCCValue(FSsystemID[4], FChladdr[6], 0);
                            FTXObjCtrl.Close();

                        }
                    }
                }
                else
                {
                    if ((second_count % 60) == 0)
                    {
                        delaytime = second_count;
                        label6.Text = (second_count / 60 + 1).ToString();
                        i = 0;
                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {

                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                            output_ampare = 0;


                        }

                        FTXObjCtrl.Close();
                     }
                    else if (( second_count - delaytime) == 1)
                    { 
                        {
                            i = 1;
                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                                output_ampare = 0;

                            }

                            FTXObjCtrl.Close();
                        }
                    }
                    else if (( second_count - delaytime) == 2)
                    {
                        {
                            i = 2;
                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                                output_ampare = 0;

                            }

                            FTXObjCtrl.Close();
                        }
                    }
                    else if (( second_count - delaytime) == 3)
                    {
                        {
                            i = 3;
                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                                output_ampare = 0;

                            }

                            FTXObjCtrl.Close();
                        }
                    }
                    else if (( second_count - delaytime) == 4)
                    {
                        {
                            i = 4;
                            FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                            for (k = 0; k < 12; k++)
                            {
                                FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                                output_ampare = 0;

                            }

                            FTXObjCtrl.Close();
                            label7.Text = output_ampare1.ToString();
                        }
                    }
                    ampere.Text = output_ampare.ToString();
                }
           }
            else
            {
                if ((second_count % 60) == 0)
                {
                    delaytime = second_count;
                    label6.Text = (second_count / 60 + 1).ToString();
                    i = 0;
                    FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                    for (k = 0; k < 12; k++)
                    {

                        FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                        output_ampare = 0;


                    }

                    FTXObjCtrl.Close();
                }
                else if (( second_count - delaytime) == 1)
                {
                    {
                        i = 1;
                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {
                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                            output_ampare = 0;

                        }

                        FTXObjCtrl.Close();
                    }
                }
                else if (( second_count - delaytime) == 2)
                {
                    {
                        i = 2;
                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {
                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                            output_ampare = 0;

                        }

                        FTXObjCtrl.Close();
                    }
                }
                else if (( second_count - delaytime) == 3)
                {
                    {
                        i = 3;
                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {
                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                            output_ampare = 0;

                        }

                        FTXObjCtrl.Close();
                    }
                }
                else if (( second_count - delaytime) == 4)
                {
                    {
                        i = 4;
                        FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                        for (k = 0; k < 12; k++)
                        {
                            FTXObjCtrl.SetCCValue(FSsystemID[i], FChladdr[k], 0);
                            output_ampare = 0;

                        }

                        FTXObjCtrl.Close();
                    }
                    label7.Text = output_ampare1.ToString();
                }
                ampere.Text = output_ampare.ToString();
            }


            if (second_count != 3600)
                second_count = second_count + 1;
            else
                second_count = 0;
            if (loop_count != (RUntime + Stoptime ) * 60)
            {
                loop_count = loop_count + 1;

            }
            else
            {
                loop_count = 0;
                second_count = 0;
            }
        }
        float output_a = 0.1f;

        private void button1_Click(object sender, EventArgs e)
        {
            int i, k;
            start = true;
            stop_thread = true;
            
            for (i = 0; i < 5; i++)
            {
                FTXObjCtrl.OpenLAN(Device_Ip[i], 7000, 1);
                for (k = 0; k < 12; k++)
                {

                    FTXObjCtrl.SetInput(FSsystemID[i], FChladdr[k], 0);

                }
                FTXObjCtrl.Close();
            }
            
            btnExit.Enabled = false;
            btnOnekeyTest.Enabled = true;
            second_count = 0;

            FTXObjCtrl.FreeObj();
            //FTXObjCtrl.FreeObj();
        }
        int runtime_hour = 0;
        double Msecs = 0;
        private void Form1_Load(object sender, EventArgs e)
        {

        }
        static int delay_time;
        private void ThreadMethod()
        {
            while (true)
            {
                if (stop_thread == true)
                    break;
                if (test_string == true)
                {
                    //delay_time = (int)(1000f - Msecs);
                    Thread.Sleep(970);  //线程暂停100毫秒 
                 
                    runtime_i = runtime_i + 1;
                    if (runtime_i == 60)
                    {
                        runtime_hour = runtime_hour + 1;
                        runtime_i = 0;
                    
                    }


                    runtime.Text = runtime_hour.ToString() + ":" + runtime_i.ToString();
                    TimeSpan ts1 = Process.GetCurrentProcess().TotalProcessorTime;
                    ThreadStart();
                    //TimeSpan ts11 = Process.GetCurrentProcess().TotalProcessorTime;
                    Msecs = Process.GetCurrentProcess().TotalProcessorTime.Subtract(ts1).TotalMilliseconds;
                    delay_time = (int)(1000f - Msecs);
                }
            }

        }
    }
}
