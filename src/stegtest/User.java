/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author nishan
 */
public class User {
    
    private int id;
    private String name;
    private String department;
    private String contactnumber;
    private String email;
    private String username;
    private String password;
    private String adminusername;
    private String adminpassword;
    
    public void setID(int id) {
        this.id = id;
    }
    
    public int getID() {
        return id;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getName() {
        return name;
    }
    
    public void setDepartment(String department) {
        this.department = department;
    }
    
    public String getDepartment() {
        return department;
    }
    
    public void setContactnumber(String contactnumber) {
        this.contactnumber = contactnumber;
    }
    
    public String getContactnumber() {
        return contactnumber;
    }
    
    public void setEmail(String email) {
        this.email = email;
    }
    
    public String getEmail() {
        return email;
    }
    
    public void setUsername(String username) {
        this.username = username;
    }
    
    public String getUsername() {
        return username;
    }
    
    public void setPassword(String password) {
        this.password = password;
    }
    
    public String getPassword() {
        return password;
    }
    public void setAdminusername(String adminusername) {
        this.adminusername = adminusername;
    }
    
    public String getAdminusername() {
        return adminusername;
    }
    public void setAdminpassword(String adminpassword) {
        this.adminpassword = adminpassword;
    }
    
    public String getAdminpassword() {
        return adminpassword;
    }
}
