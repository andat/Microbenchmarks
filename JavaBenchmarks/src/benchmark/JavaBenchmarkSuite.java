package benchmark;

import com.sun.org.apache.xpath.internal.SourceTree;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class JavaBenchmarkSuite {
    private static int ARRAY_SIZE = 1000;
    private static int THREAD_SLEEP = 50;
    private ArrayList<Integer> numbers;

    private double measureStaticMemoryAllocation(int n){
        int[] randomArray = new int[ARRAY_SIZE];
        Random rand = new Random();

        for(int i = 0; i< ARRAY_SIZE; i++)
            randomArray[i] = rand.nextInt(ARRAY_SIZE);

        int sum = 0;
        long dummyStart = System.nanoTime();
        sum += randomArray[0];
        long dummyDuration = System.nanoTime() - dummyStart;
        randomArray[0] = sum;
        sum = 0;

        long start = System.nanoTime();
        //declare arrays as local variables
        for (int i = 0; i < n; i++) {
            int local = randomArray[i];
            sum += local;
        }
        long stop = System.nanoTime();
        long elapsed = stop - start;

        //use sum so that the compiler does not optimize it
        randomArray[rand.nextInt(ARRAY_SIZE)] = sum;
        return elapsed / n;
    }

    private double measureDynamicMemoryAllocation(int n){
        int[][] matrix = new int[ARRAY_SIZE][];

        long start = System.nanoTime();
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[ARRAY_SIZE];
        }

        long stop = System.nanoTime();
        long elapsed = stop - start;

        //do something with matrix so that the compiler does not optimize it away
        Random rand = new Random();
        int index = rand.nextInt(ARRAY_SIZE);
        int index1 = rand.nextInt(ARRAY_SIZE);
        matrix[index][index1] *= rand.nextInt(n);

        return elapsed / n;
    }

    private double measureMemoryAccess(int n){
        int[] arr = new int[ARRAY_SIZE];

        long start = System.nanoTime();
        for(int j = 0; j< n; j++) {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                arr[i]++;
            }
        }
        long stop = System.nanoTime();
        long elapsed = stop - start;

        Random rand = new Random();
        arr[rand.nextInt(n)] *= rand.nextInt(n);

        return elapsed / n;
    }

    public double measureThreadCreation(int n){
        double duration = 0.0;

        for(int i = 0; i< n; i++)
            try {
                duration += createThread();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        return duration / n;
    }

    private void foo(){
        int i = 0;
        i += 1;
    }

    public double createThread() throws InterruptedException {
        long fooStart = System.nanoTime();
        foo();
        long fooDuration = System.nanoTime() - fooStart;

        long start = System.nanoTime();
        Thread t = new Thread(() -> {foo();});
        t.start();
        t.join();
        return System.nanoTime() - start - fooDuration;
    }

    void sleep_foo(){
        try{
            Thread.sleep(THREAD_SLEEP);
        } catch (InterruptedException e){

        }
    }

    double measureContextSwitch(int n){
        List<Thread> threads = new ArrayList<>();

        long start = System.nanoTime();
        for(int i = 0; i < n; i++){
            threads.add(new Thread(() -> {sleep_foo();}));
        }

        for(Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        long end = System.nanoTime();
        return (end - start - (THREAD_SLEEP * 1000000))/ n;

    }

    public void RunBenchmarks(int n) {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter("C:\\Users\\zenbookx\\Documents\\Facultate\\An III\\Sem II\\SCS\\project\\JavaBenchmarks\\ java_benchmarks_results.csv"));
            bw.write(n + "\n");
            bw.write("STATICMEM, Java, " + measureStaticMemoryAllocation(n) /1000000.0 + " ms\n");
            bw.write("DYNAMICMEM, Java, " + measureDynamicMemoryAllocation(n) /1000000.0 + " ms\n");
            bw.write("MEMACCESS, Java, " + measureMemoryAccess(n) / 1000000.0+ " ms\n");
            bw.write("THREADCREAT, Java, " + measureThreadCreation(n) / 1000000.0 + " ms\n");
            bw.write("CONTEXTSW, Java, " + measureContextSwitch(n) / 1000000.0 + " ms\n");
            bw.close();
            System.out.println("Java results written to csv.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String args[]){
        JavaBenchmarkSuite benchmarkSuite = new JavaBenchmarkSuite();
        int n;
        if(args.length > 0)
            n = Integer.parseInt(args[0]);
        else
            n = 1000;
        benchmarkSuite.RunBenchmarks(n);
    }
}
