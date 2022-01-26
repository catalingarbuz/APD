import java.io.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;


public class Tema2 {
    private static Integer nrWorkers;
    private static String fisIn, fisOut;
    public static ConcurrentHashMap<Integer, Integer> dictionar;
    public static ArrayList<String> maxLenWords;
    public static ArrayList<ResultFields> results = new ArrayList<>();

    public static int readArgs(String[] args)
            throws NumberFormatException {
        try {
            nrWorkers = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.out.println("Incorrect number of workers");
            return 1;
        }
        fisIn = args[1];
        fisOut = args[2];

        // Check for correct reading
        if (fisIn == null || fisOut == null || nrWorkers < 1) {
            return 1;
        }

        return 0;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        //Check for corect number of arguments
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        // Read arguments
        readArgs(args);
        Reader myReader = new Reader(fisIn);
        myReader.readFile();

        // Map && Reduce task
        ExecutorService maptpe = Executors.newFixedThreadPool(nrWorkers);
        ExecutorService reducetpe = Executors.newFixedThreadPool(nrWorkers);
        // For each file do map && reduce
        for (String file : myReader.getFiles()) {
            dictionar =  new ConcurrentHashMap<>();
            maxLenWords = new ArrayList<>();
            maptpe = Executors.newFixedThreadPool(nrWorkers);
            RandomAccessFile inFileReader = new RandomAccessFile(file, "r");
            // Send to the executor service task to perform map operation on the current file
            maptpe.submit(new MyMapTask(inFileReader, 0, myReader.getFragD(), maptpe));
            maptpe.awaitTermination(200, TimeUnit.MILLISECONDS);
            // Perform reduce operation on the same file
            reducetpe.submit(new MyReduceTask(file, dictionar, maxLenWords));
            reducetpe.awaitTermination(100, TimeUnit.MILLISECONDS);
        }
        // Shutdown executor services
        maptpe.shutdown();
        reducetpe.shutdown();

        // Sort results descending after rank
        results.sort(new Comparator<ResultFields>() {
            @Override
            public int compare(ResultFields o1, ResultFields o2) {
                return Double.compare(o2.getRank(), o1.getRank());
            }
        });

        // Write results in the output file
        FileWriter writer = new FileWriter(fisOut);
        for (ResultFields res : results) {
            writer.write(res.getFileName() + "," + String.format("%.2f", res.getRank()) +
                    "," + res.getMaxWordLen() + "," + res.getNrOfWords() + "\n");
        }
        writer.close();
    }
}