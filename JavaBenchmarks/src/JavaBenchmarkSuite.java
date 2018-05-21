import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class JavaBenchmarkSuite {
    private static int ARRAY_SIZE = 1000;
    private ArrayList<Integer> numbers;


    private long measureStaticMemoryAllocation(int n){
        long start = System.nanoTime();
        for (int i = 0; i < n; i++) {
            int[] array = new int[n];
        }
        long stop = System.nanoTime();
        long elapsed = stop - start;
        return elapsed;
    }

    private long measureDynamicMemoryAllocation(int n){
        long start = System.nanoTime();
        for (int i = 0; i < n; i++) {
            int arr[] = new int[ARRAY_SIZE];
        }
        long stop = System.nanoTime();
        long elapsed = stop - start;
        return elapsed;
    }

    private int measureMemoryAccess(int n){
        return 0;
    }


    public void RunBenchmarks(){
        int n = 1000;
        System.out.println("STATICMEM, Java, " + measureStaticMemoryAllocation(n) /1000000.0 + " ms");
        System.out.println("DYNAMICMEM, Java, " + measureDynamicMemoryAllocation(n) /1000000.0 + " ms");
        //System.out.println("MEMACCESS, Java, " + measureMemoryAccess(n) + " ns");
    }

    public static void main(String args[]){
        JavaBenchmarkSuite benchmarkSuite = new JavaBenchmarkSuite();
        benchmarkSuite.RunBenchmarks();
    }
}
