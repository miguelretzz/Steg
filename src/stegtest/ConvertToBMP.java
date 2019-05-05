import javax.imageio.*;
import java.awt.image.*;
import java.io.File;

/** Converts image to BMP image and deletes it
 */
public class ConvertToBMP{
    public boolean convertToBMP(String imgFile){
        try{
            BufferedImage img = ImageIO.read(new File(imgFile));

            ImageIO.write(img,new String("bmp"),new File("C:\\WINDOWS\\Temp\\temp.bmp"));
            return true;
        }catch(Exception ex){
            return false;
        }
    }
    void deleteTempImage(){
        try{
            File f = new File("C:\\WINDOWS\\Temp\temp.bmp");
            f.delete();
        }catch(Exception ex){
        }
    }
}
