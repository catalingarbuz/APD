import java.io.File;

public class ResultFields {
    private String fileName;
    private Integer maxWordLen, nrOfWords;
    private Double rank;

    public ResultFields(String fileName, Integer mWL, Integer nr, Double rank) {
        this.fileName = fileName;
        this.maxWordLen = mWL;
        this.nrOfWords = nr;
        this.rank = rank;
    }

    public String getFileName() {
        File f = new File(fileName);
        return f.getName();
    }

    public Integer getMaxWordLen() {
        return maxWordLen;
    }

    public Integer getNrOfWords() {
        return nrOfWords;
    }

    public Double getRank() {
        return rank;
    }
}
