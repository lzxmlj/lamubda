namespace Demo
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnExit = new System.Windows.Forms.Button();
            this.btnOnekeyTest = new System.Windows.Forms.Button();
            this.FTXObjCtrl = new AxKc6100TXActiveX.AxKc6100TXControl();
            this.button1 = new System.Windows.Forms.Button();
            this.starttime = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.runtime = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.ampere = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.voltage = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.FTXObjCtrl)).BeginInit();
            this.SuspendLayout();
            // 
            // btnExit
            // 
            this.btnExit.Enabled = false;
            this.btnExit.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnExit.Location = new System.Drawing.Point(349, 313);
            this.btnExit.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(229, 46);
            this.btnExit.TabIndex = 8;
            this.btnExit.Text = "Pause";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // btnOnekeyTest
            // 
            this.btnOnekeyTest.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnOnekeyTest.Location = new System.Drawing.Point(64, 313);
            this.btnOnekeyTest.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.btnOnekeyTest.Name = "btnOnekeyTest";
            this.btnOnekeyTest.Size = new System.Drawing.Size(249, 46);
            this.btnOnekeyTest.TabIndex = 7;
            this.btnOnekeyTest.Text = "Start";
            this.btnOnekeyTest.UseVisualStyleBackColor = true;
            this.btnOnekeyTest.Click += new System.EventHandler(this.btnOnekeyTest_Click);
            // 
            // FTXObjCtrl
            // 
            this.FTXObjCtrl.Enabled = true;
            this.FTXObjCtrl.Location = new System.Drawing.Point(857, 13);
            this.FTXObjCtrl.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.FTXObjCtrl.Name = "FTXObjCtrl";
            this.FTXObjCtrl.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("FTXObjCtrl.OcxState")));
            this.FTXObjCtrl.Size = new System.Drawing.Size(75, 23);
            this.FTXObjCtrl.TabIndex = 12;
            this.FTXObjCtrl.Visible = false;
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(627, 313);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(235, 46);
            this.button1.TabIndex = 13;
            this.button1.Text = "Stop";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // starttime
            // 
            this.starttime.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.starttime.Location = new System.Drawing.Point(64, 233);
            this.starttime.Multiline = true;
            this.starttime.Name = "starttime";
            this.starttime.Size = new System.Drawing.Size(305, 44);
            this.starttime.TabIndex = 15;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(61, 215);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 15);
            this.label1.TabIndex = 16;
            this.label1.Text = "开始时间";
            // 
            // runtime
            // 
            this.runtime.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.runtime.Location = new System.Drawing.Point(432, 233);
            this.runtime.Multiline = true;
            this.runtime.Name = "runtime";
            this.runtime.Size = new System.Drawing.Size(249, 44);
            this.runtime.TabIndex = 15;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(429, 215);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 15);
            this.label2.TabIndex = 16;
            this.label2.Text = "持续时间";
            // 
            // ampere
            // 
            this.ampere.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ampere.Location = new System.Drawing.Point(64, 128);
            this.ampere.Multiline = true;
            this.ampere.Name = "ampere";
            this.ampere.Size = new System.Drawing.Size(249, 44);
            this.ampere.TabIndex = 15;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(61, 110);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(82, 15);
            this.label3.TabIndex = 16;
            this.label3.Text = "当前总电流";
            // 
            // voltage
            // 
            this.voltage.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.voltage.Location = new System.Drawing.Point(432, 128);
            this.voltage.Multiline = true;
            this.voltage.Name = "voltage";
            this.voltage.Size = new System.Drawing.Size(249, 44);
            this.voltage.TabIndex = 15;
            this.voltage.Text = "14V";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(429, 110);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(67, 15);
            this.label4.TabIndex = 16;
            this.label4.Text = "当前电压";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 28.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(459, 443);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(404, 48);
            this.label5.TabIndex = 17;
            this.label5.Text = "电子负载运行模式";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(427, 51);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(15, 15);
            this.label6.TabIndex = 18;
            this.label6.Text = "0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(61, 80);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(15, 15);
            this.label7.TabIndex = 19;
            this.label7.Text = "0";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(945, 552);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.runtime);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.voltage);
            this.Controls.Add(this.ampere);
            this.Controls.Add(this.starttime);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.FTXObjCtrl);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.btnOnekeyTest);
            this.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.Name = "Form1";
            this.Text = "电子负载1_1.0_285_1.4";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.FTXObjCtrl)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Button btnOnekeyTest;
        private AxKc6100TXActiveX.AxKc6100TXControl FTXObjCtrl;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox starttime;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox runtime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox ampere;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox voltage;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
    }
}

