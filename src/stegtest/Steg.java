import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;
import javax.swing.JButton;

import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import javax.swing.border.LineBorder;
import javax.swing.plaf.metal.*;




public class Steg extends javax.swing.JFrame {

	{
		//Set Look & Feel
		try {
			javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
		} catch(Exception e) {
			e.printStackTrace();
		}

	}

	private JLabel titleLabel;
	private JTextPane licensePane;
        private JPanel MD5;
	private JPanel unhidingPanel;
	private JPanel hidingPanel;
	private JPanel selectionPanel;
	private JTextPane readmePane;
	private JPanel readmePanel;
	private JTabbedPane containerPane;

	String readme = new String("Kubli");

    // Main Method
	public static void main(String[] args) {

	    Steg inst = new Steg();

		inst.setVisible(true);
	}

	public Steg() {
		super("Kubli");

		setLocation(180,175);
		setResizable(false);
		initGUI();
	}

	private void initGUI() {
		try {
			setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
			getContentPane().setLayout(null);
			{
				titleLabel = new JLabel();
				getContentPane().add(titleLabel);
				titleLabel.setText("Kubli");
				titleLabel.setFont(new java.awt.Font("Times New Roman",1,20));
				titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
				titleLabel.setBounds(0, 0, 595, 49);
			}
			{
				containerPane = new JTabbedPane();
				getContentPane().add(containerPane);
				containerPane.setBounds(0, 49, 595, 371);
				containerPane.setTabPlacement(JTabbedPane.LEFT);
				containerPane.setFont(new java.awt.Font("Times New Roman",0,16));
				
				{
					hidingPanel = new HideGUI();
					containerPane.addTab("Hiding", null, hidingPanel, null);
					hidingPanel.setPreferredSize(new java.awt.Dimension(459,371));
					hidingPanel.setOpaque(false);
                        	}
				{
					unhidingPanel = new UnhideGUI();
					containerPane.addTab("UnHiding", null, unhidingPanel, null);
					unhidingPanel.setOpaque(false);
				}
			
			}
			pack();
			this.setSize(610, 461);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}