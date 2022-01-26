import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

public class MyReduceTask implements Runnable{
    private String filename;
    ConcurrentHashMap<Integer, Integer> dictionar;
    private final ArrayList<String> maxLenWords;

    public MyReduceTask(String f, ConcurrentHashMap<Integer, Integer>dictionar, ArrayList<String> maxLenWords) {
        this.filename = f;
        this.dictionar = dictionar;
        this.maxLenWords = maxLenWords;
    }

    @Override
    public void run() {
        int maxLen = 0;
        int countWords = 0;
        //Identify maximum length && numbers of words with that length
        for (String word : maxLenWords) {
            if (word.length() == maxLen) {
                countWords++;
            }
            if (word.length() > maxLen) {
                maxLen = word.length();
                countWords = 1;
            }
        }
        //Calculate rank of the file
        int sum = 0;
        int nrOfWords = 0;
        double rank = 0;
        for (int i = 1; i <= maxLen; i++) {
            if (dictionar.get(i) != null) {
                sum += nFibonacci(i+1) * dictionar.get(i);
                nrOfWords += dictionar.get(i);
            }
        }
        rank = (double)sum / (double)nrOfWords;
        ResultFields res = new ResultFields(filename, maxLen, countWords, rank);

        // Add the calculated result in the static array of results
        Tema2.results.add(res);
    }

    // Function to return n'th Fibonacci number
    public int nFibonacci(int n) {
        if (n <= 1) {
            return n;
        }
        return nFibonacci(n - 1) + nFibonacci(n - 2);
    }
}
