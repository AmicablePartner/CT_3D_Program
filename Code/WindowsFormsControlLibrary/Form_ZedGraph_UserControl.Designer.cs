namespace WindowsFormsControlLibrary
{
    partial class Form_ZedGraph_UserControl
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.zedGraphControl_Z = new ZedGraph.ZedGraphControl();
            this.zedGraphControl_X = new ZedGraph.ZedGraphControl();
            this.zedGraphControl_Y = new ZedGraph.ZedGraphControl();
            this.SuspendLayout();
            // 
            // zedGraphControl_Z
            // 
            this.zedGraphControl_Z.Location = new System.Drawing.Point(0, -3);
            this.zedGraphControl_Z.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.zedGraphControl_Z.Name = "zedGraphControl_Z";
            this.zedGraphControl_Z.ScrollGrace = 0D;
            this.zedGraphControl_Z.ScrollMaxX = 0D;
            this.zedGraphControl_Z.ScrollMaxY = 0D;
            this.zedGraphControl_Z.ScrollMaxY2 = 0D;
            this.zedGraphControl_Z.ScrollMinX = 0D;
            this.zedGraphControl_Z.ScrollMinY = 0D;
            this.zedGraphControl_Z.ScrollMinY2 = 0D;
            this.zedGraphControl_Z.Size = new System.Drawing.Size(900, 195);
            this.zedGraphControl_Z.TabIndex = 0;
            // 
            // zedGraphControl_X
            // 
            this.zedGraphControl_X.Location = new System.Drawing.Point(0, 193);
            this.zedGraphControl_X.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.zedGraphControl_X.Name = "zedGraphControl_X";
            this.zedGraphControl_X.ScrollGrace = 0D;
            this.zedGraphControl_X.ScrollMaxX = 0D;
            this.zedGraphControl_X.ScrollMaxY = 0D;
            this.zedGraphControl_X.ScrollMaxY2 = 0D;
            this.zedGraphControl_X.ScrollMinX = 0D;
            this.zedGraphControl_X.ScrollMinY = 0D;
            this.zedGraphControl_X.ScrollMinY2 = 0D;
            this.zedGraphControl_X.Size = new System.Drawing.Size(900, 195);
            this.zedGraphControl_X.TabIndex = 0;
            // 
            // zedGraphControl_Y
            // 
            this.zedGraphControl_Y.Location = new System.Drawing.Point(0, 390);
            this.zedGraphControl_Y.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.zedGraphControl_Y.Name = "zedGraphControl_Y";
            this.zedGraphControl_Y.ScrollGrace = 0D;
            this.zedGraphControl_Y.ScrollMaxX = 0D;
            this.zedGraphControl_Y.ScrollMaxY = 0D;
            this.zedGraphControl_Y.ScrollMaxY2 = 0D;
            this.zedGraphControl_Y.ScrollMinX = 0D;
            this.zedGraphControl_Y.ScrollMinY = 0D;
            this.zedGraphControl_Y.ScrollMinY2 = 0D;
            this.zedGraphControl_Y.Size = new System.Drawing.Size(900, 195);
            this.zedGraphControl_Y.TabIndex = 0;
            // 
            // Form_ZedGraph_UserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(900, 583);
            this.Controls.Add(this.zedGraphControl_Y);
            this.Controls.Add(this.zedGraphControl_X);
            this.Controls.Add(this.zedGraphControl_Z);
            this.Name = "Form_ZedGraph_UserControl";
            this.Text = "Form_ZedGraph_UserControl";
            this.ResumeLayout(false);

        }

        #endregion

        private ZedGraph.ZedGraphControl zedGraphControl_Z;
        private ZedGraph.ZedGraphControl zedGraphControl_X;
        private ZedGraph.ZedGraphControl zedGraphControl_Y;
    }
}