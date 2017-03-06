import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.*;
import java.net.URL;
import java.net.URLConnection;
import java.util.Scanner;


/**
 * Created by Hai Dang on 3/6/2017.
 */
public class TextXml {
    public static void main(String[] args) throws Exception{
        Scanner console = new Scanner(System.in);
        int num;
        try {
            do {
                System.out.println("Please enter a number to append to the url or -1 to quit");
                num = console.nextInt();
                String url = "http://129.32.92.49/xml/grade.php?id=" + num;
                start(url);
            } while(num != -1);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static void start(String urlString) throws Exception
    {
        URL url = new URL(urlString);
        URLConnection connection = url.openConnection();

        Document doc = parseXML(connection.getInputStream());
        NodeList name = doc.getElementsByTagName("name");
        NodeList grade = doc.getElementsByTagName("grade");
        for(int i=0; i< name.getLength();i++)
        {
            System.out.println(name.item(i).getTextContent());
            System.out.println(grade.item(i).getTextContent());
        }
    }

    public static Document parseXML(InputStream stream)
            throws Exception
    {
        DocumentBuilderFactory objDocumentBuilderFactory = null;
        DocumentBuilder objDocumentBuilder = null;
        Document doc = null;
        try
        {
            objDocumentBuilderFactory = DocumentBuilderFactory.newInstance();
            objDocumentBuilder = objDocumentBuilderFactory.newDocumentBuilder();

            doc = objDocumentBuilder.parse(stream);
        }
        catch(Exception ex)
        {
            throw ex;
        }
        return doc;
    }

}
