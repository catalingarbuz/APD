import java.io.*;
import java.util.concurrent.ExecutorService;

public class MyMapTask implements Runnable {
    private final RandomAccessFile file;
    private int offset;
    private final int fragDimension;
    private final ExecutorService tpe;

    public MyMapTask(RandomAccessFile file, int offset, int fragDimension, ExecutorService tpe) {
        this.file = file;
        this.offset = offset;
        this.fragDimension = fragDimension;
        this.tpe = tpe;
    }

    @Override
    public void run() {
        byte[] bytes = new byte[fragDimension + 50];
        try {
            if (offset > file.length()) {
                tpe.shutdown();
            }
            //Read fragment dimension bytes from the calculated offset
            file.seek(offset);
            file.read(bytes,0,fragDimension);
            offset += fragDimension;

            //Adjust fragment and next offset if it ends in the middle of a word
            int idx = fragDimension;
            char c = (char)(bytes[fragDimension - 1]);
            if (Character.isLetter(c)) {
                while(Character.isLetter(c) && offset < file.length()) {
                    c = (char)file.read();
                    bytes[idx] = (byte) c;
                    offset++;
                    idx++;
                }
            }
            // Convert byte array to string and split words using regex
            String str = new String(bytes);
            String[] words = str.split("\\W+");
            int maxLen = 0;


           for (String word : words) {
               /* Calculate maximum length of a word and
                  update dictionary */
               if (word.length() > 0) {
                   if (maxLen < word.length()) {
                       maxLen = word.length();
                   }
                   Integer val = Tema2.dictionar.putIfAbsent(word.length(), 1);
                   if (val != null) {
                       Tema2.dictionar.put(word.length(), val + 1);
                   }
               }
            }

           // Put words of maximum length in static array of maxLenWords
           for (String word : words) {
               if (word.length() == maxLen) {
                   Tema2.maxLenWords.add(word);
               }
           }
            // Create new task with the new file offset
            tpe.submit(new MyMapTask(file, offset, fragDimension, tpe));
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


}
