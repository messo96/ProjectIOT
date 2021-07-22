package ProgettoIOT;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Window;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import seiot.modulo_lab_4_1.CommChannel;
import seiot.modulo_lab_4_1.SerialCommChannel;

public class DamDashboard extends JFrame {
	private CommChannel channel;
	private JLabel label_status;
	private JLabel label_diga;
	private JLabel label_manual;
	private XYSeries series_pos;
	private ChartPanel cP_pos;
	
	public DamDashboard() throws Exception {
		
		XYSeriesCollection dataset_pos = new XYSeriesCollection();
		JFreeChart chart_pos = ChartFactory.createXYLineChart("Position", "Time(ms)", "Position(mm)", dataset_pos,
				PlotOrientation.VERTICAL, true, true, false);
		chart_pos.getXYPlot().getRenderer().setSeriesPaint(0, new Color(255, 40, 0));
		series_pos = new XYSeries("Position");
		
		dataset_pos.addSeries(series_pos);
		
		channel = new SerialCommChannel("/dev/ttyACM0", 115200);
		 JPanel panel = new JPanel();  
	     panel.setLayout(new FlowLayout());  
	     panel.add(new JLabel("Status"));
	     label_status = new JLabel();
	     label_diga = new JLabel();
	     label_manual = new JLabel();
	     panel.add(label_status);  
	     panel.add(label_diga);
	     panel.add(label_manual);
	     label_manual.setText("MANUAL CONTROL:");
	     this.add(panel);  
	     cP_pos = new ChartPanel(chart_pos);
	     panel.add(cP_pos);
	     
	     this.setSize(1082, 860);  
	     this.setLocationRelativeTo(null);  
	     this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
	     this.setVisible(true);  
	}	
	
	public String getMessage() throws Exception {
		return channel.receiveMsg();
	}
	
	public void changeStatus(String status) {
		label_status.setText(status);
	}
	
	public void addValueToChart(float time, float value) {
		series_pos.add(time, value);
		cP_pos.repaint();
	}
	
	public void setDiga(int percentage) {
		label_diga.setText("APERTURA DIGA: " + percentage + "%");
	}
	
	public void setManual(boolean flag) {
		label_manual.setText("MANUAL CONTROL: " + (flag ? "ACTIVE" : "INACTIVE") );
	}
	
	
	
	public static void main(String s[]) throws Exception {  
		DamDashboard dd = new DamDashboard();
		int time = 0;
		
		
		while(true) {
			time += 100;
			String str = dd.getMessage();
			String value[] = str.split(";");
			System.out.println(str);

			switch(value[0]) {
			case "ALLARME":
				dd.changeStatus(value[0]);
				dd.addValueToChart(time, Float.parseFloat(value[1])); 
				dd.setDiga(Integer.parseInt(value[2]));

				break;
			case "PREALLARME":
				dd.changeStatus(value[0]);
				dd.addValueToChart(time, Float.parseFloat(value[1])); 

				break;
			case "NORMALE":
				dd.changeStatus(value[0]);

				break;
			case "MANUAL":
				
			}
		}
	}
}


