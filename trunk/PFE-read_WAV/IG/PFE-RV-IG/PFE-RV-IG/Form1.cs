using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ZedGraph;

namespace PFE_RV_IG
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        // Respond to the form 'Resize' event
        private void Form1_Resize(object sender, EventArgs e)
        {
            //SetSize();
        }

        // SetSize() is separate from Resize() so we can 
        // call it independently from the Form1_Load() method
        // This leaves a 10 px margin around the outside of the control
        // Customize this to fit your needs
        private void SetSize()
        {
            zedGraphControl1.Location = new Point(10, 10);
            // Leave a small margin around the outside of the control
            zedGraphControl1.Size = new Size(ClientRectangle.Width - 20,
                                    ClientRectangle.Height - 20);
        }

        // Respond to form 'Load' event
        private void Form1_Load(object sender, EventArgs e)
        {
            // Setup the graph
            //CreateGraph(zedGraphControl1);
            // Size the control to fill the form with a margin
            //SetSize();
        }

        // Build the Chart
        private void CreateGraph(ZedGraphControl zgc)
        {
            // get a reference to the GraphPane
            GraphPane myPane = zgc.GraphPane;

            // Set the Titles
            myPane.Title.Text = "My Test Graph\n(For CodeProject Sample)";
            myPane.XAxis.Title.Text = "My X Axis";
            myPane.YAxis.Title.Text = "My Y Axis";

            // Make up some data arrays based on the Sine function
            double x, y1, y2;
            PointPairList list1 = new PointPairList();
            PointPairList list2 = new PointPairList();
            for (int i = 0; i < 36; i++)
            {
                x = (double)i + 5;
                y1 = 1.5 + Math.Sin((double)i * 0.2);
                y2 = 3.0 * (1.5 + Math.Sin((double)i * 0.2));
                list1.Add(x, y1);
                list2.Add(x, y2);
            }

            // Generate a red curve with diamond
            // symbols, and "Porsche" in the legend
            LineItem myCurve = myPane.AddCurve("Porsche",
                  list1, Color.Red, SymbolType.Diamond);

            // Generate a blue curve with circle
            // symbols, and "Piper" in the legend
            LineItem myCurve2 = myPane.AddCurve("Piper",
                  list2, Color.Blue, SymbolType.Circle);
            // Tell ZedGraph to refigure the
            // axes since the data have changed
            zgc.AxisChange();
        }

        private void UpdateGraph(ZedGraphControl zgc)
        {
            // get a reference to the GraphPane
            GraphPane myPane = zgc.GraphPane;

            // Set the Titles
            myPane.Title.Text = "Contenu fichier WAV";
            myPane.XAxis.Title.FontSpec.IsBold = true;
            myPane.XAxis.Title.FontSpec.Size = 18;
            myPane.YAxis.Title.FontSpec.Size = 18;
            myPane.Title.FontSpec.FontColor = Color.Red;
            myPane.Title.FontSpec.Size = 22;

            myPane.XAxis.Title.Text = "";
            myPane.YAxis.Title.Text = "";
            myPane.XAxis.Scale.IsUseTenPower = true;
            myPane.YAxis.Scale.IsUseTenPower = true;
   
            // Make up some data arrays based on the Sine function
 
            double x, y1, y2;
            PointPairList list1 = new PointPairList();
            PointPairList list2 = new PointPairList();
            for (int i = 0; i < wav_.data_length_get(); i++)
            {
                list1.Add(i, wav_.data_get()[i]);
            }

            // Generate a red curve with diamond
            // symbols, and "Porsche" in the legend
            LineItem myCurve = myPane.AddCurve("",
                  list1, Color.FromArgb(100, 10, 200), SymbolType.None);
            myPane.Fill = (new Fill(Color.White, Color.White, 0));
            myPane.Chart.Fill = (new Fill(Color.FromArgb(192, 192, 255), Color.FromArgb(255, 255, 210), -45F));
            
            // Generate a blue curve with circle
            // symbols, and "Piper" in the legend
            //LineItem myCurve2 = myPane.AddCurve("Piper",
            //      list2, Color.Blue, SymbolType.Circle);

            // Tell ZedGraph to refigure the
            // axes since the data have changed
            zgc.AxisChange();
            zgc.Refresh(); // Invalidate();
        }

        public Wav wav_;

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                wav_ = new Wav(openFileDialog1.FileName);
                txtout.AppendText(Convert.ToString(wav_.data_length_get()) + "\n-------\nValeurs\n-------\n");
                for (int i = 0; i < wav_.data_length_get(); ++i)
                {
                    txtout.AppendText(Convert.ToString(wav_.data_get()[i]) + "\n");
                }
                UpdateGraph(zedGraphControl1);
                zedGraphControl1.Visible = true;
                String[] tmp = openFileDialog1.FileName.Split('\\');
                nom.Text = tmp[tmp.Length - 1];
                taille.Text = Convert.ToString(wav_.header_get().dLen);
                fe.Text = Convert.ToString(wav_.header_get().nSamplesPerSec);
                nbbits.Text = Convert.ToString(wav_.header_get().nBitsPerSample);
                nbcanaux.Text = Convert.ToString(wav_.header_get().nChannels);
            }
        }
    }
}