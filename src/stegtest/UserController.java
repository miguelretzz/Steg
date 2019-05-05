/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 *
 * @author nishan
 */
public class UserController extends User{
    
    Database db;
    Connection con;
    PreparedStatement pst;
    
    public UserController() {
        super();
        db = new Database();
        con = db.getConnection();
    }
    
    public int createAccount(User u) {
        int res = 0;
        String sql = "";
        
        try {
            sql = "INSERT INTO user(`id`,`name`,`department`,`contactnumber`,`email`,`username`,`password`) VALUES(NULL, ?, ?, ?,?,?,?)";
            pst = con.prepareStatement(sql);
            
            
            pst.setString(1, u.getName());
            pst.setString(2, u.getDepartment());
            pst.setString(3, u.getContactnumber());
            pst.setString(4, u.getEmail());
            pst.setString(5, u.getUsername());
            pst.setString(6, u.getPassword());
            
            res = pst.executeUpdate();
            
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        
        return res;
    }
    
    public boolean checkLogin(User u) {
        
        String sql = "";
        ResultSet rs = null;
        
        try {
            sql = "SELECT * FROM user WHERE username = ? and password = ?";
            pst = con.prepareStatement(sql);
            
            pst.setString(1, u.getUsername());
            pst.setString(2, u.getPassword());
            
            rs = pst.executeQuery();
            
            if (rs.next()) {
                return true;
            } else {
                return false;
            }
            
            
        } catch(SQLException e) {
            System.out.println(e.getMessage());
        }
        
        
        return false;
    }
        public boolean checkAdmin(User u) {
        
        String sql = "";
        ResultSet rs = null;
        
        try {
            sql = "SELECT * FROM admin WHERE adminusername = ? and adminpassword = ?";
            pst = con.prepareStatement(sql);
            
            pst.setString(1, u.getAdminusername());
            pst.setString(2, u.getAdminpassword());
            
            rs = pst.executeQuery();
            
            if (rs.next()) {
                return true;
            } else {
                return false;
            }
            
            
        } catch(SQLException e) {
            System.out.println(e.getMessage());
        }
        
        
        return false;
    }
}
