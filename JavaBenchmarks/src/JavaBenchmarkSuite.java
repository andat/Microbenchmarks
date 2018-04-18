import java.text.SimpleDateFormat;
import java.util.Date;

public class JavaBenchmarkSuite {
    private static int ARRAY_SIZE = 1000;


    private long measureStaticMemoryAllocation(int n){
        long start = System.nanoTime();
        for (int i = 0; i < n; i++) {
            int array[];
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
        int n = 100;
        System.out.println("STATICMEM, Java, " + measureStaticMemoryAllocation(n) + " ns");
        System.out.println("DYNAMICMEM, Java, " + measureDynamicMemoryAllocation(n) + " ns");
        //System.out.println("MEMACCESS, Java, " + measureMemoryAccess(n) + " ns");
    }

    public static void main(String args[]){
        JavaBenchmarkSuite benchmarkSuite = new JavaBenchmarkSuite();
        benchmarkSuite.RunBenchmarks();
    }
}
