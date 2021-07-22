/**       SMART_EXP project #2 - Java side
 * author: Giovanni Messina
 * Matr: 0000890082
 * E-mail: giovanni.messina3@studio.unibo.it
 * 
 **/

import java.awt.Color;
import java.awt.Dimension;


import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class Viewer {
	public static void main(String[] args) throws Exception {

		boolean flag;
		Integer freq;
		String value[];
		Float pos, vel, acc;
		int time;

		JFrame window = new JFrame();
		window.setTitle("Viewer");
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setPreferredSize(new Dimension(1082, 860));
		JPanel panel = new JPanel();
		panel.setLayout(null);
		window.setContentPane(panel);
		JLabel label_status = new JLabel();
		label_status.setBounds(417, 0, 364, 33);
		panel.add(label_status);
		// CommChannel channel = new SerialCommChannel(args[0],9600);
		CommChannel channel = new SerialCommChannel("/dev/ttyACM1", 9600);
		// CommChannel channel = new SerialCommChannel("/dev/cu.isi00-DevB",9600);

		// Create dataset
		XYSeriesCollection dataset_pos = new XYSeriesCollection();
		XYSeriesCollection dataset_vel = new XYSeriesCollection();
		XYSeriesCollection dataset_acc = new XYSeriesCollection();

		// Create chart
		JFreeChart chart_pos = ChartFactory.createXYLineChart("Position", "Time(ms)", "Position(mm)", dataset_pos,
				PlotOrientation.VERTICAL, true, true, false);

		JFreeChart chart_vel = ChartFactory.createXYLineChart("Velocity", "Time(ms)", "Velocity(mm/ms)", dataset_vel,
				PlotOrientation.VERTICAL, true, true, false);

		JFreeChart chart_acc = ChartFactory.createXYLineChart("Acceleration", "Time(ms)", "Acceleration(mm/ms^2)",
				dataset_acc, PlotOrientation.VERTICAL, true, true, false);

		// Create Panel
		ChartPanel cP_pos = new ChartPanel(chart_pos);
		panel.add(cP_pos);
		cP_pos.setBounds(0, 39, 525, 329);

		ChartPanel cP_vel = new ChartPanel(chart_vel);
		panel.add(cP_vel);
		cP_vel.setBounds(535, 39, 525, 329);

		ChartPanel cP_acc = new ChartPanel(chart_acc);
		panel.add(cP_acc);
		cP_acc.setBounds(279, 371, 525, 340);

		XYSeries series_pos = new XYSeries("Position");
		XYSeries series_vel = new XYSeries("Velocity");
		XYSeries series_acc = new XYSeries("Acceleration");
		// set colors of lines
		chart_pos.getXYPlot().getRenderer().setSeriesPaint(0, new Color(255, 40, 0));
		chart_vel.getXYPlot().getRenderer().setSeriesPaint(0, new Color(40, 255, 0));
		chart_acc.getXYPlot().getRenderer().setSeriesPaint(0, new Color(0, 40, 255));

		dataset_pos.addSeries(series_pos);
		dataset_vel.addSeries(series_vel);
		dataset_acc.addSeries(series_acc);

		window.pack();
		window.setVisible(true);
		label_status.setText("Calibrating..please wait");
		
		while (true) {

			flag = false;
			freq = 1;
			time = 0;

			do {
				value = channel.receiveMsg().split(":");

				switch (value[0]) {

				case "READY":
					label_status.setText("Ready to start!");
					break;
				case "SLEEP":
					label_status.setText("Arduino is entered in sleep mode.. z..z..z.");
					break;
				case "WAKEUP":
					label_status.setText("Arduino is waked up :)");
					break;
				case "FREQ":
					freq = Integer.parseInt(value[1]);
					break;
				case "START":
					label_status.setText("EXPERIMENT IN PROGRESS..");
					series_pos.clear();
					series_vel.clear();
					series_acc.clear();
					break;
				case "WARNING":
					JOptionPane.showMessageDialog(null, value[1], value[0], JOptionPane.WARNING_MESSAGE);
					break;
				case "DATA":
					label_status.setText("Experiment is still in progress...\tFREQUENCY = " + 1000/freq + "Hz");
					pos = Float.parseFloat(value[1]);
					vel = Float.parseFloat(value[2]);
					acc = Float.parseFloat(value[3]);
					time += freq;
					series_pos.add(time, pos);
					series_vel.add(time, vel);
					series_acc.add(time, acc);
					window.repaint();
					cP_pos.repaint();
					cP_vel.repaint();
					System.out.println("Pos:" + pos + "\tVel:" + vel + "\tACC" + acc);
					break;
				case "STOP":
					label_status.setText("Finished!");

					int input_ok = JOptionPane.showConfirmDialog(null, "Esperimento terminated", value[0],
							JOptionPane.OK_CANCEL_OPTION, JOptionPane.INFORMATION_MESSAGE);
					if (input_ok == JOptionPane.OK_OPTION) {
						channel.sendMsg("OK");
					}
					flag = true;

				default:
					break;
				}

			} while (!flag);

		}
	}

}
