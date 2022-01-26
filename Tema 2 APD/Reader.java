import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class Reader {
    private String fileIn;
    private int fragD, nrOfFiles;
    private ArrayList<String> files;

    public Reader(String fileName)
    {
        setFileIn(fileName);
        this.fragD = 0;
        this.nrOfFiles = 0;
        this.files = new ArrayList<String>();
    }

    public void readFile() {
        try {
            RandomAccessFile inFileReader = new RandomAccessFile(getFileIn(), "r");
            int offSet = 0;
            long fileSize = inFileReader.length();
            String line;
          // Select first fragment dimension && number of files
          try {
              if ((line = inFileReader.readLine()) != null) {
                  setFragD(Integer.parseInt(line));
              }
              if ((line = inFileReader.readLine()) != null) {
                  setNrOfFiles(Integer.parseInt(line));
              }
          } catch (Exception e) {
              e.printStackTrace();
          }
          while (((line = inFileReader.readLine()) != null)) {
              files.add(line);
          }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String getFileIn() {
        return fileIn;
    }

    public void setFileIn(String fileIn) {
        this.fileIn = fileIn;
    }

    public int getFragD() {
        return fragD;
    }

    public void setFragD(int fragD) {
        this.fragD = fragD;
    }

    public int getNrOfFiles() {
        return nrOfFiles;
    }

    public void setNrOfFiles(int nrOfFiles) {
        this.nrOfFiles = nrOfFiles;
    }

    public ArrayList<String> getFiles() {
        return files;
    }

}
